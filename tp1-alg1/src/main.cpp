#include <iostream>
#include "Graph.hpp"

// Inicia a aplicação e imprime o resultado dos cálculos
int main() {
    Graph graph;

    std::cout << "Parte 1: " << graph.minDistance() << std::endl;
    std::cout << "Parte 2: " << graph.maxDistance() << std::endl;
    
    std::vector<int> articulationPoints = graph.findArticulationPoints();
    std::cout << "Parte 3: " << articulationPoints.size();
    for (int idx : articulationPoints) {
        std::cout << " " << idx;
    }
    std::cout << std::endl;

    return 0;
}
