#include "Map.hpp"

// Construtor: inicializa o grid e configura a estrutura do grafo
Map::Map(int rows, int cols) : rows(rows), cols(cols) {
    terrainGrid.resize(rows, std::vector<int>(cols));
    totalNodes = 2 * rows * cols + 2;
    adjacencyList.resize(totalNodes);
    sourceNode = 2 * rows * cols;
    sinkNode = 2 * rows * cols + 1;
}

// Lê o grid do mapa e a posição da capital
void Map::readInput() {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            std::cin >> terrainGrid[i][j];

    std::cin >> capitalRow >> capitalCol;
    --capitalRow;
    --capitalCol;
}

// Retorna o índice do nó de entrada para a célula (i, j)
int Map::getNodeIn(int i, int j) { return (i * cols + j) * 2; }

// Retorna o índice do nó de saída para a célula (i, j)
int Map::getNodeOut(int i, int j) { return (i * cols + j) * 2 + 1; }

// Verifica se a célula está na borda do mapa
bool Map::isBorderCell(int i, int j) { return i == 0 || j == 0 || i == rows - 1 || j == cols - 1; }

// Adiciona uma aresta com capacidade entre dois nós
void Map::addEdge(int from, int to, int capacity) {
    adjacencyList[from].push_back({to, (int)adjacencyList[to].size(), capacity}); 
    adjacencyList[to].push_back({from, (int)adjacencyList[from].size() - 1, 0}); // Capicidade 0 para ser incrementado no grafo residual
}

// Algoritmo de Edmonds-Karp para encontrar o fluxo máximo
int Map::edmondsKarp() {
    int maxFlow = 0;
    std::vector<int> parent(totalNodes);

    while (true) {
        std::vector<int> level(totalNodes, -1);
        std::queue<int> q;
        q.push(sourceNode);
        level[sourceNode] = 0;
        parent[sourceNode] = -1;

        // BFS para encontrar caminho aumentante
        while (!q.empty()) {
            int u = q.front();

            q.pop();

            for (Edge& edge : adjacencyList[u]) {
                if (edge.capacity > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[u] + 1;
                    parent[edge.to] = u;
                    q.push(edge.to);
                    if (edge.to == sinkNode) break;
                }
            }
        }

        // Se não achou caminho, finaliza
        if (level[sinkNode] == -1) break;

        // Encontra o gargalo (mínima capacidade no caminho)
        int bottleneck = INT_MAX;
        int current = sinkNode;

        while (current != sourceNode) {
            int prev = parent[current];

            for (Edge& edge : adjacencyList[prev]) {
                if (edge.to == current && edge.capacity > 0) {
                    bottleneck = std::min(bottleneck, edge.capacity);
                    break;
                }
            }

            current = prev;
        }

        // Atualiza as capacidades no grafo residual
        current = sinkNode;
        while (current != sourceNode) {
            int prev = parent[current];

            for (Edge& edge : adjacencyList[prev]) {
                if (edge.to == current && edge.capacity > 0) {
                    edge.capacity -= bottleneck;
                    adjacencyList[current][edge.reverse].capacity += bottleneck;
                    break;
                }
            }

            current = prev;
        }

        maxFlow += bottleneck;
    }

    return maxFlow;
}

// Monta o grafo e retorna o número mínimo de tropas para proteger a capital
int Map::minTroopsToProtectCapital() {
    int dirX[] = {-1, 1, 0, 0};
    int dirY[] = {0, 0, -1, 1};

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (terrainGrid[i][j] == 0) continue; // Montanha (inacessível)

            int inNode = getNodeIn(i, j);
            int outNode = getNodeOut(i, j);

            // Conecta nó de entrada ao de saída com a capacidade da célula
            addEdge(inNode, outNode, terrainGrid[i][j]);

            // Conecta saída para vizinhos acessíveis
            for (int d = 0; d < 4; ++d) {
                int ni = i + dirX[d], nj = j + dirY[d];
                if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && terrainGrid[ni][nj] != 0) {
                    addEdge(outNode, getNodeIn(ni, nj), INT_MAX);
                }
            }

            // Células na borda estão conectadas à fonte
            if (isBorderCell(i, j)) {
                addEdge(sourceNode, inNode, INT_MAX);
            }
        }
    }

    // Conecta a capital ao sorvedouro
    addEdge(getNodeOut(capitalRow, capitalCol), sinkNode, INT_MAX);

    return edmondsKarp();
}