#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <array>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

using Grid = std::vector<std::vector<char>>;

const std::array<char, 4> XMAS = {'X', 'M', 'A', 'S'};
const std::array<char, 4> SAMX = {'S', 'A', 'M', 'X'};

bool array_equal(const std::array<char, 4>& arr1, const std::array<char, 4>& target) {
    return arr1 == target;
}

bool array_has_xmas(const std::array<char, 4>& arr) {
    return array_equal(arr, XMAS) || array_equal(arr, SAMX);
}

std::array<char, 4> get_column(const std::vector<std::vector<char>>& block, int col) {
    std::array<char, 4> result;
    for (int i = 0; i < 4; i++) {
        result[i] = block[i][col];
    }
    return result;
}

std::array<char, 4> get_row(const std::vector<std::vector<char>>& block, int row) {
    std::array<char, 4> result;
    for (int i = 0; i < 4; i++) {
        result[i] = block[row][i];
    }
    return result;
}

std::array<char, 4> get_diagonal(const std::vector<std::vector<char>>& block) {
    std::array<char, 4> result;
    for (int i = 0; i < 4; i++) {
        result[i] = block[i][i];
    }
    return result;
}

std::array<char, 4> get_anti_diagonal(const std::vector<std::vector<char>>& block) {
    std::array<char, 4> result;
    for (int i = 0; i < 4; i++) {
        result[i] = block[i][3-i];
    }
    return result;
}

int search_xmas_in_block(const std::vector<std::vector<char>>& block) {
    int count = 0;
    if (array_has_xmas(get_column(block, 0))) {
        count++;
    }
    if (array_has_xmas(get_row(block, 0))) {
        count++;
    }
    if (array_has_xmas(get_diagonal(block))) {
        count++;
    }
    if (array_has_xmas(get_anti_diagonal(block))) {
        count++;
    }
    
    return count;
}

int part_1(const std::string& input) {
    std::vector<std::vector<char>> char_arr;
    std::stringstream ss(input);
    std::string line;
    
    while (std::getline(ss, line)) {
        std::vector<char> row(line.begin(), line.end());
        char_arr.push_back(row);
    }
    
    int original_rows = char_arr.size();
    int original_cols = char_arr[0].size();
    
    Grid padded_grid(original_rows + 3, std::vector<char>(original_cols + 3, '.'));
    
    for (int i = 0; i < original_rows; i++) {
        for (int j = 0; j < original_cols; j++) {
            padded_grid[i][j] = char_arr[i][j];
        }
    }
    
    int total_xmas = 0;
    
    for (int i = 0; i <= padded_grid.size() - 4; i++) {
        for (int j = 0; j <= padded_grid[0].size() - 4; j++) {
            std::vector<std::vector<char>> block(4, std::vector<char>(4));
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 4; y++) {
                    block[x][y] = padded_grid[i + x][j + y];
                }
            }
            total_xmas += search_xmas_in_block(block);
        }
    }
    
    return total_xmas;
}

std::array<char, 5> get_cross_pattern(const std::vector<std::vector<char>>& block) {
    std::array<char, 5> result;
    result[0] = block[0][0];
    result[1] = block[0][2];
    result[2] = block[1][1];
    result[3] = block[2][0];
    result[4] = block[2][2];
    return result;
}

bool array_equal_5(const std::array<char, 5>& arr1, const std::array<char, 5>& target) {
    return arr1 == target;
}

bool search_cross_mas(const std::vector<std::vector<char>>& block) {
    std::array<char, 5> letters = get_cross_pattern(block);
    
    const std::array<std::array<char, 5>, 4> rotations = {{
        {'M', 'M', 'A', 'S', 'S'},
        {'S', 'S', 'A', 'M', 'M'},
        {'S', 'M', 'A', 'S', 'M'},
        {'M', 'S', 'A', 'M', 'S'}
    }};
    
    for (const auto& rotation : rotations) {
        if (array_equal_5(letters, rotation)) {
            return true;
        }
    }
    return false;
}

int part_2(const std::string& input) {
    std::vector<std::vector<char>> char_arr;
    std::stringstream ss(input);
    std::string line;
    
    while (std::getline(ss, line)) {
        std::vector<char> row(line.begin(), line.end());
        char_arr.push_back(row);
    }
    
    int original_rows = char_arr.size();
    int original_cols = char_arr[0].size();
    
    Grid padded_grid(original_rows + 2, std::vector<char>(original_cols + 2, '.'));
    
    for (int i = 0; i < original_rows; i++) {
        for (int j = 0; j < original_cols; j++) {
            padded_grid[i][j] = char_arr[i][j];
        }
    }
    
    int total_cross_mas = 0;
    
    for (int i = 0; i <= padded_grid.size() - 3; i++) {
        for (int j = 0; j <= padded_grid[0].size() - 3; j++) {
            std::vector<std::vector<char>> block(3, std::vector<char>(3));
            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 3; y++) {
                    block[x][y] = padded_grid[i + x][j + y];
                }
            }
            if (search_cross_mas(block)) {
                total_cross_mas++;
            }
        }
    }
    
    return total_cross_mas;
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