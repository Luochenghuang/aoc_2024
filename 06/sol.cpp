/*
Part 1:
* 1. We first parse the input into a Map object.
* 2. We then find the carat and its direction.
* 3. We then move the carat to the next position.
* 4. We then update the map with the new position of the carat.
* 5. We repeat the process until the carat is out of bounds or we have found a loop.
* 6. Finally, we return the total number of visited positions.

Part 2:
* 1. Same as part 1, but we also check if the carat has travelled in the historic path.
    * Only if the carat is traveling in the same historic PATH AND DIRECTION do we 
        consider it a loop. 
* 2. We then return the total number of loops.
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <array>
#include <algorithm>
#include <utility>
#include <set>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}
enum class ExitCode {
    LOOP,
    SUCCESS,
    OUT_OF_BOUNDS
};

// Stores the map and the historic movement of the carat.
class Map {
private:
    std::vector<std::vector<char>> current_map;
    std::vector<std::vector<std::vector<int>>> history_map;  // 3D map for detect_loop=true
    std::pair<int, int> current_loc;
    int current_dir_idx;
    int time;
    bool debug;
    bool detect_loop;
    bool travelled_in_historic_path;
    bool turning;

    // returns the position and direction of the carat
    std::pair<std::pair<int, int>, int> find_carat() {
        std::array<char, 4> carat_chars = {'^', '>', 'v', '<'};
        for (int i = 0; i < current_map.size(); i++) {
            for (int j = 0; j < current_map[0].size(); j++) {
                for (int idx = 0; idx < 4; idx++) {
                    if (current_map[i][j] == carat_chars[idx]) {
                        return {{i, j}, idx};
                    }
                }
            }
        }
        return {{-1, -1}, -1};
    }

    char get_carat_from_dir_idx() {
        std::array<char, 4> carat_chars = {'^', '>', 'v', '<'};
        return carat_chars[current_dir_idx];
    }

    // returns the location of that the carat is facing.
    std::pair<int, int> find_loc_in_front() {
        std::pair<int, int> loc = current_loc;
        switch (current_dir_idx) {
            case 0: loc.first--; break;  // Up
            case 1: loc.second++; break; // Right
            case 2: loc.first++; break;  // Down
            case 3: loc.second--; break; // Left
        }
        
        if (loc.first < 0 || loc.first >= current_map.size() || 
            loc.second < 0 || loc.second >= current_map[0].size()) {
            return {-1, -1};
        }
        return loc;
    }

    // returns the object in front of the carat
    char find_obj_in_front() {
        auto loc = find_loc_in_front();
        if (loc.first == -1) return '\0';
        return current_map[loc.first][loc.second];
    }

public:
    Map(const std::string& input, bool detect_loop = false, bool debug = false) 
        : debug(debug), detect_loop(detect_loop), time(0), 
          travelled_in_historic_path(false), turning(false) {
        
        std::stringstream ss(input);
        std::string line;
        while (std::getline(ss, line)) {
            current_map.push_back(std::vector<char>(line.begin(), line.end()));
        }

        if (detect_loop) {
            history_map = std::vector<std::vector<std::vector<int>>>(
                4, std::vector<std::vector<int>>(
                    current_map.size(), std::vector<int>(current_map[0].size(), 0)));
        } else {
            history_map = std::vector<std::vector<std::vector<int>>>(
                1, std::vector<std::vector<int>>(
                    current_map.size(), std::vector<int>(current_map[0].size(), 0)));
        }

        auto [loc, dir] = find_carat();
        current_loc = loc;
        current_dir_idx = dir;
    }

    // Moves the carat to the next position. Updates the states of the map.
    ExitCode update() {
        time++;
        if (detect_loop) {
            if (!turning) {
                if (history_map[current_dir_idx][current_loc.first][current_loc.second] == 1) {
                    if (travelled_in_historic_path) {
                        // If the carat has travelled in the historic path, we have found a loop.
                        return ExitCode::LOOP;
                    }
                    travelled_in_historic_path = true;
                } else {
                    travelled_in_historic_path = false;
                }
            }
            history_map[current_dir_idx][current_loc.first][current_loc.second] = 1;
        } else {
            history_map[0][current_loc.first][current_loc.second] = 1;
        }

        char obj_in_front = find_obj_in_front();
        if (obj_in_front == '.') {
            current_map[current_loc.first][current_loc.second] = '.';
            current_loc = find_loc_in_front();
            current_map[current_loc.first][current_loc.second] = get_carat_from_dir_idx();
            turning = false;
        } else if (obj_in_front == '#') {
            current_dir_idx = (current_dir_idx + 1) % 4;
            turning = true;
        } else if (obj_in_front == '\0') {
            return ExitCode::OUT_OF_BOUNDS;
        }
        return ExitCode::SUCCESS;
    }

    // Returns the total number of visited positions.
    int get_total_visited() {
        int sum = 0;
        for (int i = 0; i < current_map.size(); i++) {
            for (int j = 0; j < current_map[0].size(); j++) {
                if (!detect_loop) {
                    sum += history_map[0][i][j];
                } else {
                    for (int k = 0; k < 4; k++) {
                        if (history_map[k][i][j] > 0) {
                            sum++;
                            break;
                        }
                    }
                }
            }
        }
        return sum;
    }

    void set_obstacle(int i, int j) {
        current_map[i][j] = '#';
    }

    std::vector<std::pair<int, int>> get_dot_locations() {
        std::vector<std::pair<int, int>> dots;
        for (int i = 0; i < current_map.size(); i++) {
            for (int j = 0; j < current_map[0].size(); j++) {
                if (current_map[i][j] == '.') {
                    dots.push_back({i, j});
                }
            }
        }
        return dots;
    }
};

int part_1(const std::string& input) {
    Map map(input);
    while (map.update() == ExitCode::SUCCESS) {}
    return map.get_total_visited();
}

int part_2(const std::string& input) {
    Map og_map(input, true);
    auto dot_loc_list = og_map.get_dot_locations();
    int total_loops = 0;

    for (const auto& loc : dot_loc_list) {
        Map map(input, true);
        map.set_obstacle(loc.first, loc.second);
        
        while (true) {
            ExitCode exit_code = map.update();
            if (exit_code == ExitCode::LOOP) {
                total_loops++;
                break;
            } else if (exit_code == ExitCode::OUT_OF_BOUNDS) {
                break;
            }
        }
    }
    return total_loops;
}

int main() {
    std::string test_input = readFile("test_input.txt");
    std::string input = readFile("input.txt");

    std::cout << "part_1_test: " << part_1(test_input) << std::endl;
    std::cout << "part_1:      " << part_1(input) << std::endl;
    std::cout << "part_2_test: " << part_2(test_input) << std::endl;
    std::cout << "part_2:      " << part_2(input) << std::endl;

    return 0;
}