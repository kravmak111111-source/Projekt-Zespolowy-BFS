#ifndef ASTAR_H
#define ASTAR_H

#include "Grid.h"
#include "Stats.h"
#include <climits>  // INT_MAX
#include <cstdlib>  // abs



class AStar {
private:
    struct Node {
        Point p;
        int f; // g + h
        int g; // koszt od startu
    };

    // Własny Min-Heap bez vectorów
    class MinHeap {
    private:
        Node* arr;
        int sz;
        int cap;

        static bool less(const Node& a, const Node& b) {
            // Najpierw mniejsze f, przy remisie mniejsze g (opcjonalnie stabilniej)
            if (a.f != b.f) return a.f < b.f;
            return a.g < b.g;
        }

        void swap(Node& a, Node& b) {
            Node tmp = a;
            a = b;
            b = tmp;
        }

        void grow() {
            int newCap = (cap == 0) ? 64 : cap * 2;
            Node* nArr = new Node[newCap];
            for (int i = 0; i < sz; i++) nArr[i] = arr[i];
            delete[] arr;
            arr = nArr;
            cap = newCap;
        }

        void siftUp(int i) {
            while (i > 0) {
                int p = (i - 1) / 2;
                if (less(arr[p], arr[i])) break;
                swap(arr[p], arr[i]);
                i = p;
            }
        }

        void siftDown(int i) {
            while (true) {
                int l = 2 * i + 1;
                int r = 2 * i + 2;
                int smallest = i;

                if (l < sz && less(arr[l], arr[smallest])) smallest = l;
                if (r < sz && less(arr[r], arr[smallest])) smallest = r;

                if (smallest == i) break;
                swap(arr[i], arr[smallest]);
                i = smallest;
            }
        }

    public:
        MinHeap() : arr(nullptr), sz(0), cap(0) {}
        ~MinHeap() { delete[] arr; }

        bool empty() const { return sz == 0; }

        void push(const Node& n) {
            if (sz == cap) grow();
            arr[sz] = n;
            siftUp(sz);
            sz++;
        }

        Node popMin() {
            Node out = arr[0];
            sz--;
            if (sz > 0) {
                arr[0] = arr[sz];
                siftDown(0);
            }
            return out;
        }
    };

    static int manhattan(Point a, Point b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

public:
    static PathResult findPath(const Grid& grid, Point start, Point goal, AlgoStats* stats = nullptr) {
        PathResult result; // points=nullptr, length=0 (konstruktor z Grid.h)

        // Edge case
        if (start == goal) {
            result.points = new Point[1];
            result.points[0] = start;
            result.length = 1;
            return result;
        }

        // Tablice pomocnicze
        int** gScore = new int*[grid.height];
        Point** parent = new Point*[grid.height];
        bool** closed = new bool*[grid.height];

        for (int y = 0; y < grid.height; y++) {
            gScore[y] = new int[grid.width];
            parent[y] = new Point[grid.width];
            closed[y] = new bool[grid.width];

            for (int x = 0; x < grid.width; x++) {
                gScore[y][x] = INT_MAX;
                parent[y][x] = {-1, -1};
                closed[y][x] = false;
            }
        }

        MinHeap open;

        gScore[start.y][start.x] = 0;
        int h0 = manhattan(start, goal);
        open.push({start, h0, 0});
        if (stats) stats->pushed++;

        int dx[4] = {0, 0, -1, 1};
        int dy[4] = {-1, 1, 0, 0};

        bool found = false;

        while (!open.empty()) {
            Node cur = open.popMin();
            if (stats) stats->visited++;

            Point current = cur.p;

            // Jeśli to "stary" wpis (gorszy niż obecny gScore) – pomijamy
            if (cur.g != gScore[current.y][current.x]) continue;

            if (closed[current.y][current.x]) continue;
            closed[current.y][current.x] = true;

            if (current == goal) {
                found = true;
                break;
            }

            for (int i = 0; i < 4; i++) {
                int nx = current.x + dx[i];
                int ny = current.y + dy[i];

                if (!grid.isWalkable(nx, ny)) continue;
                if (closed[ny][nx]) continue;

                int movementCost = grid.getCost(nx, ny);
                int tentativeG = gScore[current.y][current.x] + movementCost;

                if (tentativeG < gScore[ny][nx]) {
                    gScore[ny][nx] = tentativeG;
                    parent[ny][nx] = current;

                    int f = tentativeG + manhattan({nx, ny}, goal);
                    open.push({{nx, ny}, f, tentativeG});
                    if (stats) stats->pushed++;
                }
            }
        }

        // Odtwarzanie ścieżki
        if (found) {
            int count = 0;
            Point step = goal;
            while (step != start) {
                count++;
                step = parent[step.y][step.x];
            }
            count++; // + start

            result.points = new Point[count];
            result.length = count;

            step = goal;
            int idx = count - 1;
            while (step != start) {
                result.points[idx--] = step;
                step = parent[step.y][step.x];
            }
            result.points[0] = start;
        }

        // Sprzątanie
        for (int y = 0; y < grid.height; y++) {
            delete[] gScore[y];
            delete[] parent[y];
            delete[] closed[y];
        }
        delete[] gScore;
        delete[] parent;
        delete[] closed;

        return result;
    }
};

#endif
