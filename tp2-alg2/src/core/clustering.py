# Implementações das distâncias (Minkowski p, Mahalanobis)
# e dois algoritmos aproximados para k-center (farthest-first e interval-refinement).
# Também inclui runner que executa múltiplas repetições e retorna métricas.
import numpy as np
import time
from sklearn.metrics import silhouette_score, adjusted_rand_score
from sklearn.cluster import KMeans

# -------------------- Distâncias --------------------

"""
Distâncias de Minkowski entre pares (vetorizadas).
X: (n,d)
p: >=1 (1 -> Manhattan, 2 -> Euclidiana)
"""
def minkowski_distance_matrix(X, p=2):
    X = np.asarray(X, dtype=float)
    diff = np.abs(X[:, None, :] - X[None, :, :])    # (n,n,d)
    Dp = np.sum(diff ** p, axis=2)                  # (n,n)
    D = Dp ** (1.0 / p)
    return D

"""
Distâncias de Mahalanobis aos pares usando a covariância de X (linhas = observações).
Implementa d(x,y) = sqrt((x-y)^T S^{-1} (x-y)).
Regulaliza ligeiramente a covariância para evitar singularidades.
"""
def mahalanobis_distance_matrix(X):
    X = np.asarray(X, dtype=float)
    n, d = X.shape
    mu = X.mean(axis=0)
    Xc = X - mu

    if n > 1:
        S = (Xc.T @ Xc) / (n - 1)
    else:
        S = np.eye(d)
    eps = 1e-8
    S_reg = S + eps * np.eye(d)
    Sinv = np.linalg.inv(S_reg)
    D = np.zeros((n, n))
    for i in range(n):
        diff = Xc[i] - Xc         # (n,d)
        t = diff @ Sinv           # (n,d)
        q = np.sum(t * diff, axis=1)
        D[i, :] = np.sqrt(np.maximum(q, 0.0))
    return D

# -------------------- Auxiliares --------------------

"""
Centros: lista de índices em X ou matriz de coordenadas do centro.
Retorna rótulos e raio.
"""
def assign_labels_from_centers(X, centers, metric='minkowski', p=2):
    X = np.asarray(X, dtype=float)
    n = X.shape[0]
    if len(centers) == 0:
        return np.zeros(n, dtype=int), float('inf')

    # Unifica centros para coordenadas
    if isinstance(centers[0], (int, np.integer)):
        centers_coords = X[centers]
    else:
        centers_coords = np.asarray(centers, dtype=float)
    diff = np.abs(X[:, None, :] - centers_coords[None, :, :])  # (n, k, d)
    if metric == 'minkowski' and p == 2:
        Dpc = np.sqrt(np.sum(diff ** 2, axis=2))
    elif metric == 'minkowski' and p == 1:
        Dpc = np.sum(diff, axis=2)
    else:
        Dpc = np.sum(diff ** p, axis=2) ** (1.0 / p)
    labels = np.argmin(Dpc, axis=1)
    radii = Dpc[np.arange(n), labels]

    return labels, float(radii.max() if n > 0 else 0.0)


# -------------------- Algoritmos --------------------

"""
Seleção gulosa de "farthest-first" com centro inicial escolhido aleatoriamente.
Retorna lista de índices, rótulos e raio.
"""
def kcenter_farthest_first(X, k, distance_matrix=None, metric='minkowski', p=2, random_state=None):
    rng = np.random.RandomState(random_state)
    n = X.shape[0]
    if k >= n:
        centers = list(range(n))
        labels, radius = assign_labels_from_centers(X, centers, metric=metric, p=p)
        return centers, labels, radius
    if distance_matrix is None:
        if metric == 'mahalanobis':
            D = mahalanobis_distance_matrix(X)
        else:
            D = minkowski_distance_matrix(X, p=p)
    else:
        D = distance_matrix
    first = int(rng.randint(0, n))
    centers = [first]
    min_dists = D[first].copy()
    for _ in range(1, k):
        nxt = int(np.argmax(min_dists))
        centers.append(nxt)
        min_dists = np.minimum(min_dists, D[nxt])
    labels, radius = assign_labels_from_centers(X, centers, metric=metric, p=p)

    return centers, labels, radius


