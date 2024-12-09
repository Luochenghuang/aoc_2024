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

std::vector<std::pair<int, int>> parseStrToPairs(const std::string& input) {
    std::vector<std::pair<int, int>> pairs;
    std::pair<int, int> pair;
    
    for (size_t i = 0; i < input.length(); i++) {
        int num = input[i] - '0';
        if (i % 2 == 0) {
            pair.first = num;
            if (i == input.length() - 1) {
                pair.second = 0;
                pairs.push_back(pair);
            }
        } else {
            pair.second = num;
            pairs.push_back(pair);
        }
    }
    return pairs;
}

std::vector<std::vector<int>> pairsToBlocks(const std::vector<std::pair<int, int>>& pairs) {
    std::vector<std::vector<int>> blocks;
    for (size_t i = 0; i < pairs.size(); i++) {
        std::vector<int> block;
        for (int j = 0; j < pairs[i].first; j++) {
            block.push_back(i);
        }
        for (int j = 0; j < pairs[i].second; j++) {
            block.push_back(-1);
        }
        blocks.push_back(block);
    }
    return blocks;
}

std::vector<int> expandIntoBlocks(const std::string& input) {
    auto pairs = parseStrToPairs(input);
    auto blocks = pairsToBlocks(pairs);
    
    std::vector<int> result;
    for (const auto& block : blocks) {
        result.insert(result.end(), block.begin(), block.end());
    }
    return result;
}

std::vector<int> moveOneBlock(const std::vector<int>& blocks) {
    std::vector<int> newBlocks(blocks.begin(), blocks.end() - 1);
    auto it = std::find(newBlocks.begin(), newBlocks.end(), -1);
    if (it != newBlocks.end()) {
        *it = blocks.back();
    }
    return newBlocks;
}

unsigned long long checksum(const std::vector<int>& blocks) {
    unsigned long long sum = 0;
    for (size_t i = 0; i < blocks.size(); i++) {
        sum += (unsigned long long)blocks[i] * i;
    }
    return sum;
}

std::pair<size_t, size_t> findContiguousFile(const std::vector<int>& blocks, int value) {
    size_t start_idx = 0;
    size_t end_idx = 0;
    bool found_start = false;
    
    for (size_t i = 0; i < blocks.size(); i++) {
        if (blocks[i] == value) {
            if (!found_start) {
                start_idx = i;
                found_start = true;
            }
            end_idx = i;
        }
    }
    return {start_idx, end_idx};
}

std::vector<int> changeFileToNone(std::vector<int> blocks, int value) {
    for (auto& block : blocks) {
        if (block == value) {
            block = -1;
        }
    }
    return blocks;
}

size_t findFirstContiguousFreeSpace(const std::vector<int>& blocks, size_t free_space_size) {
    size_t free_space_count = 0;
    for (size_t i = 0; i < blocks.size(); i++) {
        if (blocks[i] == -1) {
            free_space_count++;
        } else {
            free_space_count = 0;
        }
        if (free_space_count == free_space_size) {
            return i - free_space_size + 1;
        }
    }
    return blocks.size();
}

unsigned long long part1(const std::string& input) {
    auto blocks = expandIntoBlocks(input);
    while (std::find(blocks.begin(), blocks.end(), -1) != blocks.end()) {
        blocks = moveOneBlock(blocks);
    }
    return checksum(blocks);
}

unsigned long long part2(const std::string& input) {
    auto blocks = expandIntoBlocks(input);
    
    int max_value = -1;
    for (int block : blocks) {
        if (block > max_value) max_value = block;
    }
    
    for (int value = max_value; value >= 0; value--) {
        auto [start_idx, end_idx] = findContiguousFile(blocks, value);
        size_t free_space_size = end_idx - start_idx + 1;
        
        std::vector<int> temp_blocks(blocks.begin(), blocks.begin() + start_idx);
        size_t free_space_idx = findFirstContiguousFreeSpace(temp_blocks, free_space_size);
        
        if (free_space_idx == temp_blocks.size()) continue;
        
        blocks = changeFileToNone(blocks, value);
        for (size_t i = 0; i < free_space_size; i++) {
            blocks[free_space_idx + i] = value;
        }
    }
    
    for (auto& block : blocks) {
        if (block == -1) block = 0;
    }
    
    return checksum(blocks);
}

int main() {
    std::string test_input = readFile("test_input.txt");
    std::string input = readFile("input.txt");
    std::cout << "part_1_test: " << part1(test_input) << std::endl;
    std::cout << "part_1:      " << part1(input) << std::endl;
    std::cout << "part_2_test: " << part2(test_input) << std::endl;
    std::cout << "part_2:      " << part2(input) << std::endl;

    return 0;
}