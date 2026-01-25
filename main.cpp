#include <iostream>
#include "Grid.h"
#include "BFS.h"
#include "Dijkstra.h"
#include "AStar.h"


int main() {
    const int width = 50;
    const int height = 30;
    std::cout << "=== PROJEKT BEZ VECTOROW ===\n";

    Grid map(width, height);
/*
    srand(time(nullptr)); 

    int x = rand() % width;
    int y = rand() % height;

    bool vertical = rand() % 2;
    int length = rand() % 10 + 10; 

    // Sciana
    for (int i = 0; i < length; i++) {
        if (vertical)
            map.addObstacle(x, y + i);
        else
             map.addObstacle(x + i, y);
    }
              for(int i = 2; i < 8; i++) map.addObstacle(5, i);
*/
    Point start = {0, 0};
    Point goal = {width - 3, height - 3};
    map.generateObstacles(150, width, height, start, goal); // 20 małych ścian
    

    std::cout << "Mapa poczatkowa:\n";
    map.print();

    std::cout << "\nSzukam sciezki BFS...\n";
    PathResult bfsPath = BFS::findPath(map, start, goal);

    if (bfsPath.length == 0) {
        std::cout << "Nie znaleziono sciezki!\n";
    } else {
        std::cout << "Znaleziono! Dlugosc: " << bfsPath.length << "\n";
        map.print(bfsPath.points, bfsPath.length);
        
        // WAŻNE: Sprzątamy pamięć po ścieżce
        bfsPath.free();
    }
    std::cout << "\nSzukam sciezki Dijkstra...\n";
    PathResult dpath = Dijkstra::findPath(map, start, goal);

    if (dpath.length == 0) {
     std::cout << "Nie znaleziono sciezki!\n";
    } else {
     std::cout << "Znaleziono! Dlugosc: " << dpath.length << "\n";
        map.print(dpath.points, dpath.length);
        dpath.free();
    }
     std::cout << "\nSzukam sciezki A...\n";
     AStarStats st;
     PathResult aStarPath = AStar::findPath(map, start, goal, &st);

    if (aStarPath.length == 0) {
        std::cout << "A: Nie znaleziono sciezki!\n";
    } else {
        std::cout << "A: Znaleziono! Dlugosc: " << aStarPath.length << "\n";
        map.print(aStarPath.points, aStarPath.length);
    }

    // ======================
    // Porównanie (optymalność)
    // ======================
    if (bfsPath.length > 0 && aStarPath.length > 0) {
        if (bfsPath.length == aStarPath.length) {
            std::cout << "\n[OK] A daje sciezke optymalna (taka sama dlugosc jak BFS).\n";
        } else {
            std::cout << "\n[UWAGA] Rozna dlugosc! BFS=" << bfsPath.length
                      << " A*=" << aStarPath.length << "\n";
        }
    }

    bfsPath.free();
    aStarPath.free();

    // Grid posprząta się sam w destruktorze
    return 0;
}