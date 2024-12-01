#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <sstream>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

std::vector<int> parseInput(const std::string& input) {
    std::vector<int> numbers;
    std::istringstream stream(input);
    int num;
    while (stream >> num) {
        numbers.push_back(num);
    }
    return numbers;
}

int part_1(const std::string& input) {
    std::vector<int> first_col, second_col;
    std::istringstream stream(input);
    std::string line;
    
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        int num1, num2;
        lineStream >> num1 >> num2;
        first_col.push_back(num1);
        second_col.push_back(num2);
    }

    std::vector<int> first_col_sorted = first_col;
    std::vector<int> second_col_sorted = second_col;
    std::sort(first_col_sorted.begin(), first_col_sorted.end());
    std::sort(second_col_sorted.begin(), second_col_sorted.end());

    int distance = 0;
    for (size_t i = 0; i < first_col_sorted.size(); i++) {
        distance += std::abs(first_col_sorted[i] - second_col_sorted[i]);
    }
    
    return distance;
}

int part_2(const std::string& input) {
    std::vector<int> first_col, second_col;
    std::istringstream stream(input);
    std::string line;
    
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        int num1, num2;
        lineStream >> num1 >> num2;
        first_col.push_back(num1);
        second_col.push_back(num2);
    }

    int total = 0;
    for (int num : first_col) {
        int count = std::count(second_col.begin(), second_col.end(), num);
        total += count * num;
    }
    
    return total;
}

int main() {
    std::string input = readFile("input.txt");
    std::string test_input = readFile("test.txt");

    std::cout << "part_1_test: " << part_1(test_input) << std::endl;
    std::cout << "part_1:      " << part_1(input) << std::endl;
    
    std::cout << "part_2_test: " << part_2(test_input) << std::endl;
    std::cout << "part_2:      " << part_2(input) << std::endl;

    return 0;
}