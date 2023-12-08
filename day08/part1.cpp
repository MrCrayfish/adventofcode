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

    std::map<std::string, Node> nodes;
    while (std::getline(input, line)) {
        std::string start = line.substr(0, 3);
        std::string left = line.substr(7, 3);
        std::string right = line.substr(12, 3);
        nodes[start] = {left, right};
    }

    size_t count = 0;
    int move_index = 0;
    std::string current = "AAA";
    while (current != "ZZZ") {
        bool move = moves[move_index % moves.size()];
        move_index++;
        Node node = nodes[current];
        current = move ? node.right : node.left;
        count++;
    }
    std::cout << count << std::endl;
}