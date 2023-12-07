#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

enum class Type {
    Five,
    Four,
    Full,
    Three,
    Two,
    One,
    High
};

struct Hand {
    std::string set;
    int bid;
    Type type;
};

struct Count {
    char c;
    int num;
};

Type DetermineType(std::string &set) {
    std::vector<Count> chars;
    for (char c : set) {
        std::vector<Count>::iterator it = std::find_if(chars.begin(), chars.end(), [&c](Count &count) {
            return count.c == c;
        });
        if (it != chars.end()) {
            (*it).num++;
            continue;
        }
        chars.push_back({c, 1});
    }

    std::sort(chars.begin(), chars.end(), [=](auto &a, auto &b) {
        return a.num > b.num;
    });

    for (auto &c : chars) {
        if (c.num == 5) {
            return Type::Five;
        } else if (c.num == 4) {
            return Type::Four;
        } else if (c.num == 3) {
            for (auto &c2 : chars) {
                if (c2.num == 2) {
                    return Type::Full;
                }
            }
            return Type::Three;
        } else if (c.num == 2) {
            for (auto &c2 : chars) {
                if (c2.c != c.c && c2.num == 2) {
                    return Type::Two;
                }
            }
            return Type::One;
        }
    }
    return Type::High;
}

int main() {
    std::map<char, int> rankings;
    rankings['A'] = 0;
    rankings['K'] = 1;
    rankings['Q'] = 2;
    rankings['J'] = 3;
    rankings['T'] = 4;
    rankings['9'] = 5;
    rankings['8'] = 6;
    rankings['7'] = 7;
    rankings['6'] = 8;
    rankings['5'] = 9;
    rankings['4'] = 10;
    rankings['3'] = 11;
    rankings['2'] = 12;

    std::vector<Hand> hands;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        std::stringstream ss(line);
        std::string set;
        int bid;
        ss >> set >> bid;
        hands.push_back({set, bid, DetermineType(set)});
    }

    std::sort(hands.begin(), hands.end(), [&rankings](auto &a, auto &b) {
        if (a.type == b.type) {
            for (int i = 0; i < a.set.length(); i++) {
                int rank_a = rankings.at(a.set[i]);
                int rank_b = rankings.at(b.set[i]);
                if (rank_a < rank_b) {
                    return true;
                } else if (rank_a > rank_b) {
                    return false;
                }
            }
        }
        return a.type < b.type;
    });

    int sum = std::accumulate(hands.begin(), hands.end(), 0, [&hands](int sum, Hand &hand) {
        std::vector<Hand>::iterator it = std::find_if(hands.begin(), hands.end(), [&hand](Hand &h) {
            return &hand == &h;
        });
        int rank = hands.size() - (it - hands.begin());
        return sum + rank * hand.bid;
    });

    std::cout << sum << std::endl;
}