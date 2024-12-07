#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

std::vector<std::string> split(const std::string& str, const std::string& delim) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        tokens.push_back(str.substr(prev, pos - prev));
        prev = pos + delim.length();
    } while (pos != std::string::npos);
    return tokens;
}

std::vector<int> parse_numbers(const std::string& str) {
    std::vector<int> numbers;
    std::istringstream iss(str);
    int num;
    while (iss >> num) {
        numbers.push_back(num);
    }
    return numbers;
}

void generate_combinations(std::vector<std::vector<std::string>>& all_combinations,
                         const std::vector<std::string>& possible_operators,
                         std::vector<std::string>& current,
                         size_t length) {
    if (current.size() == length) {
        all_combinations.push_back(current);
        return;
    }
    for (const auto& op : possible_operators) {
        current.push_back(op);
        generate_combinations(all_combinations, possible_operators, current, length);
        current.pop_back();
    }
}

bool can_equate(long long target, const std::vector<int>& rest, int part_num) {
    std::vector<std::string> possible_operators;
    if (part_num == 1) {
        possible_operators = {"+", "*"};
    } else {
        possible_operators = {"+", "*", "||"};
    }
    
    std::vector<std::vector<std::string>> all_combinations;
    std::vector<std::string> current;
    generate_combinations(all_combinations, possible_operators, current, rest.size() - 1);
    
    for (const auto& ops : all_combinations) {
        long long current_value = rest[0];
        for (size_t i = 0; i < ops.size(); ++i) {
            if (ops[i] == "||") {
                std::string combined = std::to_string(current_value) + std::to_string(rest[i + 1]);
                current_value = std::stoll(combined);
            } else if (ops[i] == "+") {
                current_value += rest[i + 1];
            } else if (ops[i] == "*") {
                current_value *= rest[i + 1];
            }
        }
        if (current_value == target) {
            return true;
        }
    }
    return false;
}

long solve(const std::string& input, int part_num) {
    long tally = 0;
    std::istringstream iss(input);
    std::string line;
    
    while (std::getline(iss, line)) {
        auto parts = split(line, ": ");
        long long target = std::stoll(parts[0]);
        auto rest = parse_numbers(parts[1]);
        
        if ((part_num == 1 && can_equate(target, rest, 1)) ||
            (part_num == 2 && can_equate(target, rest, 2))) {
            tally += target;
        }
    }
    return tally;
}

int main() {
    std::string test_input = readFile("test_input.txt");
    std::string input = readFile("input.txt");

    std::cout << "part_1_test: " << solve(test_input, 1) << std::endl;
    std::cout << "part_1:      " << solve(input, 1) << std::endl;
    std::cout << "part_2_test: " << solve(test_input, 2) << std::endl;
    std::cout << "part_2:      " << solve(input, 2) << std::endl;

    return 0;
}