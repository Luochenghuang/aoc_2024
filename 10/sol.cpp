/*
The strategy here is simple:
For part 1, we first find all the trail heads (0) in the matrix.
Then for each trail head, we keep track of leads that might be able to reach the 9.
We then step through the values from 1 to 9, finding new leads in the neighbours of the current leads.
When we reach 9, we return the set of unique leads.
Finally we sum up the size of the set of unique leads for each trail head.

For part 2, we do the same thing, except we return the list of leads instead of the set of unique leads.
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include <sstream>
#include <algorithm>

using Matrix = std::vector<std::vector<int>>;
using Position = std::pair<int, int>;
using PositionList = std::vector<Position>;

Matrix inputToMatrix(const std::string& input) {
    Matrix matrix;
    std::istringstream stream(input);
    std::string line;
    
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        std::vector<int> row;
        for (char c : line) {
            if (std::isdigit(c)) {
                row.push_back(c - '0');
            }
        }
        matrix.push_back(row);
    }
    return matrix;
}

PositionList getPositionListOfTrailHeads(const Matrix& matrix) {
    PositionList positions;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == 0) {
                positions.push_back({i, j});
            }
        }
    }
    return positions;
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

class Trail {
public:
    Trail(const Matrix& map, const Position& starting_pos) 
        : map(map), starting_pos(starting_pos), current_value(0) {
        current_heads_pos_list.push_back(starting_pos);
    }

    PositionList find9Positions(bool part_1 = true) {
        while (current_value < 9) {
            nextStep();
        }
        if (part_1) {
            std::set<Position> unique_positions(current_heads_pos_list.begin(), current_heads_pos_list.end());
            return PositionList(unique_positions.begin(), unique_positions.end());
        } else {
            return current_heads_pos_list;
        }
    }

private:
    void nextStep() {
        current_value++;
        PositionList new_pos_list;
        
        for (const auto& pos : current_heads_pos_list) {
            for (const auto& neighbour_pos : getNeighbours(pos)) {
                if (positionIsValuedAt(neighbour_pos, current_value)) {
                    new_pos_list.push_back(neighbour_pos);
                }
            }
        }
        current_heads_pos_list = new_pos_list;
    }

    PositionList getNeighbours(const Position& position) {
        PositionList neighbours;
        const std::vector<std::pair<int, int>> deltas = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        
        for (const auto& delta : deltas) {
            int new_row = position.first + delta.first;
            int new_col = position.second + delta.second;
            
            if (new_row >= 0 && new_row < map.size() && 
                new_col >= 0 && new_col < map[0].size()) {
                neighbours.push_back({new_row, new_col});
            }
        }
        return neighbours;
    }

    bool positionIsValuedAt(const Position& position, int desired_value) {
        return map[position.first][position.second] == desired_value;
    }

    Matrix map;
    Position starting_pos;
    int current_value;
    PositionList current_heads_pos_list;
};

int solve(const std::string& input, bool part_1 = true) {
    Matrix matrix = inputToMatrix(input);
    PositionList position_list = getPositionListOfTrailHeads(matrix);
    int total_score = 0;

    for (const auto& position : position_list) {
        Trail trail(matrix, position);
        auto trail_9_positions = trail.find9Positions(part_1);
        total_score += trail_9_positions.size();
    }
    return total_score;
}

int part1(const std::string& input) {
    return solve(input, true);
}

int part2(const std::string& input) {
    return solve(input, false);
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