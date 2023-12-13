#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>

struct Record {
    std::string data;
    std::vector<int> springs;
    int total;
};

void UpdateData(std::string &data, int index, int length) {
    if (index - 1 >= 0) {
        data[index - 1] = '.';
    }
    if (index + length < data.length()) {
        data[index + length] = '.';
    }
    while (length > 0) {
        data[index + length - 1] = '#';
        length--;
    }
}

bool CanPlace(std::string &data, int index, int length) {
    for (int i = index; i < index + length; i++) {
        if (data[i] != '?' && data[i] != '#') {
            return false;
        }
    }

    if (data[index] == '#') {
        if (index - 1 >= 0 && (data[index - 1] != '?' && data[index - 1] != '.')) {
            return false;
        }
        if (index + length < data.size() && (data[index + length] != '?' && data[index + length] != '.')) {
            return false;
        }
    }

    if (index - 1 >= 0 && data[index - 1] == '#')
        return false;

    if (index + length < data.size() && data[index + length] == '#')
        return false;

    return true;
}

int Count(std::string &data) {
    int count = 0;
    for (char &c : data) {
        if (c == '#') {
            count++;
        }
    }
    return count;
}

void Place(Record &r, int s, int start, std::string &data, std::set<std::string> &matches, int placed, int depth) {
    if (s >= r.springs.size()) {
        return;
    }
    int val = r.springs[s];
    for (int j = start; j < data.length(); j++) {
        if (CanPlace(data, j, val)) {
            std::string copy = data;
            UpdateData(copy, j, val);
            if (placed + 1 == r.springs.size() && Count(copy) == r.total) {
                // std::cout << copy << std::endl;
                matches.insert(copy);
                continue;
            }

            Place(r, s + 1, j + val, copy, matches, placed + 1, depth + 1);
        }
    }
}

int main() {
    std::vector<Record> records;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        std::string data = line.substr(0, line.find(' '));
        std::vector<int> springs;
        std::stringstream ss(line.substr(data.length()));
        int total = 0;
        int spring;
        while (std::getline(ss, line, ',')) {
            int val = std::stoi(line);
            springs.push_back(val);
            total += val;
        }
        records.push_back({data, springs, total});
    }

    int arrangements = 0;
    for (Record &r : records) {
        std::set<std::string> matches;
        Place(r, 0, 0, r.data, matches, 0, 0);
        arrangements += matches.size();
    }
    std::cout << arrangements << std::endl;
}