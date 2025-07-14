#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

// Estrutura de aresta com capacidade e referência reversa
struct Edge {
    int to;
    int reverse;
    int capacity;
};

class Map {
private:
    int rows, cols;
    int totalNodes;
    int sourceNode, sinkNode;
    int capitalRow, capitalCol;

    std::vector<std::vector<int>> terrainGrid;
    std::vector<std::vector<Edge>> adjacencyList;

    int getNodeIn(int i, int j);     // Retorna o nó de entrada da célula (i,j)
    int getNodeOut(int i, int j);    // Retorna o nó de saída da célula (i,j)
    bool isBorderCell(int i, int j); // Verifica se a célula está na borda
    void addEdge(int from, int to, int capacity);
    int edmondsKarp();               // Algoritmo de fluxo máximo

public:
    Map(int rows, int cols);
    void readInput();               // Lê o grid e a posição da capital
    int minTroopsToProtectCapital(); // Calcula a quantidade mínima de tropas
};

#endif
