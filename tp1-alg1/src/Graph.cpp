#include "Graph.hpp"

Graph::Graph() : n(0), timer(0) {
    readInput();
}

// Verifica se dois abrigos estão conectados, ou seja, se suas áreas de cobertura se sobrepõem.
bool Graph::areConnected(const Shelter& a, const Shelter& b) const {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    long long distSq = dx * dx + dy * dy;
    long long sumR = a.r + b.r;
    return distSq <= (long long)sumR * sumR;
}

// Verifica se um ponto (px, py) está dentro da área de cobertura de um abrigo.
// Embora os métodos areConnected e isInside sejam muito parecidos, resolvi criar funções diferentes para separar a semântica. 
bool Graph::isInside(const Shelter& s, int px, int py) const {
    long long dx = s.x - px;
    long long dy = s.y - py;
    long long distSq = dx * dx + dy * dy;
    return distSq <= (long long)s.r * s.r;
}

// Lê a entrada do problema: posições de A e B, os abrigos e seus raios e depois calcula suas conexões.
// Define também quais abrigos contêm os pontos de partida e de chegada.
// Por fim, inicializa as variáveis auxiliares para o DFS.
void Graph::readInput() {
    std::cin >> Ax >> Ay >> Bx >> By >> n;
    shelters.reserve(n);
    connectionsList.resize(n);

    for (int i = 0; i < n; ++i) {
        int r, x, y;
        std::cin >> r >> x >> y;
        shelters.emplace_back(r, x, y);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (areConnected(shelters[i], shelters[j])) {
                connectionsList[i].push_back(j);
                connectionsList[j].push_back(i);
            }
        }
    }

    // Determinar os abrigos de partida e chegada
    for (int i = 0; i < n; ++i) {
        if (isInside(shelters[i], Ax, Ay)) startNodes.push_back(i);
        if (isInside(shelters[i], Bx, By)) endNodes.push_back(i);
    }

    visited.assign(n, false);
    timeIn.assign(n, -1);
    low.assign(n, -1);
    isArticulation.assign(n, false);
}

// Calcula a menor quantidade de abrigos necessários para ir de A até B usando BFS.
int Graph::minDistance() {
    const int INF = 1e9;
    std::vector<int> shelterMinSteps(n, INF);
    std::queue<int> queue;

    for (int node : startNodes) {
        shelterMinSteps[node] = 0;
        queue.push(node);
    }

    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();

        for (int v : connectionsList[u]) {
            if (shelterMinSteps[v] > shelterMinSteps[u] + 1) {
                shelterMinSteps[v] = shelterMinSteps[u] + 1;
                queue.push(v);
            }
        }
    }

    int answer = INF;
    for (int node : endNodes) {
        if (shelterMinSteps[node] < answer) {
            answer = shelterMinSteps[node];
        }
    }

    return (answer == INF ? -1 : answer);
}

// Calcula a maior distância (em número de abrigos) entre quaisquer dois abrigos no grafo.
int Graph::maxDistance() {
    int maxDistance = 0;

    for (int start = 0; start < n; ++start) {
        std::vector<bool> visitedLocal(n, false);
        std::queue<std::pair<int, int>> queue;

        queue.push({start, 0});
        visitedLocal[start] = true;
        
        while (!queue.empty()) {
            int currentNode = queue.front().first;
            int distance = queue.front().second;
            queue.pop();
            
            if (distance > maxDistance) {
                maxDistance = distance;
            }
            
            for (int v : connectionsList[currentNode]) {
                if (!visitedLocal[v]) {
                    visitedLocal[v] = true;
                    queue.push({v, distance + 1});
                }
            }
        }
    }
    
    return maxDistance;
}

// Função de busca em profundidade DFS para encontrar pontos de articulação.
// Mantém registros dos tempos de entrada (tin) e dos menores tempos alcançáveis (low).
void Graph::dfs(int currentNode, int parentNode) {
    int childrenCount = 0;

    visited[currentNode] = true;
    timeIn[currentNode] = low[currentNode] = timer++;
    
    for (int v : connectionsList[currentNode]) {
        if (v == parentNode) continue;

        if (visited[v]) {
            if (low[currentNode] > timeIn[v]) {
                low[currentNode] = timeIn[v];
            }
        } else {
            dfs(v, currentNode);

            if (low[currentNode] > low[v]) {
                low[currentNode] = low[v];
            }
            
            if (low[v] >= timeIn[currentNode] && parentNode != -1) {
                isArticulation[currentNode] = true;
            }

            childrenCount++;
        }
    }
    
    if (parentNode == -1 && childrenCount > 1) {
        isArticulation[currentNode] = true;
    }
}

// Encontra todos os abrigos críticos cuja remoção desconecta componentes,
// chamando `dfs` para todos os vértices não visitados.
std::vector<int> Graph::findArticulationPoints() {
    std::vector<int> criticals;
    timer = 0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs(i, -1);
        }
    }

    for (int i = 0; i < n; ++i) {
        if (isArticulation[i]) {
            // Adiciona + 1 porque o enunciado pede índices começando do 1.
            criticals.push_back(i + 1);
        }
    }

    return criticals;
}