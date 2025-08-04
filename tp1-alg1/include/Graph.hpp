#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <queue>
#include <vector>

struct Shelter {
    int x, y, r;

    Shelter(int r, int x, int y) : x(x), y(y), r(r) {}
};

// Classe que agrupa os métodos de leitura da entrada e cálculo das partes 1, 2 e 3 do trabalho. 
class Graph {
    private:
        int Ax, Ay, Bx, By, n;
        std::vector<Shelter> shelters;
        std::vector<std::vector<int>> connectionsList;
        std::vector<int> startNodes, endNodes;
        
        int timer;
        std::vector<bool> visited;
        std::vector<int> timeIn, low;
        std::vector<bool> isArticulation;

        bool areConnected(const Shelter& a, const Shelter& b) const;
        bool isInside(const Shelter& s, int px, int py) const;
        void dfs(int currentNode, int parentNode);

    public:
        Graph();

        void readInput();
        int minDistance(); 
        int maxDistance(); 
        std::vector<int> findArticulationPoints();
};

#endif