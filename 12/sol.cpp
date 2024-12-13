/*
For this puzzle, the solutions to part 1 and part 2 are very different.
* Part 1:
We first find all the congruent pieces and find their locations.
We then iterate over the locations, and for each location, we find the perimeter of the congruent piece
given the number of neighbours.
Finally, we tally up the scores.

* Part 2:
To calculate the perimeter this time, we utilize a useful fact:
The number of sides of a polygon is equal to the number of vertices.
Using this fact, we now transformed the problem into finding the number of vertices of a polygon.
We iterate over the congruent pieces, and for each piece:
1. We generate the shape map of the piece.
2. We then iterate over the shape map, using overlapping 4x4 tiles. 1 represents the existence of a the patch.
    - If the tile sums up to 1 or 3, there is 1 vertex.
    - If the tile sums up to 0 or 4, there is 0 vertices.
    - If the tile sums up to 2, there is 2 vertices only if the values are diagnal; 0 otherwise.
Finally, we sum up the scores.
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include <sstream>
#include <queue>
#include <climits>

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

class Map {
private:
    std::vector<std::vector<char>> map;
    std::vector<std::vector<int>> visited_map;

public:
    Map(const std::vector<std::vector<char>>& input_map) : 
        map(input_map),
        visited_map(input_map.size(), std::vector<int>(input_map[0].size(), 0)) {}

    std::vector<std::pair<int, int>> findNeighbours(std::pair<int, int> location) {
        std::vector<std::pair<int, int>> neighbours;
        std::vector<std::pair<int, int>> delta_loc = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        
        for (const auto& delta : delta_loc) {
            int new_x = location.first + delta.first;
            int new_y = location.second + delta.second;
            if (new_x >= 0 && new_x < map.size() && new_y >= 0 && new_y < map[0].size()) {
                neighbours.push_back({new_x, new_y});
            }
        }
        return neighbours;
    }

    std::set<std::pair<int, int>> exploreAndFindCongruentLocations(std::pair<int, int> startLocation) {
        std::queue<std::pair<int, int>> leads;
        std::set<std::pair<int, int>> shape_locations;
        leads.push(startLocation);
        shape_locations.insert(startLocation);

        while (!leads.empty()) {
            auto current_location = leads.front();
            leads.pop();
            auto neighbours = findNeighbours(current_location);
            
            for (const auto& neighbour : neighbours) {
                if (map[neighbour.first][neighbour.second] == map[current_location.first][current_location.second] 
                    && shape_locations.find(neighbour) == shape_locations.end()) {
                    leads.push(neighbour);
                    shape_locations.insert(neighbour);
                }
            }
        }
        return shape_locations;
    }

    int findPerimeterLength(const std::set<std::pair<int, int>>& shapeLocations) {
        int perimeter_length = 0;
        std::vector<std::pair<int, int>> delta_loc = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        
        for (const auto& location : shapeLocations) {
            perimeter_length += 4;
            for (const auto& delta : delta_loc) {
                std::pair<int, int> new_loc = {location.first + delta.first, location.second + delta.second};
                if (shapeLocations.find(new_loc) != shapeLocations.end()) {
                    perimeter_length--;
                }
            }
        }
        return perimeter_length;
    }

    void markVisited(const std::set<std::pair<int, int>>& shapeLocations) {
        for (const auto& location : shapeLocations) {
            visited_map[location.first][location.second] = 1;
        }
    }

    std::pair<int, int> findNextLocationToVisit() {
        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[0].size(); j++) {
                if (visited_map[i][j] == 0) {
                    return {i, j};
                }
            }
        }
        return {-1, -1};
    }

    std::vector<std::vector<int>> generateShapeMap(const std::set<std::pair<int, int>>& shapeLocations) {
        int x_min = INT_MAX, x_max = INT_MIN;
        int y_min = INT_MAX, y_max = INT_MIN;
        
        for (const auto& location : shapeLocations) {
            x_min = std::min(x_min, location.first);
            x_max = std::max(x_max, location.first);
            y_min = std::min(y_min, location.second);
            y_max = std::max(y_max, location.second);
        }
        
        std::vector<std::vector<int>> shape_map(y_max - y_min + 3, 
            std::vector<int>(x_max - x_min + 3, 0));
            
        for (const auto& location : shapeLocations) {
            shape_map[location.second - y_min + 1][location.first - x_min + 1] = 1;
        }
        
        return shape_map;
    }
};

class PerimeterTracer {
private:
    std::vector<std::vector<int>> map;

public:
    PerimeterTracer(const std::vector<std::vector<int>>& input_map) : map(input_map) {}

    std::vector<std::vector<int>> getNeighbourTiles(int cx, int cy) {
        std::vector<std::vector<int>> tiles(2, std::vector<int>(2));
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                if (cx + i - 1 >= 0 && cx + i - 1 < map.size() &&
                    cy + j - 1 >= 0 && cy + j - 1 < map[0].size()) {
                    tiles[i][j] = map[cx + i - 1][cy + j - 1];
                }
            }
        }
        return tiles;
    }

    int findNVertices(const std::vector<std::vector<int>>& tiles) {
        int sum = 0;
        for (const auto& row : tiles) {
            for (int val : row) {
                sum += val;
            }
        }

        if (sum == 1 || sum == 3) return 1;
        if (sum == 0 || sum == 4) return 0;
        if (sum == 2) {
            if ((tiles[0][0] == tiles[1][1]) || (tiles[1][0] == tiles[0][1])) {
                return 2;
            }
            return 0;
        }
        throw std::runtime_error("Impossible tile configuration");
    }

    int findAllVertices() {
        int vertices = 0;
        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[0].size(); j++) {
                auto tiles = getNeighbourTiles(i, j);
                vertices += findNVertices(tiles);
            }
        }
        return vertices;
    }
};

std::vector<std::vector<char>> inputToMap(const std::string& input) {
    std::vector<std::vector<char>> result;
    std::stringstream ss(input);
    std::string line;
    
    while (std::getline(ss, line)) {
        if (!line.empty()) {
            result.push_back(std::vector<char>(line.begin(), line.end()));
        }
    }
    return result;
}

int part1(const std::string& input) {
    auto map = Map(inputToMap(input));
    int tally = 0;
    
    while (true) {
        auto location = map.findNextLocationToVisit();
        if (location.first == -1) break;
        
        auto shape_locations = map.exploreAndFindCongruentLocations(location);
        int perimeter_length = map.findPerimeterLength(shape_locations);
        map.markVisited(shape_locations);
        tally += perimeter_length * shape_locations.size();
    }
    return tally;
}

int part2(const std::string& input) {
    auto map = Map(inputToMap(input));
    int tally = 0;
    
    while (true) {
        auto location = map.findNextLocationToVisit();
        if (location.first == -1) break;
        
        auto shape_locations = map.exploreAndFindCongruentLocations(location);
        auto shape_map = map.generateShapeMap(shape_locations);
        auto perimeter_tracer = PerimeterTracer(shape_map);
        int n_vertices = perimeter_tracer.findAllVertices();
        map.markVisited(shape_locations);
        tally += n_vertices * shape_locations.size();
    }
    return tally;
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