"""
Refina o raio ótimo via busca binária, parando quando o intervalo de busca = width_frac * initial_width. 
Teste de viabilidade para raio r se baseia na cobertura gulosa removendo pontos dentro de um raio de 2r do centro escolhido.
Retorna lista de índices, rótulos, raio e iteracoes.
"""
def kcenter_interval_refinement(X, k, distance_matrix=None, metric='minkowski', p=2, width_frac=0.05, random_state=None):
    rng = np.random.RandomState(random_state)
    n = X.shape[0]
    if distance_matrix is None:
        if metric == 'mahalanobis':
            D = mahalanobis_distance_matrix(X)
        else:
            D = minkowski_distance_matrix(X, p=p)
    else:
        D = distance_matrix
    low = 0.0
    high = float(np.max(D))
    initial_width = high - low
    target_width = initial_width * width_frac
    iterations = 0
    last_centers = None

    def feasible(r, pick_random=True):
        uncovered = set(range(n))
        centers_local = []
        while uncovered and len(centers_local) <= k:
            if pick_random:
                seed = int(rng.choice(list(uncovered)))
            else:
                seed = next(iter(uncovered))
            centers_local.append(seed)

            # Remove todos os pontos dentro de 2r da seed.
            to_remove = {i for i in uncovered if D[seed, i] <= 2.0 * r + 1e-12}
            uncovered -= to_remove
        return len(centers_local) <= k, centers_local

    while (high - low) > target_width:
        iterations += 1
        mid = (low + high) / 2.0
        ok, centers_try = feasible(mid, pick_random=True)
        if ok:
            high = mid
            last_centers = centers_try
        else:
            low = mid

    if last_centers is None:
        _, last_centers = feasible(high, pick_random=True)
    labels, radius = assign_labels_from_centers(X, last_centers, metric=metric, p=p)

    return last_centers, labels, radius, iterations

# -------------------- Runners --------------------

"""
Executa uma única iteração de um algoritmo e retorna um dicionário com tempo de execução, raio, silhueta e rand ajustado.
"""
def run_experiment_once(X, y_true, k, algorithm='farthest', metric='minkowski', p=2,
                        width_frac=0.05, distance_matrix=None, random_state=None):
    t0 = time.time()
    if algorithm == 'farthest':
        centers, labels, radius = kcenter_farthest_first(X, k, distance_matrix=distance_matrix, metric=metric, p=p, random_state=random_state)
    elif algorithm == 'interval':
        centers, labels, radius, iters = kcenter_interval_refinement(X, k, distance_matrix=distance_matrix, metric=metric, p=p, width_frac=width_frac, random_state=random_state)
    elif algorithm == 'kmeans':
        km = KMeans(n_clusters=k, random_state=random_state)
        labels = km.fit_predict(X)
        centers_coords = km.cluster_centers_

        # Calcula raio com distância euclidiana
        labels, radius = assign_labels_from_centers(X, centers_coords, metric='minkowski', p=2)
    else:
        raise ValueError("unknown algorithm")
    runtime = time.time() - t0

    sil = None
    try:
        if len(set(labels)) >= 2 and len(set(labels)) < X.shape[0]:
            sil = float(silhouette_score(X, labels, metric='euclidean'))
        else:
            sil = None
    except Exception:
        sil = None
    ari = None
    if y_true is not None:
        try:
            ari = float(adjusted_rand_score(y_true, labels))
        except Exception:
            ari = None
    return {
        'runtime': float(runtime),
        'radius': float(radius),
        'silhouette': sil,
        'adjusted_rand': ari,
    }

