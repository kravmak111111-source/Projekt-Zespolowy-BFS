#ifndef GRID_H
#define GRID_H

#include <iostream>

struct Point {
    int x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point& other) const { return !(*this == other); }
};

// Struktura do zwracania ścieżki (zamiast vectora)
struct PathResult {
    Point* points; // Tablica punktów
    int length;    // Ile ich jest

    // Konstruktor domyślny
    PathResult() : points(nullptr), length(0) {}
    
    // Metoda do czyszczenia pamięci po użyciu
    void free() {
        if (points != nullptr) {
            delete[] points;
            points = nullptr;
            length = 0;
        }
    }
};

class Grid {
public:
    int width;
    int height;
    int** cells; // Tablica 2D (wskaźnik na wskaźniki)

    Grid(int w, int h) : width(w), height(h) {
        // Alokacja tablicy wierszy
        cells = new int*[height];
        for (int i = 0; i < height; ++i) {
            // Alokacja kolumn dla każdego wiersza i zerowanie
            cells[i] = new int[width];
            for (int j = 0; j < width; ++j) {
                cells[i][j] = 0;
            }
        }
    }

    // WAŻNE: Destruktor, żeby posprzątać pamięć (nie mamy vectora, który robi to sam)
    ~Grid() {
        for (int i = 0; i < height; ++i) {
            delete[] cells[i];
        }
        delete[] cells;
    }

    void addObstacle(int x, int y) {
        if (isValid(x, y)) cells[y][x] = 1;
    }

    bool isWalkable(int x, int y) const {
        return isValid(x, y) && cells[y][x] != 1;
    }

    bool isValid(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    void print(Point* path = nullptr, int pathLength = 0) const {
        // Rysowanie mapy - musimy stworzyć bufor znaków ręcznie
        char** display = new char*[height];
        for(int i=0; i<height; i++) {
            display[i] = new char[width];
            for(int j=0; j<width; j++) {
                display[i][j] = (cells[i][j] == 1) ? '#' : '.';
            }
        }

        // Nanoszenie ścieżki
        if (path != nullptr) {
            for(int i = 0; i < pathLength; i++) {
                Point p = path[i];
                if(isValid(p.x, p.y)) display[p.y][p.x] = '*';
            }
        }

        // Wypisywanie
        std::cout << "  ";
        for(int x=0; x<width; x++) std::cout << x % 10;
        std::cout << "\n";
        for(int y=0; y<height; y++) {
            std::cout << y % 10 << " ";
            for(int x=0; x<width; x++) std::cout << display[y][x];
            std::cout << "\n";
        }

        // Sprzątanie po rysowaniu
        for(int i=0; i<height; i++) delete[] display[i];
        delete[] display;
    }
};

#endif