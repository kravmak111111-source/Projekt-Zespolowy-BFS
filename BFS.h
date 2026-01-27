#ifndef BFS_H
#define BFS_H

#include "Grid.h"
#include "Stats.h"
#include <queue> // Zakładam, że queue jest dozwolone. Jeśli nie, pisz.

class BFS {
public:
    static PathResult findPath(const Grid& grid, Point start, Point goal, AlgoStats* stats = nullptr) {
        std::queue<Point> q;
        q.push(start);

        int visitedCount = 0;

        // Alokacja tablicy odwiedzonych (visited) i rodziców (parent)
        bool** visited = new bool*[grid.height];
        Point** parent = new Point*[grid.height];
        
        for(int i=0; i<grid.height; i++) {
            visited[i] = new bool[grid.width];
            parent[i] = new Point[grid.width];
            for(int j=0; j<grid.width; j++) {
                visited[i][j] = false;
                parent[i][j] = {-1, -1}; // Oznaczenie braku rodzica
            }
        }

        visited[start.y][start.x] = true;
        visitedCount++;
        
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};
        bool found = false;

        while (!q.empty()) {
            Point current = q.front();
            q.pop();

            if (current == goal) {
                found = true;
                break;
            }

            for (int i = 0; i < 4; i++) {
                int nx = current.x + dx[i];
                int ny = current.y + dy[i];

                if (grid.isWalkable(nx, ny) && !visited[ny][nx]) {
                    visited[ny][nx] = true;
                    visitedCount++;
                    parent[ny][nx] = current;
                    q.push({nx, ny});
                }
            }
        }

        PathResult result;
        
        if (found) {
            // Krok 1: Policz długość ścieżki
            int count = 0;
            Point step = goal;
            while (step != start) {
                count++;
                step = parent[step.y][step.x];
            }
            count++; // Dodajemy start

            // Krok 2: Zaalokuj tablicę wynikową
            result.points = new Point[count];
            result.length = count;

            // Krok 3: Wypełnij tablicę (idąc od tyłu)
            step = goal;
            int index = count - 1; // Wpisujemy od końca, żeby ścieżka była poprawna
            while (step != start) {
                result.points[index--] = step;
                step = parent[step.y][step.x];
            }
            result.points[0] = start;
        }

        // Sprzątanie pamięci pomocniczej (bardzo ważne przy braku vectora!)
        for(int i=0; i<grid.height; i++) {
            delete[] visited[i];
            delete[] parent[i];
        }
        delete[] visited;
        delete[] parent;
        if (stats) {
            stats->visited = visitedCount;
        }   

        
        return result;
    }
};

#endif