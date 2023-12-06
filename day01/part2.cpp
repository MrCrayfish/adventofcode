#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> g_Words = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

bool IsNumber(char c) {
    return c >= '0' && c <= '9';
}

int GetCalibrationValue(std::string &value) {
    size_t length = value.length();
    size_t bounds[] = {length, 0};
    int numbers[2];

    // Find lowest number char
    for (int i = 0; i < length; i++) {
        if (IsNumber(value[i])) {
            numbers[0] = (value[i] - '0');
            bounds[0] = i;
            break;
        }
    }

    // Find highest number char
    for (int i = length - 1; i >= 0; i--) {
        if (IsNumber(value[i])) {
            numbers[1] = (value[i] - '0');
            bounds[1] = i;
            break;
        }
    }

    // Find number words in string and update if outside bounds
    for (int i = 0; i < g_Words.size(); i++) {
        auto pos = value.find(g_Words[i]);
        while (pos != std::string::npos) {
            if (pos < bounds[0]) {
                numbers[0] = i;
                bounds[0] = pos;
            } else if (pos > bounds[1]) {
                numbers[1] = i;
                bounds[1] = pos;
            }
            pos = value.find(g_Words[i], pos + 1);
        }
    }

    // Finally convert chars to int
    std::stringstream ss;
    ss << numbers[0] << numbers[1];
    int number;
    ss >> number;
    return number;
}

main() {
    // Reads in input
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
    for (std::string &value : values) {
        sum += GetCalibrationValue(value);
    }

    std::cout << sum << std::endl;
}