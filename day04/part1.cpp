#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    int total = 0;
    std::vector<std::string> pending_cards;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        bool captured_winning = false;
        int card_value = 0;
        std::vector<int> winning_numbers;
        std::string numbers = line.substr(line.find(':') + 2, line.length());
        std::stringstream ss(numbers);
        std::string input;
        while(ss >> input) {
            if(std::isdigit(input[0])) {
                if(!captured_winning) {
                    winning_numbers.push_back(std::stoi(input));
                } else {
                    int num = std::stoi(input);
                    if(std::find(winning_numbers.begin(), winning_numbers.end(), num) != winning_numbers.end()) {
                        card_value = card_value > 0 ? card_value * 2 : 1; 
                    }
                }
            } else if(input[0] == '|') {
                captured_winning = true;
            }
        }
        total += card_value;
    }
    std::cout << total << std::endl;
}