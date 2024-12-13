/*
The strategy here is simple and fast:
We store the number of stones as a dictionary, where the key is the stone and the value is the number of stones.
We then iterate over the dictionary, and for each stone, we evaluate the new stones that can be created.
We then update the dictionary with the new stones.
We repeat this process for the number of blinks.
Finally, we sum up the number of stones in the dictionary.
*/
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

std::vector<long long> inputToList(const std::string& input) {
    std::vector<long long> result;
    std::stringstream ss(input);
    long long num;
    while (ss >> num) {
        result.push_back(num);
    }
    return result;
}

std::map<long long, long long> listToDict(const std::vector<long long>& stoneList) {
    std::map<long long, long long> result;
    for (long long element : stoneList) {
        result[element]++;
    }
    return result;
}

std::vector<std::pair<long long, long long>> evalveDict(long long element) {
    if (element == 0) {
        return {{1, 1}};
    }
    
    std::string numStr = std::to_string(element);
    if (numStr.length() % 2 == 0) {
        int mid = numStr.length() / 2;
        long long firstHalf = std::stoll(numStr.substr(0, mid));
        long long secondHalf = std::stoll(numStr.substr(mid));
        return {{firstHalf, 1}, {secondHalf, 1}};
    } else {
        return {{2024LL * element, 1}};
    }
}

long long solve(const std::string& input, int nBlinks) {
    std::vector<long long> stones = inputToList(input);
    std::map<long long, long long> stonesDict = listToDict(stones);
    
    for (int i = 0; i < nBlinks; i++) {
        std::map<long long, long long> newStonesDict;
        
        for (const auto& pair : stonesDict) {
            auto evalvedStonesList = evalveDict(pair.first);
            
            for (const auto& evolved : evalvedStonesList) {
                newStonesDict[evolved.first] += evolved.second * pair.second;
            }
        }
        
        stonesDict = newStonesDict;
    }
    
    long long sum = 0;
    for (const auto& pair : stonesDict) {
        sum += pair.second;
    }
    return sum;
}

int main() {
    std::string test_input = readFile("test_input.txt");
    std::string input = readFile("input.txt");
    std::cout << "part_1_test: " << solve(test_input, 25) << std::endl;
    std::cout << "part_1:      " << solve(input, 25) << std::endl;
    std::cout << "part_2:      " << solve(input, 75) << std::endl;

    return 0;
}