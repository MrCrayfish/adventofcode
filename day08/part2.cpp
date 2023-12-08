#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

struct Node {
    std::string left;
    std::string right;
};

struct Input {
    std::string current;
};

bool IsOne(int &val) {
    return val == 1;
}

int main() {
    std::string line;
    std::ifstream input("input.txt");

    // Read input
    std::vector<bool> moves;
    std::getline(input, line);
    std::string navigate = line;
    for (char &c : navigate) {
        moves.push_back(c == 'R');
    }

    // Skip the blank line
    std::getline(input, line);

    std::vector<Input> starting_nodes;
    std::map<std::string, Node> nodes;
    while (std::getline(input, line)) {
        std::string start = line.substr(0, 3);
        std::string left = line.substr(7, 3);
        std::string right = line.substr(12, 3);
        nodes[start] = {left, right};
        if (start[2] == 'A') {
            starting_nodes.push_back({start});
        }
    }

    std::vector<int> multiples;
    for (auto &n : starting_nodes) {
        size_t count = 0;
        int move_index = 0;
        while (true) {
            count++;
            bool move = moves[move_index];
            move_index = (move_index + 1) % moves.size();
            n.current = move ? nodes[n.current].right : nodes[n.current].left;
            if (n.current[2] == 'Z') {
                multiples.push_back(count);
                break;
            }
        }
    }

    std::vector<int> primes;
    while (true) {
        for (size_t num = 2; num < 10000000; num++) {
            for (size_t prime = 2; prime <= num; prime++) {
                if (num % prime == 0) {
                    bool once = false;
                    for (int i = 0; i < multiples.size(); i++) {
                        size_t val = multiples[i];
                        if (val % num == 0) {
                            multiples[i] = val / num;
                            if (!once) {
                                primes.push_back(num);
                                once = true;
                            }
                        }
                    }

                    int match = 0;
                    for (auto &m : multiples) {
                        if (m != 1) {
                            break;
                        }
                        match++;
                    }

                    if (match == multiples.size()) {
                        size_t sum = 1;
                        for (int &p : primes) {
                            sum *= p;
                        }
                        std::cout << sum << std::endl;
                        return 0;
                    }
                }
            }
        }
    }
}