import csv
import numpy as np
import os

"""
Gera um CSV compacto contendo média e desvio padrão das métricas
para cada experimento definido em run_experiments().

results_dict: dicionário retornado por run_experiments
output_path : caminho do CSV de saída
"""
def summarize_results_to_csv(results_dict, output_path):

    rows = []

    # Percorre todos os blocos "experiments" do run_experiments()
    for exp in results_dict["experiments"]:

        algorithm   = exp.get("algorithm")
        metric      = exp.get("metric")
        width_frac  = exp.get("width_frac", None)

        # extrair p (apenas para Minkowski)
        if metric.startswith("minkowski_p"):
            p = int(metric.split("p")[1])
        else:
            p = None

        # extrair lista de execuções
        runs = exp["runs"]

        # coletar métricas válidas
        radius_list = [r["radius"]       for r in runs if r.get("radius")       is not None]
        sil_list    = [r["silhouette"]   for r in runs if r.get("silhouette")   is not None]
        ari_list    = [r["adjusted_rand"] for r in runs if r.get("adjusted_rand") is not None]
        time_list   = [r["runtime"]      for r in runs if r.get("runtime")      is not None]

        def mean(x): return float(np.mean(x)) if len(x) else None
        def std(x):  return float(np.std(x))  if len(x) else None

        row = {
            "algorithm": algorithm,
            "metric": metric,
            "p": p,
            "width_frac": width_frac,

            "radius_mean": mean(radius_list),
            "radius_std":  std(radius_list),

            "silhouette_mean": mean(sil_list),
            "silhouette_std":  std(sil_list),

            "ari_mean": mean(ari_list),
            "ari_std":  std(ari_list),

            "time_mean": mean(time_list),
            "time_std":  std(time_list),
        }

        rows.append(row)

    os.makedirs(os.path.dirname(output_path), exist_ok=True)

    fieldnames = [
        "algorithm", "metric", "p", "width_frac",
        "radius_mean", "radius_std",
        "silhouette_mean", "silhouette_std",
        "ari_mean", "ari_std",
        "time_mean", "time_std"
    ]

    with open(output_path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)

    print(f"CSV resumo salvo em: {output_path}")
