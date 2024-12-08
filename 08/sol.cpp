#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

std::vector<std::vector<char>> parse_input_to_2d_vector(const std::string& input) {
    std::vector<std::vector<char>> result;
    std::stringstream ss(input);
    std::string line;
    while (std::getline(ss, line)) {
        result.push_back(std::vector<char>(line.begin(), line.end()));
    }
    return result;
}

template<typename T>
void print_2d_vector(const std::vector<std::vector<T>>& vec) {
    std::cout << "----- print_2d_vector -----" << std::endl;
    for (const auto& row : vec) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "----- end -----" << std::endl;
}

class AntennaArray {
private:
    char frequency;
    std::vector<std::vector<char>> map_array;
    std::vector<std::pair<int, int>> positions;
    std::vector<std::vector<int>> antinode_map;

    std::vector<std::pair<int, int>> find_positions() {
        std::vector<std::pair<int, int>> positions;
        for (int i = 0; i < map_array.size(); ++i) {
            for (int j = 0; j < map_array[0].size(); ++j) {
                if (map_array[i][j] == frequency) {
                    positions.push_back({i, j});
                }
            }
        }
        return positions;
    }

    std::pair<std::pair<int, int>, std::pair<int, int>> find_antinode_positions(const std::pair<int, int>& pos_0, const std::pair<int, int>& pos_1) {
        auto delta_position = std::make_pair(pos_1.first - pos_0.first, pos_1.second - pos_0.second);
        auto antinode_0 = std::make_pair(pos_0.first - delta_position.first, pos_0.second - delta_position.second);
        auto antinode_1 = std::make_pair(pos_1.first + delta_position.first, pos_1.second + delta_position.second);
        return std::make_pair(antinode_0, antinode_1);
    }

    bool check_position_is_in_bounds(const std::pair<int, int>& position) {
        return position.first >= 0 && position.first < map_array.size() && position.second >= 0 && position.second < map_array[0].size();
    }

    template<typename T>
    std::vector<std::pair<T, T>> get_combinations(const std::vector<T>& items) {
        std::vector<std::pair<T, T>> result;
        for (size_t i = 0; i < items.size(); ++i) {
            for (size_t j = i + 1; j < items.size(); ++j) {
                result.push_back({items[i], items[j]});
            }
        }
        return result;
    }

    std::vector<std::pair<int, int>> find_harmonics_positions(const std::pair<int, int>& pos_0, const std::pair<int, int>& pos_1) {
        auto delta_position = std::make_pair(pos_1.first - pos_0.first, pos_1.second - pos_0.second);
        std::vector<std::pair<int, int>> total_harmonics_positions;
        
        for (int i = 0; ; ++i) {
            auto harmonic_0 = std::make_pair(
                pos_0.first - i * delta_position.first,
                pos_0.second - i * delta_position.second
            );
            if (!check_position_is_in_bounds(harmonic_0)) break;
            total_harmonics_positions.push_back(harmonic_0);
        }
        
        for (int i = 0; ; ++i) {
            auto harmonic_1 = std::make_pair(
                pos_1.first + i * delta_position.first,
                pos_1.second + i * delta_position.second
            );
            if (!check_position_is_in_bounds(harmonic_1)) break;
            total_harmonics_positions.push_back(harmonic_1);
        }
        
        return total_harmonics_positions;
    }

public:
    AntennaArray(char frequency, const std::vector<std::vector<char>>& map_array) 
        : frequency(frequency), map_array(map_array) {
        positions = find_positions();
        antinode_map = std::vector<std::vector<int>>(map_array.size(), std::vector<int>(map_array[0].size(), 0));
    }

    std::vector<std::vector<int>> find_all_antinode_positions() {
        auto position_pairs = get_combinations(positions);
        for (const auto& [pos_0, pos_1] : position_pairs) {
            auto [antinode_0, antinode_1] = find_antinode_positions(pos_0, pos_1);
            
            if (check_position_is_in_bounds(antinode_0)) {
                antinode_map[antinode_0.first][antinode_0.second] = 1;
            }
            if (check_position_is_in_bounds(antinode_1)) {
                antinode_map[antinode_1.first][antinode_1.second] = 1;
            }
        }
        return antinode_map;
    }

    std::vector<std::vector<int>> find_all_harmonics_positions() {
        auto position_pairs = get_combinations(positions);
        auto harmonics_map = std::vector<std::vector<int>>(map_array.size(), std::vector<int>(map_array[0].size(), 0));
        for (const auto& [pos_0, pos_1] : position_pairs) {
            auto harmonics_positions = find_harmonics_positions(pos_0, pos_1);
            for (const auto& harmonic_position : harmonics_positions) {
                harmonics_map[harmonic_position.first][harmonic_position.second] = 1;
            }
        }
        return harmonics_map;
    }

};

int solve(const std::string& input, bool part_1) {  
    std::vector<std::vector<char>> map_array = parse_input_to_2d_vector(input);
    std::set<char> freq_set;
    for (const auto& row : map_array) {
        for (char c : row) {
            if (c != '.' && c != '#') {
                freq_set.insert(c);
            }
        }
    }
    int total = 0;
    auto antinode_map = std::vector<std::vector<int>>(map_array.size(), std::vector<int>(map_array[0].size(), 0));
    for (char freq : freq_set) {
        AntennaArray antenna_array(freq, map_array);
        auto current_antinode_map = std::vector<std::vector<int>>(map_array.size(), std::vector<int>(map_array[0].size(), 0));        if (part_1) {
            current_antinode_map = antenna_array.find_all_antinode_positions();
        } else {
            current_antinode_map = antenna_array.find_all_harmonics_positions();
        }
        for (int i = 0; i < antinode_map.size(); ++i) {
            for (int j = 0; j < antinode_map[0].size(); ++j) {
                antinode_map[i][j] = antinode_map[i][j] or current_antinode_map[i][j];
            }
        }
    }
    // print_2d_vector(antinode_map);
    for (const auto& row : antinode_map) {
        for (int elem : row) {
            total += elem;
        }
    }
    return total;
}


int main() {
    std::string test_input = readFile("test_input.txt");
    std::string input = readFile("input.txt");
    std::cout << "part_1_test: " << solve(test_input, 1) << std::endl;
    std::cout << "part_1:      " << solve(input, 1) << std::endl;
    std::cout << "part_2_test: " << solve(test_input, 0) << std::endl;
    std::cout << "part_2:      " << solve(input, 0) << std::endl;

    return 0;
}