"""
Executa o conjunto completo de experimentos exigido pela especificação do TP2.
- X: Matrix de dados (n,d)
- y_true: fornecido para determianr k
- p_values: Minkowski p-values para teste
- use_mahalanobis: se True, tambem executa experimentos baseados em Mahalanobis
- algorithms: ['farthest', 'interval', 'kmeans']
- width_fracs: lista de frações de largura usadas interval refinement (5 valores)
- runs: número de repetições (15)
Retorna:
    {
        'meta': {n, d, k},
        'experiments': [
            {
                'metric': 'minkowski_p2',
                'algorithm': 'farthest',
                'runs': [ {...}, {...}, ... ]
            },
            {
                'metric': 'minkowski_p1',
                'algorithm': 'interval',
                'width_frac': 0.05,
                'runs': [ {...}, {...}, ... ]
            },
            ...
        ]
    }
"""
def run_experiments(
    X,
    y_true=None,
    p_values=[1,2],
    use_mahalanobis=False,
    algorithms=['farthest','interval','kmeans'],
    width_fracs=[0.01,0.05,0.1,0.15,0.25],
    runs=15,
    random_seed=0
):
    rng = np.random.RandomState(random_seed)
    X = np.asarray(X, dtype=float)
    n = X.shape[0]

    if y_true is None:
        raise ValueError("O valor y_true deve ser fornecido para determinar k")
    k = int(len(np.unique(y_true)))

    results = {
        'meta': {
            'n': n,
            'd': int(X.shape[1]),
            'k': k
        },
        'experiments': []
    }

    # Pré-calcula todas as matrizes de distância (uma vez por métrica)
    distance_matrices = {}

    # Distancias Minkowski
    for p in p_values:
        key = f"minkowski_p{p}"
        distance_matrices[key] = minkowski_distance_matrix(X, p=p)

    # Mahalanobis
    if use_mahalanobis:
        distance_matrices["mahalanobis"] = mahalanobis_distance_matrix(X)

    # Experimentos para a distancia de Minkowski
    for p in p_values:
        key = f"minkowski_p{p}"
        D = distance_matrices[key]

        for alg in algorithms:
            # Interval refinement, precisa de width_fracs
            if alg == "interval":
                for wf in width_fracs:
                    runs_list = []
                    for _ in range(runs):
                        seed = int(rng.randint(0, 2**31 - 1))
                        res = run_experiment_once(
                            X, y_true, k,
                            algorithm='interval',
                            metric='minkowski',
                            p=p,
                            width_frac=wf,
                            distance_matrix=D,
                            random_state=seed
                        )
                        runs_list.append(res)

                    results["experiments"].append({
                        "metric": key,
                        "algorithm": "interval",
                        "width_frac": wf,
                        "runs": runs_list
                    })

            # Farthest-first ou KMeans
            else:
                runs_list = []
                for _ in range(runs):
                    seed = int(rng.randint(0, 2**31 - 1))
                    res = run_experiment_once(
                        X, y_true, k,
                        algorithm=alg,
                        metric='minkowski',
                        p=p,
                        distance_matrix=D,
                        random_state=seed
                    )
                    runs_list.append(res)

                results["experiments"].append({
                    "metric": key,
                    "algorithm": alg,
                    "runs": runs_list
                })

    # Experimentos para a distância de Mahalanobis
    if use_mahalanobis:
        D = distance_matrices["mahalanobis"]
        for alg in algorithms:
            if alg == "interval":
                for wf in width_fracs:
                    runs_list = []
                    for _ in range(runs):
                        seed = int(rng.randint(0, 2**31 - 1))
                        res = run_experiment_once(
                            X, y_true, k,
                            algorithm='interval',
                            metric='mahalanobis',
                            width_frac=wf,
                            distance_matrix=D,
                            random_state=seed
                        )
                        runs_list.append(res)

                    results["experiments"].append({
                        "metric": "mahalanobis",
                        "algorithm": "interval",
                        "width_frac": wf,
                        "runs": runs_list
                    })

            else:
                runs_list = []
                for _ in range(runs):
                    seed = int(rng.randint(0, 2**31 - 1))
                    res = run_experiment_once(
                        X, y_true, k,
                        algorithm=alg,
                        metric='mahalanobis',
                        distance_matrix=D,
                        random_state=seed
                    )
                    runs_list.append(res)

                results["experiments"].append({
                    "metric": "mahalanobis",
                    "algorithm": alg,
                    "runs": runs_list
                })

    return results
