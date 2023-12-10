#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_set>
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

    bool Collision() {
        return a.x != 0 && b.x != 0;
    }

    bool PartialCollision() {
        return a.y != 0 && b.x != 0;
    }
};

typedef std::map<char, PipeType> PipeTypes;
typedef std::vector<std::vector<char>> Grid;

struct MoveResult {
    int x, y;
    Direction next;
};

struct Path {
    int x, y;

    bool operator==(const Path &other) const {
        return x == other.x && y == other.y;
    }
};

class PathHash {
public:
    size_t operator()(const Path &path) const noexcept {
        size_t h1 = std::hash<int>{}(path.x);
        size_t h2 = std::hash<int>{}(path.y);
        return h1 ^ (h2 << 1);
    }
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

void ReplaceStart(MoveResult &result, PipeTypes &types, Grid &grid) {
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

    std::unordered_set<Path, PathHash> path_nodes;
    path_nodes.insert({start.x, start.y});

    MoveResult result;
    result.x = start.x;
    result.y = start.y;
    result.next = Right;
    Move(result, pipe_types, grid, result.next);
    path_nodes.insert({result.x, result.y});

    grid[start.y][start.x] = '-';

    while (true) {
        Move(result, pipe_types, grid, result.next);
        path_nodes.insert({result.x, result.y});
        if (result.x + result.next.x == start.x && result.y + result.next.y == start.y) {
            break;
        }
    }

    int area = 0;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[0].size(); x++) {
            if (path_nodes.find({x, y}) != path_nodes.end())
                continue;
            int collisions = 0;
            int test_y = y - 1;
            while (test_y >= 0) {
                if (path_nodes.find({x, test_y}) != path_nodes.end()) {
                    char c = grid[test_y][x];
                    if (pipe_types[c].Collision()) {
                        collisions++;
                    } else if (pipe_types[c].PartialCollision()) {
                        int h = pipe_types[c].b.x;
                        test_y--;
                        while (test_y >= 0) {
                            c = grid[test_y][x];
                            if (!pipe_types[c].PartialCollision()) {
                                test_y--;
                                continue;
                            }
                            if (pipe_types[c].b.x != h) {
                                collisions++;
                            }
                            test_y--;
                            break;
                        }
                        continue;
                    }
                }
                test_y--;
            }
            if (collisions % 2 == 1) {
                area++;
                std::cout << "Inside: " << (x + 1) << " " << (y + 1) << std::endl;
            }
            // std::cout << "------------" << std::endl;
        }
    }
    std::cout << area << std::endl;
}