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
};

bool IsSymbol(char c) {
    return c != '.' && (c < '0' || c > '9');
}

bool FindSymbol(std::vector<std::string> &schematics, PartNumber &part) {
    for (int j = part.line - 1; j < part.line + 2; j++) {
        if (j < 0 || j >= schematics.size())
            continue;
        for (int i = part.start - 1; i <= part.end + 1; i++) {
            if (i < 0 || i >= schematics[j].length())
                continue;
            if (IsSymbol(schematics[j][i])) {
                return true;
            }
        }
    }
    return false;
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

    int sum = std::accumulate(parts.begin(), parts.end(), 0, [&schematics](int sum, PartNumber &part) {
        if (FindSymbol(schematics, part)) {
            return sum + std::stoi(schematics[part.line].substr(part.start, part.end + 1));
        }
        return sum;
    });

    std::cout << sum << std::endl;
}