/*
Part 1: Moving Single-Width Objects
* 1. Parse input into:
   - Map with walls (#)
   - Robot (@)
   - Boxes (O)
* 2. For each move in the sequence:
   - Check if move is valid (no wall or box blocking)
   - If box is in the way, try to push it first
   - Move robot if possible
* 3. Calculate final score:
   - Sum (row * 100 + column) for each box position

Part 2: Moving Double-Width Boxes
1. Parse input similar to Part 1, but:
   - Convert each box into width-2 box []
   - Keep robot as single width @
   - Double the map width
* 2. For each move:
   - Handle vertical moves (^,v) for full box width
   - Handle horizontal moves (<,>) from box edge only
   - Check all affected objects before moving
   - Move all objects together when valid
* 3. Calculate score same as Part 1, using left edge of boxes
*/
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <unordered_map>
#include <variant>
#include <queue>
#include <set>

using Point = std::pair<int, int>;
using ParticleData = std::pair<Point, Point>;

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end;
    while ((end = s.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(s.substr(start));
    return tokens;
}

class Movable {
public:
    Point location;
    explicit Movable(Point loc) : location(loc) {}
    virtual ~Movable() = default;
};

class Box : public Movable {
public:
    explicit Box(Point loc) : Movable(loc) {}
};

class Robot : public Movable {
public:
    explicit Robot(Point loc) : Movable(loc) {}
};

class MapPart1 {
private:
    std::map<char, Point> delta_location_dict = {
        {'<', {0, -1}},
        {'>', {0, 1}},
        {'^', {-1, 0}},
        {'v', {1, 0}}
    };
    std::vector<std::vector<char>> raw_map;
    std::vector<std::vector<int>> walls;
    std::vector<std::unique_ptr<Box>> boxes;
    std::unique_ptr<Robot> robot;

public:
    explicit MapPart1(const std::vector<std::vector<char>>& inputMap) {
        raw_map = inputMap;
        walls = std::vector<std::vector<int>>(
            inputMap.size(), std::vector<int>(inputMap[0].size(), 0));

        // Initialize walls
        for (int i = 0; i < inputMap.size(); ++i) {
            for (int j = 0; j < inputMap[i].size(); ++j) {
                if (inputMap[i][j] == '#') {
                    walls[i][j] = 1;
                }
                else if (inputMap[i][j] == 'O') {
                    boxes.push_back(std::make_unique<Box>(Point{i, j}));
                }
                else if (inputMap[i][j] == '@') {
                    robot = std::make_unique<Robot>(Point{i, j});
                }
            }
        }
    }

    Point get_location_at_direction(const Point& location, char direction) {
        const auto& delta = delta_location_dict[direction];
        return {location.first + delta.first, location.second + delta.second};
    }

    Box* get_box_at(const Point& location) {
        for (const auto& box : boxes) {
            if (box->location == location) {
                return box.get();
            }
        }
        return nullptr;
    }

    char get_neighbour_at_direction(const Movable* movable, char direction) {
        Point nLoc = get_location_at_direction(movable->location, direction);
        if (nLoc.first < 0 || nLoc.first >= raw_map.size() ||
            nLoc.second < 0 || nLoc.second >= raw_map[0].size()) {
            return '#';
        }
        if (walls[nLoc.first][nLoc.second] == 1) {
            return '#';
        }
        Box* box = get_box_at(nLoc);
        return box ? 'O' : '.';
    }

    bool move(Movable* movable, char direction) {
        char neighbour = get_neighbour_at_direction(movable, direction);
        bool canMove = true;

        if (neighbour == '#') {
            canMove = false;
        }
        else if (neighbour == '.') {
            canMove = true;
        }
        else if (neighbour == 'O') {
            Box* box = get_box_at(get_location_at_direction(movable->location, direction));
            canMove = move(box, direction);
        }

        if (canMove) {
            movable->location = get_location_at_direction(movable->location, direction);
        }
        return canMove;
    }

    void move_robot(char direction) {
        move(robot.get(), direction);
    }

    int sum_gps() const {
        int tally = 0;
        for (const auto& box : boxes) {
            tally += box->location.first * 100 + box->location.second;
        }
        return tally;
    }
};

template<typename MapType>
std::tuple<MapType, std::vector<char>> parseInput(const std::string& input_str) {
    // Split input into map and moves
    size_t split_pos = input_str.find("\n\n");
    std::string map_str = input_str.substr(0, split_pos);
    std::string moves_str = input_str.substr(split_pos + 2);

    // Parse map
    std::vector<std::vector<char>> map_arr;
    size_t pos = 0;
    std::string line;
    while ((pos = map_str.find('\n')) != std::string::npos) {
        line = map_str.substr(0, pos);
        map_arr.push_back(std::vector<char>(line.begin(), line.end()));
        map_str.erase(0, pos + 1);
    }
    map_arr.push_back(std::vector<char>(map_str.begin(), map_str.end()));

    MapType map(map_arr);

    // Parse moves
    std::vector<char> moves(moves_str.begin(), moves_str.end());

    return std::tuple<MapType, std::vector<char>>(std::move(map), std::move(moves));
}

int part1(const std::string& inputStr) {
    auto [map, moves] = parseInput<MapPart1>(inputStr);
    
    for (char move : moves) {
        map.move_robot(move);
    }
    return map.sum_gps();
}

// Part 2 ================================================================


class Movable2 {
public:
    std::vector<std::pair<int, int>> locations;
    
    Movable2(const std::vector<std::pair<int, int>>& locs) : locations(locs) {}
    virtual ~Movable2() = default;
};

class Box2 : public Movable2 {
public:
    Box2(const std::vector<std::pair<int, int>>& locs) : Movable2(locs) {}
};

class Robot2 : public Movable2 {
public:
    Robot2(const std::vector<std::pair<int, int>>& locs) : Movable2(locs) {}
};

class MapPart2 {
private:
    std::unordered_map<char, std::pair<int, int>> delta_location_dict;
    std::vector<std::vector<char>> raw_map;
    std::vector<std::vector<int>> walls;
    std::vector<std::shared_ptr<Box2>> boxes;
    std::shared_ptr<Robot2> robot;
    int nx, ny;

public:
    MapPart2(const std::vector<std::vector<char>>& input_map) {
        delta_location_dict = {
            {'<', {0, -1}},
            {'>', {0, 1}},
            {'^', {-1, 0}},
            {'v', {1, 0}}
        };

        raw_map = input_map;
        ny = input_map.size();
        nx = input_map[0].size() * 2;

        // Initialize walls
        walls = std::vector<std::vector<int>>(ny, std::vector<int>(nx, 0));
        
        // Convert walls
        for (int i = 0; i < ny; i++) {
            for (int j = 0; j < nx/2; j++) {
                if (input_map[i][j] == '#') {
                    walls[i][j*2] = 1;
                    walls[i][j*2 + 1] = 1;
                }
            }
        }

        // Convert boxes
        for (int i = 0; i < ny; i++) {
            for (int j = 0; j < nx/2; j++) {
                if (input_map[i][j] == 'O') {
                    std::vector<std::pair<int, int>> box_locs = {
                        {i, j*2},
                        {i, j*2 + 1}
                    };
                    boxes.push_back(std::make_shared<Box2>(box_locs));
                }
            }
        }

        // Convert robot
        for (int i = 0; i < ny; i++) {
            for (int j = 0; j < nx/2; j++) {
                if (input_map[i][j] == '@') {
                    std::vector<std::pair<int, int>> robot_loc = {{i, j*2}};
                    robot = std::make_shared<Robot2>(robot_loc);
                    break;
                }
            }
        }
    }

    std::vector<std::pair<int, int>> get_location_at_direction(
        const std::vector<std::pair<int, int>>& locations, 
        char direction
    ) {
        auto delta = delta_location_dict[direction];
        std::vector<std::pair<int, int>> result;

        if (direction == '^' || direction == 'v') {
            for (const auto& loc : locations) {
                result.push_back({
                    loc.first + delta.first,
                    loc.second + delta.second
                });
            }
        } else {
            const auto& loc = (direction == '<') ? locations[0] : locations.back();
            result.push_back({
                loc.first + delta.first,
                loc.second + delta.second
            });
        }
        return result;
    }

    std::shared_ptr<Box2> get_box_at(const std::pair<int, int>& location) {
        for (const auto& box : boxes) {
            for (const auto& loc : box->locations) {
                if (loc == location) return box;
            }
        }
        return nullptr;
    }

    std::vector<std::variant<char, std::shared_ptr<Box2>>> get_immediate_neighbours_at_direction(
        const std::shared_ptr<Movable2>& movable, 
        char direction
    ) {
        auto n_loc_list = get_location_at_direction(movable->locations, direction);
        std::vector<std::variant<char, std::shared_ptr<Box2>>> neighbours;

        for (const auto& n_loc : n_loc_list) {
            if (n_loc.first < 0 || n_loc.first >= ny || 
                n_loc.second < 0 || n_loc.second >= nx) {
                neighbours.push_back('#');
            }
            else if (walls[n_loc.first][n_loc.second] == 1) {
                neighbours.push_back('#');
            }
            else {
                auto box = get_box_at(n_loc);
                if (box) {
                    neighbours.push_back(box);
                } else {
                    neighbours.push_back('.');
                }
            }
        }
        return neighbours;
    }

    bool is_movable(const std::shared_ptr<Movable2>& movable, char direction) {
        auto neighbours = get_immediate_neighbours_at_direction(movable, direction);
        
        for (const auto& neighbour : neighbours) {
            if (std::holds_alternative<char>(neighbour)) {
                if (std::get<char>(neighbour) == '#') {
                    return false;
                }
            } else {
                auto box = std::get<std::shared_ptr<Box2>>(neighbour);
                if (!is_movable(box, direction)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool move(const std::shared_ptr<Movable2>& movable, char direction) {
        if (!is_movable(movable, direction)) {
            return false;
        }

        // Get all movable objects that need to be moved
        std::set<std::shared_ptr<Movable2>> to_move;
        std::queue<std::shared_ptr<Movable2>> to_check;
        to_move.insert(movable);
        to_check.push(movable);

        while (!to_check.empty()) {
            auto current = to_check.front();
            to_check.pop();

            auto neighbours = get_immediate_neighbours_at_direction(current, direction);
            for (const auto& neighbour : neighbours) {
                if (std::holds_alternative<std::shared_ptr<Box2>>(neighbour)) {
                    auto box = std::get<std::shared_ptr<Box2>>(neighbour);
                    if (to_move.find(box) == to_move.end()) {
                        to_move.insert(box);
                        to_check.push(box);
                    }
                }
            }
        }

        // Move all objects
        auto delta = delta_location_dict[direction];
        for (const auto& obj : to_move) {
            std::vector<std::pair<int, int>> new_locs;
            for (const auto& loc : obj->locations) {
                new_locs.push_back({
                    loc.first + delta.first,
                    loc.second + delta.second
                });
            }
            obj->locations = new_locs;
        }

        return true;
    }

    bool move(char direction) {
        return move(robot, direction);
    }

    void print() {
        std::vector<std::vector<char>> map_str(ny, std::vector<char>(nx, '.'));
        
        for (int i = 0; i < ny; i++) {
            for (int j = 0; j < nx; j++) {
                if (walls[i][j] == 1) map_str[i][j] = '#';
            }
        }

        for (const auto& box : boxes) {
            map_str[box->locations[0].first][box->locations[0].second] = '[';
            map_str[box->locations[1].first][box->locations[1].second] = ']';
        }

        map_str[robot->locations[0].first][robot->locations[0].second] = '@';

        for (const auto& row : map_str) {
            for (char c : row) std::cout << c;
            std::cout << '\n';
        }
    }

    int sum_gps() {
        int tally = 0;
        for (const auto& box : boxes) {
            tally += box->locations[0].first * 100 + box->locations[0].second;
        }
        return tally;
    }
};

int part2(const std::string& input_str) {
    auto [map, moves] = parseInput<MapPart2>(input_str);
    
    for (char move : moves) {
        map.move(move);
    }
    return map.sum_gps();
}

int main() {
    std::string test_input = read_file("test_input.txt");
    std::string input = read_file("input.txt");
    std::cout << "part_1_test: " << part1(test_input) << std::endl;
    std::cout << "part_1     : " << part1(input) << std::endl;
    std::cout << "part_2_test: " << part2(test_input) << std::endl;
    std::cout << "part_2     : " << part2(input) << std::endl;

    return 0;
}