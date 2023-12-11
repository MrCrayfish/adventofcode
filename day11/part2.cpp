#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

struct Galaxy {
    int x, y;
};

main() {
    std::vector<Galaxy> galaxies;
    std::set<int> galaxy_columns;
    std::set<int> galaxy_rows;

    int y = 0;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        int x = 0;
        for (char &c : line) {
            if (c == '#') {
                galaxies.push_back({x, y});
                galaxy_columns.insert(x);
                galaxy_rows.insert(y);
            }
            x++;
        }
        y++;
    }

    int empty_space_size = 1000000 - 1;
    size_t sum = 0;
    for (int i = 0; i < galaxies.size(); i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            Galaxy *a = &galaxies[i];
            Galaxy *b = &galaxies[j];

            // Add the manhatten distance
            sum += std::abs(a->x - b->x) + std::abs(a->y - b->y);

            // Find any empty columns and add the aditional distance
            int min_x = std::min<int>(a->x, b->x);
            int max_x = std::max<int>(a->x, b->x);
            for (int x = min_x + 1; x < max_x; x++) {
                if (galaxy_columns.find(x) == galaxy_columns.end()) {
                    sum += empty_space_size;
                }
            }

            // Find any empty rows and add the aditional distance
            int min_y = std::min<int>(a->y, b->y);
            int max_y = std::max<int>(a->y, b->y);
            for (int y = min_y + 1; y < max_y; y++) {
                if (galaxy_rows.find(y) == galaxy_rows.end()) {
                    sum += empty_space_size;
                }
            }
        }
    }

    std::cout << sum << std::endl;
}