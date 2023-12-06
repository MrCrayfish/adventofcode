#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

struct Game {
    int num;
    int red = 0;
    int green = 0;
    int blue = 0;
};

bool IsNumber(char c) {
    return c >= '0' && c <= '9';
}

main() {
    std::vector<Game> games;
    unsigned int game_count = 1;

    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        Game game;
        game.num = game_count;

        int count = 0;
        std::string s;
        std::string data = line.substr(line.find(':') + 2, line.length());
        std::stringstream ss(data);
        while (ss >> s) {
            if (IsNumber(s[0])) {
                count = std::stoi(s);
            } else if (s[0] == 'r') {
                game.red = std::max<int>(game.red, count);
            } else if (s[0] == 'g') {
                game.green = std::max<int>(game.green, count);
            } else if (s[0] == 'b') {
                game.blue = std::max<int>(game.blue, count);
            }
        }

        games.push_back(game);
        game_count++;
    }
    input.close();

    int sum = std::accumulate(games.begin(), games.end(), 0, [](int sum, Game &game) {
        if(game.red <= 12 && game.green <= 13 && game.blue <= 14) {
            return sum + game.num;
        }
        return sum;
    });

    std::cout << sum << std::endl;
}