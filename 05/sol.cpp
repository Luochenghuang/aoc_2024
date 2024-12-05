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

std::pair<std::string, std::string> parse_input(const std::string& input) {
    size_t split_pos = input.find("\n\n");
    return {input.substr(0, split_pos), input.substr(split_pos + 2)};
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

std::vector<int> parse_numbers(const std::string& s, char delim = ',') {
    std::vector<int> numbers;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        numbers.push_back(std::stoi(item));
    }
    return numbers;
}

int solve(const std::string& input, int part) {
    auto [p1, p2] = parse_input(input);
    std::set<std::pair<int, int>> compare_list;
    
    for (const auto& line : split(p1, '\n')) {
        auto nums = parse_numbers(line, '|');
        compare_list.insert({nums[0], nums[1]});
    }
    
    auto is_less_than = [&compare_list](int a, int b) {
        if (compare_list.count({a, b})) return true;
        if (compare_list.count({b, a})) return false;
        return a < b;
    };
    
    int total = 0;
    for (const auto& update_list_str : split(p2, '\n')) {
        std::vector<int> update_list = parse_numbers(update_list_str);
        std::vector<int> sorted_list = update_list;
        
        std::sort(sorted_list.begin(), sorted_list.end(), 
            [&is_less_than](int x, int y) {
                return is_less_than(x, y);
            });
        
        if (part == 1) {
            if (update_list == sorted_list) {
                total += update_list[update_list.size() / 2];
            }
        } else {
            if (update_list != sorted_list) {
                total += sorted_list[sorted_list.size() / 2];
            }
        }
    }
    
    return total;
}

int part_1(const std::string& input) {
    return solve(input, 1);
}

int part_2(const std::string& input) {
    return solve(input, 2);
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