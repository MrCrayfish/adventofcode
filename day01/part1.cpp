#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

bool IsNumber(char c) {
    return c >= '0' && c <= '9';
}

main() {
    std::vector<std::string> values;
    std::string line;
    std::ifstream input("input.txt");
    if (input.is_open()) {
        while (std::getline(input, line)) {
            std::string my_value(line);
            values.push_back(my_value);
        }
        input.close();
    }

    int sum = 0;
    for (auto &&value : values) {
        std::stringstream ss;
        std::all_of(value.begin(), value.end(), [&ss](char &c) {
            if (!IsNumber(c))
                return true;
            ss << c;
            return false;
        });
        std::all_of(value.rbegin(), value.rend(), [&ss](char &c) {
            if (!IsNumber(c))
                return true;
            ss << c;
            return false;
        });
        int number;
        ss >> number;
        sum += number;
    }

    std::cout << sum << std::endl;
}