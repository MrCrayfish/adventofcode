#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

class Direction {
public:
    int x, y;

    bool operator==(const Direction &other) const {
        return x == other.x && y == other.y;
    }
};

Direction Left{-1, 0};
Direction Up{0, -1};
Direction Right{1, 0};
Direction Down{0, 1};

Direction &Opposite(Direction &in) {
    if (in == Up)
        return Down;
    if (in == Down)
        return Up;
    if (in == Left)
        return Right;
    return Left;
}

class PipeType {
public:
    Direction a;
    Direction b;

    Direction Out(Direction &in) {
        if (a == in) {
            return b;
        }
        return a;
    }

    bool Accepts(Direction &in) {
        return a == in || b == in;
    }
};

typedef std::map<char, PipeType> PipeTypes;
typedef std::vector<std::vector<char>> Grid;

struct MoveResult {
    int x, y;
    Direction next;
};

bool Move(MoveResult &result, PipeTypes &types, Grid &grid, Direction &in) {
    if (result.x + in.x >= 0 && result.x + in.x < grid[0].size() && result.y + in.y >= 0 && result.y + in.y < grid.size()) {
        char s = grid[result.y][result.x];
        char c = grid[result.y + in.y][result.x + in.x];
        if (types.find(c) != types.end()) {
            if (types[c].Accepts(Opposite(in))) {
                result.x += in.x;
                result.y += in.y;
                result.next = types[c].Out(Opposite(in));
                return true;
            }
        }
    }
    return false;
}

int main() {
    PipeTypes pipe_types;
    pipe_types['|'] = {Up, Down};
    pipe_types['-'] = {Left, Right};
    pipe_types['L'] = {Up, Right};
    pipe_types['J'] = {Up, Left};
    pipe_types['7'] = {Down, Left};
    pipe_types['F'] = {Down, Right};

    MoveResult start;
    Grid grid;
    std::string line;
    std::ifstream input("input.txt");
    while (std::getline(input, line)) {
        std::vector<char> row;
        for (char &c : line) {
            row.push_back(c);
            if (c == 'S') {
                start.x = row.size() - 1;
                start.y = grid.size();
            }
        }
        grid.push_back(row);
    }

    MoveResult result;
    result.x = start.x;
    result.y = start.y;
    result.next = Right;
    Move(result, pipe_types, grid, result.next);

    int moves = 1;
    while (true) {
        Move(result, pipe_types, grid, result.next);
        if (result.x + result.next.x == start.x && result.y + result.next.y == start.y) {
            break;
        }
        moves++;
    }
    std::cout << moves / 2 + 1 << std::endl;
}