#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct PartNumber {
    int line;
    int start;
    int end;
    bool valid;
    int sym_line = -1;
    int sym_index = -1;
};

bool IsSymbol(char c) {
    return c != '.' && (c < '0' || c > '9');
}

void FindSymbol(std::vector<std::string> &schematics, PartNumber &part) {
    for (int j = part.line - 1; j < part.line + 2; j++) {
        if (j < 0 || j >= schematics.size())
            continue;
        for (int i = part.start - 1; i <= part.end + 1; i++) {
            if (i < 0 || i >= schematics[j].length())
                continue;
            char c = schematics[j][i];
            if (IsSymbol(c)) {
                part.sym_line = j;
                part.sym_index = i;
                return;
            }
        }
    }
}

int CreateNumber(PartNumber &part) {
}

int main() {
    std::vector<std::string> schematics;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        schematics.push_back(line);
    }

    std::vector<PartNumber> parts;
    PartNumber currentPart;
    for (int i = 0; i < schematics.size(); i++) {
        bool reading = false;
        std::string data = schematics[i];
        std::stringstream ss;
        for (int j = 0; j < data.length(); j++) {
            if (data[j] >= '0' && data[j] <= '9') {
                if (!reading) {
                    currentPart = PartNumber{i, j};
                    reading = true;
                }
            } else if (reading) {
                currentPart.end = j - 1;
                parts.push_back(currentPart);
                reading = false;
            }
        }
        if (reading) {
            currentPart.end = data.length() - 1;
            parts.push_back(currentPart);
        }
    }

    for (auto &&part : parts) {
        FindSymbol(schematics, part);
    }

    // this got messy :(
    int sum = 0;
    for (int i = 0; i < parts.size(); i++) {
        for (int j = i + 1; j < parts.size(); j++) {
            if (parts[i].sym_line == -1 || parts[j].sym_line == -1)
                continue;
            if (std::abs(parts[i].line - parts[j].line) > 2)
                continue;
            if (parts[i].sym_line == parts[j].sym_line && parts[i].sym_index == parts[j].sym_index) {
                int first = std::stoi(schematics[parts[i].line].substr(parts[i].start, parts[i].end + 1));
                int second = std::stoi(schematics[parts[j].line].substr(parts[j].start, parts[j].end + 1));
                sum += first * second;
                std::cout << first << " " << second << std::endl;
            }
        }
    }

    std::cout << sum << std::endl;
}