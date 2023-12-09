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

void GenerateSequence(Report &report, const std::vector<int> &input) {
    std::vector<int> new_sequence;
    for (int i = 0; i < input.size() - 1; i++) {
        int a = input[i];
        int b = input[i + 1];
        int d = b - a;
        new_sequence.push_back(d);
    }
    report.sequences.push_back(new_sequence);
    for (int &val : new_sequence) {
        if (val != 0) {
            GenerateSequence(report, new_sequence);
            return;
        }
    }
}

void GeneratePredictions(Report &report) {
    std::vector<std::vector<int>> *seq = &report.sequences;
    (*seq)[seq->size() - 1].insert((*seq)[seq->size() - 1].begin(), 0);
    for (int i = seq->size() - 2; i >= 0; i--) {
        int a = (*seq)[i].front();
        int b = (*seq)[i + 1].front();
        int p = a - b;
        (*seq)[i].insert((*seq)[i].begin(), p);
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
        GenerateSequence(report, report.sequences[0]);
        GeneratePredictions(report);
        sum += report.sequences[0].front();
    }

    std::cout << sum << std::endl;
}