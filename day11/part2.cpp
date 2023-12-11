#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct Galaxy {
    int x, y;
};

class Path {
public:
    Galaxy *a;
    Galaxy *b;
    int distance;

    Path(Galaxy &a, Galaxy &b) {
        this->a = &a;
        this->b = &b;
        this->distance = std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }
};

main() {
    std::vector<Galaxy> galaxies;
    std::vector<Path> paths;
    std::unordered_set<int> occupied_columns;
    std::unordered_set<int> occupied_rows;

    int y = 0;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        int x = 0;
        for (char &c : line) {
            if (c == '#') {
                Galaxy galaxy{x, y};
                galaxies.push_back(galaxy);
                occupied_columns.insert(x);
                occupied_rows.insert(y);
            }
            x++;
        }
        y++;
    }

    for (int i = 0; i < galaxies.size(); i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            Path path(galaxies[i], galaxies[j]);
            paths.push_back(path);
        }
    }

    int empty_space_size = 1000000;
    size_t sum = 0;
    for (auto &p : paths) {
        sum += p.distance;
        int min_x = std::min<int>(p.a->x, p.b->x);
        int max_x = std::max<int>(p.a->x, p.b->x);
        for (int x = min_x + 1; x < max_x; x++) {
            if (occupied_columns.find(x) == occupied_columns.end()) {
                sum += empty_space_size - 1;
            }
        }
        int min_y = std::min<int>(p.a->y, p.b->y);
        int max_y = std::max<int>(p.a->y, p.b->y);
        for (int y = min_y + 1; y < max_y; y++) {
            if (occupied_rows.find(y) == occupied_rows.end()) {
                sum += empty_space_size - 1;
            }
        }
    }

    std::cout << sum << std::endl;
}