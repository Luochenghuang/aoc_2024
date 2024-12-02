#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

bool is_safe(const std::vector<int>& array) {
    int prev = array[0];
    bool is_increasing = true;
    bool is_decreasing = true;
    
    for (size_t i = 1; i < array.size(); i++) {
        int num = array[i];
        if (num <= prev) {
            is_increasing = false;
        }
        if (num >= prev) {
            is_decreasing = false;
        }
        if (abs(num - prev) > 3) {
            is_increasing = false;
            is_decreasing = false;
        }
        prev = num;
    }
    
    return is_increasing || is_decreasing;
}

int part_1(const std::string& input) {
    std::istringstream iss(input);
    std::string line;
    int total_safe = 0;
    
    while (std::getline(iss, line)) {
        std::vector<int> array;
        std::istringstream line_stream(line);
        int num;
        while (line_stream >> num) {
            array.push_back(num);
        }
        if (is_safe(array)) {
            total_safe++;
        }
    }
    return total_safe;
}

int part_2(const std::string& input) {
    std::istringstream iss(input);
    std::string line;
    int total_safe = 0;
    
    while (std::getline(iss, line)) {
        std::vector<int> array;
        std::istringstream line_stream(line);
        int num;
        while (line_stream >> num) {
            array.push_back(num);
        }
        
        if (is_safe(array)) {
            total_safe++;
        } else {
            for (size_t i = 0; i < array.size(); i++) {
                std::vector<int> new_array;
                for (size_t j = 0; j < array.size(); j++) {
                    if (j != i) {
                        new_array.push_back(array[j]);
                    }
                }
                if (is_safe(new_array)) {
                    total_safe++;
                    break;
                }
            }
        }
    }
    return total_safe;
}

int main() {
    std::ifstream input_file("input.txt");
    std::ifstream test_file("test.txt");
    
    std::string input((std::istreambuf_iterator<char>(input_file)),
                     std::istreambuf_iterator<char>());
    std::string test_input((std::istreambuf_iterator<char>(test_file)),
                          std::istreambuf_iterator<char>());
    
    std::cout << "part_1_test: " << part_1(test_input) << "\n";
    std::cout << "part_1:      " << part_1(input) << "\n";
    
    std::cout << "part_2_test: " << part_2(test_input) << "\n";
    std::cout << "part_2:      " << part_2(input) << "\n";
    
    return 0;
}