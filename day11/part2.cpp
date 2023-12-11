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

class Galaxy {
public:
    int x, y;

    Galaxy() {}

    Galaxy(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator<(const Galaxy &other) const {
        return y == other.y ? x < other.x : y < other.y;
    }

    bool operator==(const Galaxy &other) const {
        return x == other.x && y == other.y;
    }
};

class Path {
public:
    Galaxy a;
    Galaxy b;
    int distance;

    Path(Galaxy &a, Galaxy &b) {
        this->a = std::min<Galaxy>(a, b);
        this->b = std::max<Galaxy>(a, b);
        this->distance = std::abs(a.x - b.x) + std::abs(a.y - b.y);
    }

    bool operator==(const Path &other) const {
        return this->a == other.a && this->b == other.b;
    }

    class Hash {
    public:
        size_t operator()(const Path &path) const noexcept {
            size_t h1 = std::hash<int>{}(path.a.x);
            size_t h2 = std::hash<int>{}(path.a.y);
            size_t h3 = std::hash<int>{}(path.b.x);
            size_t h4 = std::hash<int>{}(path.b.y);
            return ((h1 ^ (h2 << 1)) ^ (h3 << 1)) ^ (h4 << 1);
        }
    };
};

main() {
    std::vector<Galaxy> galaxies;
    std::unordered_set<Path, Path::Hash> paths;
    std::unordered_set<int> occupied_columns;
    std::unordered_set<int> occupied_rows;

    int y = 0;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        int x = 0;
        for (char &c : line) {
            if (c == '#') {
                Galaxy galaxy(x, y);
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
            paths.insert(path);
        }
    }

    size_t sum = 0;
    for (auto &p : paths) {
        sum += p.distance;
        int min_x = std::min<int>(p.a.x, p.b.x);
        int max_x = std::max<int>(p.a.x, p.b.x);
        for (int x = min_x + 1; x < max_x; x++) {
            if (std::find(occupied_columns.begin(), occupied_columns.end(), x) == occupied_columns.end()) {
                sum += 999999;
            }
        }
        int min_y = std::min<int>(p.a.y, p.b.y);
        int max_y = std::max<int>(p.a.y, p.b.y);
        for (int y = min_y + 1; y < max_y; y++) {
            if (std::find(occupied_rows.begin(), occupied_rows.end(), y) == occupied_rows.end()) {
                sum += 999999;
            }
        }
    }

    std::cout << sum << std::endl;
}