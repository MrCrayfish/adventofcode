#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

typedef int Card;
typedef std::vector<Card> CardList;

void ProcessCards(const CardList &cards, Card *start, Card *end, int &total) {
    Card *copy = start;
    while (copy != end) {
        int wins = *copy;
        total += wins;
        if (wins > 0) {
            ProcessCards(cards, copy + 1, copy + 1 + wins, total);
        }
        copy++;
    }
}

int main() {
    int card_num = 1;
    CardList pending_cards;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        bool captured_winning = false;
        int win_count = 0;
        std::vector<int> winning_numbers;
        std::string numbers = line.substr(line.find(':') + 2, line.length());
        std::stringstream ss(numbers);
        std::string input;
        while (ss >> input) {
            if (std::isdigit(input[0])) {
                if (!captured_winning) {
                    winning_numbers.push_back(std::stoi(input));
                } else {
                    int num = std::stoi(input);
                    if (std::find(winning_numbers.begin(), winning_numbers.end(), num) != winning_numbers.end()) {
                        win_count++;
                    }
                }
            } else if (input[0] == '|') {
                captured_winning = true;
            }
        }
        pending_cards.push_back(win_count);
        card_num++;
    }

    int total = pending_cards.size();
    Card *start = &pending_cards.front();
    Card *end = &pending_cards.back();
    ProcessCards(pending_cards, start, end, total);
    std::cout << total << std::endl;
}