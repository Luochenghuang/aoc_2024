/*
Part 1:
* 1. Parse the input into a list of particles
* 2. For each particle, predict its position at time t
    * new_position = (p + v * time) % bounds
    * but to make sure it's positive, we do (position + bounds) % bounds
* 3. Count the number of particles in each quadrant
* 4. Return the product of the counts

Part 2:
The trick is that the easter egg image happens when none of the particles overlap.
*/
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>

using Point = std::pair<int, int>;
using ParticleData = std::pair<Point, Point>;

std::string readFile(const std::string& filename) {
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

std::vector<ParticleData> input_to_list(const std::string& input) {
    std::vector<ParticleData> p_and_v_list;
    auto lines = split(input, "\n");
    
    for (const auto& line : lines) {
        auto parts = split(line, " ");
        auto p_parts = split(split(parts[0], "=")[1], ",");
        auto v_parts = split(split(parts[1], "=")[1], ",");
        
        Point p{std::stoi(p_parts[0]), std::stoi(p_parts[1])};
        Point v{std::stoi(v_parts[0]), std::stoi(v_parts[1])};
        p_and_v_list.push_back({p, v});
    }
    return p_and_v_list;
}

// new_position = (p + v * time) % bounds
// but to make sure it's positive, we do (position + bounds) % bounds
Point predict_single_particle_position(int time, const ParticleData& p_and_v, const Point& bounds) {
    auto [p, v] = p_and_v;
    int p_x = ((p.first + v.first * time) % bounds.first + bounds.first) % bounds.first;
    int p_y = ((p.second + v.second * time) % bounds.second + bounds.second) % bounds.second;
    return {p_x, p_y};
}

std::vector<std::vector<int>> predict_particle_position(int time, const std::vector<ParticleData>& p_and_v_list, const Point& bounds) {
    std::vector<std::vector<int>> map(bounds.first, std::vector<int>(bounds.second, 0));
    for (const auto& p_and_v : p_and_v_list) {
        auto [p_x, p_y] = predict_single_particle_position(time, p_and_v, bounds);
        map[p_x][p_y]++;
    }
    return map;
}

std::array<int, 4> get_4_quadrant_particle_counts(const std::vector<std::vector<int>>& map) {
    int nx = map.size();
    int ny = map[0].size();
    
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    
    for (int i = 0; i < nx/2; i++) {
        for (int j = 0; j < ny/2; j++) {
            q1 += map[i][j];
        }
        for (int j = ny/2 + 1; j < ny; j++) {
            q2 += map[i][j];
        }
    }
    
    for (int i = nx/2 + 1; i < nx; i++) {
        for (int j = 0; j < ny/2; j++) {
            q3 += map[i][j];
        }
        for (int j = ny/2 + 1; j < ny; j++) {
            q4 += map[i][j];
        }
    }
    
    return {q1, q2, q3, q4};
}

int solve(const std::string& input, int time, const Point& bounds) {
    auto p_and_v_list = input_to_list(input);
    auto map = predict_particle_position(time, p_and_v_list, bounds);
    auto [a, b, c, d] = get_4_quadrant_particle_counts(map);
    return a * b * c * d;
}

// for debugging
void print_map(const std::vector<std::vector<int>>& map) {
    for (int j = 0; j < map[0].size(); j++) {
        for (int i = 0; i < map.size(); i++) {
            if (map[i][j] == 0) {
                std::cout << '.';
            } else if (map[i][j] == 1) {
                std::cout << '#';
            } else {
                std::cout << '@'; // For overlapping particles
            }
        }
        std::cout << std::endl;
    }
}

int part_1(const std::string& input) {
    return solve(input, 100, {101, 103});
}

void part_2(const std::string& input) {
    auto p_and_v_list = input_to_list(input);
    // the image must repeat every 101 * 103 steps
    for (int i = 0; i < 101 * 103; i++) {
        auto map = predict_particle_position(i, p_and_v_list, {101, 103});
        bool all_ones = true;
        for (const auto& row : map) {
            for (int val : row) {
                if (val > 1) {
                    all_ones = false;
                    break;
                }
            }
            if (!all_ones) break;
        }
        if (all_ones) {
            std::cout << "\nFinal state at time " << i << ":\n";
            print_map(map);
            return;
        }
    }
}

int main() {
    std::string test_input = readFile("test_input.txt");
    std::string input = readFile("input.txt");
    std::cout << "part_1_test: " << solve(test_input, 100, {7, 11}) << std::endl;
    std::cout << "part_1:      " << part_1(input) << std::endl;
    part_2(input);

    return 0;
}