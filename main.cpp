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
    int attempts = 0;
    const int MAX_ATTEMPTS = 100;

    std::cout << "=== PROJEKT BEZ VECTOROW ===\n";

    Grid map(width, height);

    Point start = {0, 0};
    Point goal = {width - 3, height - 3};
    PathResult testPath;
    do {
        attempts++;
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

    }while(testPath.length == 0 && attempts < MAX_ATTEMPTS); // Powtarza aż będzie ścieżka
    
    if (attempts == MAX_ATTEMPTS) {
        std::cout << "Nie udalo sie wygenerowac mapy z dostepna sciezka po " << MAX_ATTEMPTS << " probach.\n";
        return 1;
    }
    
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

  
    bfsPath.free();
    aStarPath.free();

    // Grid posprząta się sam w destruktorze
    return 0;
}