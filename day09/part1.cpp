#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct Report {
    std::vector<std::vector<int>> sequences;
};

void GenerateSequence(std::vector<std::vector<int>> &sequences, const std::vector<int> &input) {
    std::vector<int> new_sequence;
    for (int i = 0; i < input.size() - 1; i++) {
        int a = input[i];
        int b = input[i + 1];
        int d = b - a;
        new_sequence.push_back(d);
    }
    sequences.push_back(new_sequence);
    for (int &val : new_sequence) {
        if (val != 0) {
            GenerateSequence(sequences, new_sequence);
            return;
        }
    }
}

void GeneratePredictions(std::vector<std::vector<int>> &sequences) {
    sequences[sequences.size() - 1].push_back(0);
    for (int i = sequences.size() - 2; i >= 0; i--) {
        int a = sequences[i].back();
        int b = sequences[i + 1].back();
        int p = a + b;
        sequences[i].push_back(p);
    }
}

int main() {
    std::vector<Report> reports;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        std::vector<int> history;
        std::stringstream ss(line);
        int val;
        while (ss >> val) {
            history.push_back(val);
        }

        Report report;
        report.sequences.push_back(history);
        reports.push_back(report);
    }

    int sum = 0;
    for (Report &report : reports) {
        GenerateSequence(report.sequences, report.sequences[0]);
        GeneratePredictions(report.sequences);
        sum += report.sequences[0].back();
    }

    std::cout << sum << std::endl;
}