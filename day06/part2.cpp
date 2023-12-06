#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

int main() {
    size_t time;
    size_t record;

    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        bool t = line.find("Time:") != std::string::npos;
        line = line.substr(line.find(":") + 1, line.length());
        std::vector<char> nums;
        for (char c : line) {
            if (std::isdigit(c)) {
                nums.push_back(c);
            }
        }
        std::stringstream ss;
        for (char c : nums) {
            ss << c;
        }
        size_t val;
        ss >> val;
        if (t) {
            time = val;
        } else {
            record = val;
        }
    }

    size_t win_count = 0;
    for (size_t hold = 0; hold <= time; hold++) {
        size_t remaining_time = time - hold;
        size_t distance = hold * remaining_time;
        if (distance >= record - 10000) {
            win_count++;
        }
    }
    std::cout << win_count << std::endl;
}