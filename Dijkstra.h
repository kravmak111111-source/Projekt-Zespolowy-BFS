#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Grid.h"
#include "Stats.h"
#include <queue>
#include <limits>

class Dijkstra {
public:
    static PathResult findPath(const Grid& grid, Point start, Point goal, AlgoStats* stats = nullptr) {

        // =============================
        // STRUKTURA DO KOLEJKI
        // =============================
        struct Node {
            Point p;
            int cost;

            bool operator>(const Node& other) const {
                return cost > other.cost;
            }
        };

        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
        pq.push({start, 0});
        int visitedCount = 0;

        // =============================
        // TABLICE POMOCNICZE
        // =============================
        int** dist = new int*[grid.height];
        Point** parent = new Point*[grid.height];

        for (int i = 0; i < grid.height; i++) {
            dist[i] = new int[grid.width];
            parent[i] = new Point[grid.width];
            for (int j = 0; j < grid.width; j++) {
                dist[i][j] = std::numeric_limits<int>::max();
                parent[i][j] = {-1, -1};
            }
        }

        dist[start.y][start.x] = 0;

        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        bool found = false;

        // =============================
        // GŁÓWNA PĘTLA
        // =============================
        while (!pq.empty()) {
            Node current = pq.top();
            pq.pop();
            visitedCount++;

            Point p = current.p;

            if (p == goal) {
                found = true;
                break;
            }

            // jeśli znaleźliśmy już lepszą drogę
            if (current.cost > dist[p.y][p.x])
                continue;

            for (int i = 0; i < 4; i++) {
                int nx = p.x + dx[i];
                int ny = p.y + dy[i];

                if (!grid.isWalkable(nx, ny))
                    continue;

                int movementCost = grid.getCost(nx, ny);
                int newCost = dist[p.y][p.x] + movementCost;

                if (newCost < dist[ny][nx]) {
                    dist[ny][nx] = newCost;
                    parent[ny][nx] = p;
                    pq.push({{nx, ny}, newCost});
                }
            }
        }

        // =============================
        // BUDOWANIE WYNIKU
        // =============================
        PathResult result;

        if (found) {
            int count = 0;
            Point step = goal;

            while (step != start) {
                count++;
                step = parent[step.y][step.x];
            }
            count++; // start

            result.points = new Point[count];
            result.length = count;

            step = goal;
            int index = count - 1;
            while (step != start) {
                result.points[index--] = step;
                step = parent[step.y][step.x];
            }
            result.points[0] = start;
        } else {
            result.points = nullptr;
            result.length = 0;
        }

        // =============================
        // SPRZĄTANIE
        // =============================
        for (int i = 0; i < grid.height; i++) {
            delete[] dist[i];
            delete[] parent[i];
        }
        delete[] dist;
        delete[] parent;
        if (stats) {
            stats->visited = visitedCount;
        }

        return result;
    }
};

#endif