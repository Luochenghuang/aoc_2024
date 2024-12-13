/*
To solve this puzzle, we use a linear algebra approach.
We define a_x, a_y are the x and y movement after pressing a button;
b_x, b_y are the x and y movement after pressing b button.
We then construct a 2x2 matrix to represent the action of the a and b buttons as:
A = |a_x a_y|
    |b_x b_y|
We can represent the total movement as a vector:
P = |p_x|
    |p_y|
Then we have:
P = A * |n_a|
        |n_b|
Where n_a and n_b are the number of times the a and b buttons are pressed, which
can be solved by:
|n_a| = A^(-1) * P
|n_b|

The tricky part to to make sure that n_a and n_b are non-negative integers.
*/
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

struct GameDict {
    std::pair<int, int> a;
    std::pair<int, int> b;
    std::pair<int, int> location;
};

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

std::vector<GameDict> input_to_dicts(const std::string& input) {
    std::vector<GameDict> dicts;
    auto blocks = split(input, "\n\n");
    
    for (const auto& block : blocks) {
        auto lines = split(block, "\n");
        auto button_a = lines[0];
        auto button_b = lines[1];
        auto prize = lines[2];

        auto button_a_coords = split(split(button_a, ":")[1], ",");
        auto button_b_coords = split(split(button_b, ":")[1], ",");
        auto prize_coords = split(split(prize, ":")[1], ",");

        GameDict dict;
        dict.a = {
            std::stoi(split(button_a_coords[0], "+")[1]),
            std::stoi(split(button_a_coords[1], "+")[1])
        };
        dict.b = {
            std::stoi(split(button_b_coords[0], "+")[1]),
            std::stoi(split(button_b_coords[1], "+")[1])
        };
        dict.location = {
            std::stoi(split(prize_coords[0], "=")[1]),
            std::stoi(split(prize_coords[1], "=")[1])
        };
        dicts.push_back(dict);
    }
    return dicts;
}

std::pair<long long, long long> compute_a_b(const GameDict& dict, bool part_1) {
    std::vector<std::vector<double>> matrix = {
        {static_cast<double>(dict.a.first), static_cast<double>(dict.b.first)},
        {static_cast<double>(dict.a.second), static_cast<double>(dict.b.second)}
    };
    
    double det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    if (std::abs(det) < 1e-10) return {-1, -1};  // Matrix is not invertible
    
    std::vector<std::vector<double>> inv = {
        {matrix[1][1] / det, -matrix[0][1] / det},
        {-matrix[1][0] / det, matrix[0][0] / det}
    };
    
    std::vector<double> location = {
        static_cast<double>(dict.location.first),
        static_cast<double>(dict.location.second)
    };
    if (!part_1) {
        location[0] += 10000000000000.0;
        location[1] += 10000000000000.0;
    }
    
    std::vector<double> result = {
        inv[0][0] * location[0] + inv[0][1] * location[1],
        inv[1][0] * location[0] + inv[1][1] * location[1]
    };
    
    auto is_int = [](double x) {
        return std::abs(std::round(x) - x) < 1e-3;
    };

    if (result[0] >= 0 && result[1] >= 0 && is_int(result[0]) && is_int(result[1])) {
        return {std::round(result[0]), std::round(result[1])};
    }
    return {-1, -1};
}

long long solve(const std::string& input, bool part_1) {
    auto dicts = input_to_dicts(input);
    long long cost = 0;
    
    for (const auto& dict : dicts) {
        auto [n_a, n_b] = compute_a_b(dict, part_1);
        if (n_a != -1) {
            cost += n_a * 3 + n_b * 1;
        }
    }
    return cost;
}

int main() {
    std::string test_input = readFile("test_input.txt");
    std::string input = readFile("input.txt");
    std::cout << "part_1_test: " << solve(test_input, true) << std::endl;
    std::cout << "part_1:      " << solve(input, true) << std::endl;
    std::cout << "part_2:      " << solve(input, false) << std::endl;

    return 0;
}