#include <iostream>
#include "Grid.h"
#include "BFS.h"

int main() {
    std::cout << "=== PROJEKT BEZ VECTOROW ===\n";

    Grid map(10, 10);

    // Sciana
    for(int i = 2; i < 8; i++) map.addObstacle(5, i);

    Point start = {2, 5};
    Point goal = {8, 5};

    std::cout << "Mapa poczatkowa:\n";
    map.print();

    std::cout << "\nSzukam sciezki BFS...\n";
    PathResult path = BFS::findPath(map, start, goal);

    if (path.length == 0) {
        std::cout << "Nie znaleziono sciezki!\n";
    } else {
        std::cout << "Znaleziono! Dlugosc: " << path.length << "\n";
        map.print(path.points, path.length);
        
        // WAŻNE: Sprzątamy pamięć po ścieżce
        path.free();
    }

    // Grid posprząta się sam w destruktorze
    return 0;
}