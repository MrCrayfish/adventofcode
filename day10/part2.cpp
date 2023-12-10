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

    bool IsHorizontal() {
        return a.x != 0 && b.x != 0;
    }

    bool IsCorner() {
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

    // Figured you can use the polygon technique to determine if a point is inside.
    // Odd collisions means the point is inside, otherwise it's outside.

    int area = 0;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[0].size(); x++) {

            // If the point is on the path, skip. Only looking for non-path positions
            if (path_nodes.find({x, y}) != path_nodes.end())
                continue;

            int collisions = 0;

            // We are testing for collision going up
            int test_y = y;
            while (--test_y >= 0) {
                // If test point is not on the path, skip.
                if (path_nodes.find({x, test_y}) == path_nodes.end())
                    continue;

                // Get the pipe at the test point
                char c = grid[test_y][x];

                // If pipe is horizontal (-) then we crossed in/out of the shape
                if (pipe_types[c].IsHorizontal()) {
                    collisions++;
                    continue;
                }

                // If pipe is a corner (L or J), we need to find the above corner (F or 7)
                if (pipe_types[c].IsCorner()) {

                    // Capture the horizontal dir of the corner pipe
                    int h = pipe_types[c].b.x;

                    // Now find the above corner pipe
                    while (--test_y > 0) {
                        c = grid[test_y][x];
                        if (pipe_types[c].IsCorner()) {
                            // If above corner pipe horizontal dir is not  the same
                            // as the captured, then we crossed in/out of the shape.
                            if (pipe_types[c].b.x != h) {
                                collisions++;
                            }
                            break;
                        }
                    }
                }
            }

            // If collided an odd amount of times, the point was inside the path
            if (collisions % 2 == 1) {
                area++;
            }
        }
    }
    std::cout << area << std::endl;
}