#include <iostream>
#include <chrono>
#include "Grid.h"
#include "BFS.h"
#include "Dijkstra.h"
#include "AStar.h"
#include "Stats.h"


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
    PathResult testPath;
    do {
        map.clear();
        map.generateObstacles(150, start, goal); // 150 małych ścian  

       for(int i=0; i<300; i++) {
        int mx = rand() % width;
        int my = rand() % height;
        // Nie stawiaj błota na starcie/meczu ani na ścianach
        if((mx != start.x || my != start.y) && (mx != goal.x || my != goal.y)) {
             map.addMud(mx, my, 5); // Koszt 5!
        }

        }
        testPath = BFS::findPath(map, start, goal);

    }while(testPath.length == 0); // Powtarzaj aż będzie ścieżka
    
    
    AlgoStats bfsStats, dijkstraStats, aStarStats;

    // Generujemy trudny teren "błoto", tworzy się kilka plam.
    srand(time(nullptr));
    for(int i=0; i<300; i++) {
        int mx = rand() % width;
        int my = rand() % height;
        // Nie stawiaj błota na starcie/meczu ani na ścianach
        if((mx != start.x || my != start.y) && (mx != goal.x || my != goal.y)) {
             map.addMud(mx, my, 5); // Koszt 5!
        }
    }

    std::cout << "Legenda: . = trawa(1), # = sciana, ~ = bloto(5)\n";


    std::cout << "Mapa poczatkowa:\n";
    map.print();

    // BFS

    std::cout << "\n--- BFS (Ignoruje wagi, idzie na oslep przez bloto) ---\n";
    auto t1 = std::chrono::high_resolution_clock::now();
    PathResult bfsPath = BFS::findPath(map, start, goal, &bfsStats );
    auto t2 = std::chrono::high_resolution_clock::now();
    bfsStats.timeMs = std::chrono::duration<double, std::milli>(t2 - t1).count();

    if (bfsPath.length > 0) {
    std::cout << "Znaleziono! Dlugosc: " << bfsPath.length << "\n";
    // Używamy 'B' dla BFS
    map.print(bfsPath.points, bfsPath.length, 'B'); 
    bfsPath.free();
    }

    // Dijkstra


    std::cout << "\n--- Dijkstra (Omija bloto jesli warto) ---\n";
    t1 = std::chrono::high_resolution_clock::now();
    PathResult dpath = Dijkstra::findPath(map, start, goal, &dijkstraStats );
    t2 = std::chrono::high_resolution_clock::now();
    dijkstraStats.timeMs = std::chrono::duration<double, std::milli>(t2 - t1).count();

    if (dpath.length > 0) {
    std::cout << "Znaleziono! Dlugosc: " << dpath.length << "\n";
    // Używamy 'D' dla Dijkstry
    map.print(dpath.points, dpath.length, 'D');
    dpath.free();
    }

    // A*

     std::cout << "\n--- A* (Heurystyka + wagi) ---\n";
    
    t1 = std::chrono::high_resolution_clock::now();
    PathResult aStarPath = AStar::findPath(map, start, goal, &aStarStats );
    t2 = std::chrono::high_resolution_clock::now();
    aStarStats.timeMs = std::chrono::duration<double, std::milli>(t2 - t1).count();

    if (aStarPath.length > 0) {
    std::cout << "A: Znaleziono! Dlugosc: " << aStarPath.length << "\n";
    // Używamy 'A' dla A*
    map.print(aStarPath.points, aStarPath.length, 'A');
    }

    // ======================
    // Podsumowanie
    // ======================
    std::cout << "\n=== WYNIKI ===\n";
    std::cout << "BFS:      " << bfsStats.timeMs << " ms  odwiedzone : " << bfsStats.visited << " (Moze isc przez bloto, bo ignoruje koszt)\n";
    std::cout << "Dijkstra: " << dijkstraStats.timeMs << " ms  odwiedzone : " << dijkstraStats.visited << " (Szuka najtanszej drogi)\n";
    std::cout << "A*:       " << aStarStats.timeMs << " ms  odwiedzone : " << aStarStats.visited << " (Najszybszy i najdokladniejszy dzieki heurystyce)\n";

    // ======================
    // Porównanie (optymalność)
    // ======================
    if (bfsPath.length > 0 && aStarPath.length > 0) {
        if (bfsPath.length != aStarPath.length) {
            std::cout << "\n[CIEKAWOSTKA] Algorytmy wybraly rozne drogi!\n";
            std::cout << "BFS (krotsza w krokach, ale drozsza): " << bfsPath.length << " krokow.\n";
            std::cout << "A* (moze byc dluzsza w krokach, ale omija bloto): " << aStarPath.length << " krokow.\n";
        }
    }

    bfsPath.free();
    aStarPath.free();

    // Grid posprząta się sam w destruktorze
    return 0;
}