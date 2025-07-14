#include <iostream>
#include "Map.hpp"

int main() {
    int n, m, minTroops;

    std::cin >> n >> m;
    Map kingdomMap(n, m);

    kingdomMap.readInput();

    minTroops = kingdomMap.minTroopsToProtectCapital();
    std::cout << minTroops << std::endl;

    return 0;
}