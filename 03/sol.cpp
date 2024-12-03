#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

std::vector<std::string> splitString(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = str.find(delimiter);
    
    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    
    result.push_back(str.substr(start));
    return result;
}

bool isInString(const std::string& str, const std::string& substr) {
    return str.find(substr) != std::string::npos;
}

int part_1(const std::string& input) {
    std::vector<std::string> after_mul_list = splitString(input, "mul(");
    int total = 0;
    
    for (size_t i = 1; i < after_mul_list.size(); i++) {
        const std::string& after_mul = after_mul_list[i];
        if (isInString(after_mul, ")")) {
            std::string num_comma_num = splitString(after_mul, ")")[0];
            if (!isInString(num_comma_num, ",")) {
                continue;
            }
            // std::cout << num_comma_num << std::endl;
            std::vector<std::string> nums = splitString(num_comma_num, ",");
            if (nums.size() == 2) {
                bool is_num0 = true;
                bool is_num1 = true;
                for (char c : nums[0]) if (!std::isdigit(c)) is_num0 = false;
                for (char c : nums[1]) if (!std::isdigit(c)) is_num1 = false;
                if (is_num0 && is_num1) {
                    total += std::stoi(nums[0]) * std::stoi(nums[1]);
                }
            }
        }
    }
    return total;
}

std::string preprocess(const std::string& input) {
    std::vector<std::string> after_dont_list = splitString(input, "don't()");
    std::string do_string = after_dont_list[0];
    
    for (size_t i = 1; i < after_dont_list.size(); i++) {
        std::vector<std::string> do_parts = splitString(after_dont_list[i], "do()");
        for (size_t j = 1; j < do_parts.size(); j++) {
            do_string += do_parts[j];
        }
    }
    return do_string;
}

int part_2(const std::string& input) {
    std::string do_string = preprocess(input);
    return part_1(do_string);
}

int main() {
    std::string input = readFile("input.txt");
    std::string test_input = readFile("test.txt");
    std::string test_input_2 = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";

    std::cout << "part_1_test: " << part_1(test_input) << std::endl;
    std::cout << "part_1:      " << part_1(input) << std::endl;
    
    std::cout << "part_2_test: " << part_2(test_input_2) << std::endl;
    std::cout << "part_2:      " << part_2(input) << std::endl;

    return 0;
}