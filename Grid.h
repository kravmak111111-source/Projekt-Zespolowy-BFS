#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <cstdlib>  
#include <ctime>
#include <random>
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

        // Dodawanie trudnego terenu (np. błoto, woda)
        // Ustawiamy wartość np. 5 (koszt przejścia 5 razy większy niż zwykle)
        void addMud(int x, int y, int cost) {
            if (isValid(x, y) && cells[y][x] != 1) { 
                cells[y][x] = cost; 
            }
        }

        // Pobieranie kosztu pola
        int getCost(int x, int y) const {
            if (!isValid(x, y)) return 9999;
            if (cells[y][x] == 1) return 9999; // Ściana
            if (cells[y][x] == 0) return 1;    // Zwykłe pole ma koszt 1
            return cells[y][x];                // Błoto zwraca swoją wagę (np. 5)
        }

        bool isWalkable(int x, int y) const {
            return isValid(x, y) && cells[y][x] != 1;
        }

        bool isValid(int x, int y) const {
            return x >= 0 && x < width && y >= 0 && y < height;
        }

        void print(Point* path = nullptr, int pathLength = 0, char pathSymbol = '*') const {
        // Alokacja bufora (bez zmian)
        char** display = new char*[height];
        for(int i=0; i<height; i++) {
            display[i] = new char[width];
            for(int j=0; j<width; j++) {
                if (cells[i][j] == 1) display[i][j] = '#';      // Ściana
                else if (cells[i][j] > 1) display[i][j] = '~';  // Błoto
                else display[i][j] = '.';                       // Trawa
            }
        }

        // Nanoszenie ścieżki z wybranym symbolem
        if (path != nullptr && pathLength > 0) {
            for(int i = 0; i < pathLength; i++) {
                Point p = path[i];
                if(isValid(p.x, p.y)) {
                    display[p.y][p.x] = pathSymbol; // Używamy znaku przekazanego w argumencie
                }
            }
            
            // Opcjonalnie: Nadpisz Start i Koniec, żeby były super widoczne
            Point start = path[0];
            Point end = path[pathLength - 1];
            if(isValid(start.x, start.y)) display[start.y][start.x] = 'S'; // Start
            if(isValid(end.x, end.y))     display[end.y][end.x]   = 'M'; // Meta
        }

        // Wypisywanie (bez zmian)
        std::cout << "  ";
        for(int x=0; x<width; x++) std::cout << x % 10 << " ";
        std::cout << "\n";
        for(int y=0; y<height; y++) {
            std::cout << y % 10 << " ";
            for(int x=0; x<width; x++) std::cout << display[y][x] << " ";
            std::cout << "\n";
        }

        // Sprzątanie (bez zmian)
        for(int i=0; i<height; i++) delete[] display[i];
        delete[] display;
    }
    
    void generateObstacles(int wallCount, int width, int height, Point start, Point goal) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> xDist(0, width - 1);
    std::uniform_int_distribution<int> yDist(0, height - 1);
    std::uniform_int_distribution<int> lenDist(2, 6);   // krótkie ściany
    std::uniform_int_distribution<int> dirDist(0, 1);   // 0 = poziom, 1 = pion

    for (int w = 0; w < wallCount; w++) {
        int x = xDist(rng);
        int y = yDist(rng);
        int len = lenDist(rng);
        bool vertical = dirDist(rng);

        for (int i = 0; i < len; i++) {
            int nx = x + (vertical ? 0 : i);
            int ny = y + (vertical ? i : 0);


            if ((nx == start.x && ny == start.y) ||
                (nx == goal.x  && ny == goal.y))
                continue;
            if (rand() % 4 != 0) // 25% szans na "dziurę"
                addObstacle(nx, ny);
            }




     }
}

};

#endif