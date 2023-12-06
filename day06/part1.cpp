#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::vector<int> times;
    std::vector<int> records;

    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        std::vector<int> *v = line.find("Time:") != std::string::npos ? &times : &records;
        line = line.substr(line.find(":") + 1, line.length());
        std::cout << line << std::endl;
        std::stringstream ss(line);
        int val;
        while (ss >> val) {
            v->push_back(val);
        }
    }

    std::vector<int> wins;
    for (int i = 0; i < times.size(); i++) {
        int win_count = 0;
        int time = times[i];
        for (int hold = 0; hold <= time; hold++) {
            int remaining_time = time - hold;
            int distance = hold * remaining_time;
            if (distance > records[i]) {
                win_count++;
            }
        }
        wins.push_back(win_count);
    }

    int sum = std::accumulate(wins.begin(), wins.end(), 0, [](int sum, int &win_count) {
        return sum == 0 ? win_count : sum * win_count;
    });

    std::cout << sum << std::endl;
}