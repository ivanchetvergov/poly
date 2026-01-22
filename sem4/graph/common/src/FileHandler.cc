#include "FileHandler.h"

#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>

namespace graph {

bool FileHandler::saveToFile(std::string const& filename, std::string const& content) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    file << content;
    file.close();
    return true;
}

bool FileHandler::loadFromFile(std::string const& filename, std::string& content) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    std::string line;
    content.clear();
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    file.close();
    return true;
}

bool FileHandler::appendToFile(std::string const& filename, std::string const& content) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return false;
    file << content;
    file.close();
    return true;
}

bool FileHandler::saveKeyValuePairs(std::string const& filename, std::vector<std::pair<std::string, int>> const& pairs) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (auto const& [key, count] : pairs) {
        file << key << " " << count << "\n";
    }
    file.close();
    return true;
}

bool FileHandler::loadKeyValuePairs(std::string const& filename, std::vector<std::pair<std::string, int>>& pairs) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    pairs.clear();
    std::string key;
    int count;
    while (file >> key >> count) {
        pairs.emplace_back(key, count);
    }
    file.close();
    return true;
}

bool FileHandler::saveTreeStructure(std::string const& filename, std::vector<std::tuple<std::string, std::string, std::string>> const& nodes) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (auto const& [node, parent, color] : nodes) {
        file << node << " " << parent << " " << color << "\n";
    }
    file.close();
    return true;
}

bool FileHandler::saveGraphEdges(std::string const& filename, std::vector<std::tuple<int, int, double>> const& edges) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (auto const& [from, to, weight] : edges) {
        file << from << " " << to << " " << weight << "\n";
    }
    file.close();
    return true;
}

bool FileHandler::savePath(std::string const& filename, std::vector<int> const& path) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (size_t i = 0; i < path.size(); ++i) {
        file << path[i];
        if (i + 1 < path.size()) file << " ";
    }
    file << "\n";
    file.close();
    return true;
}

bool FileHandler::savePaths(std::string const& filename, std::vector<std::vector<int>> const& paths) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    std::vector<std::vector<int>> non_empty_paths;
    for (auto const& path : paths) {
        if (!path.empty()) {
            non_empty_paths.push_back(path);
        }
    }
    file << non_empty_paths.size() << "\n";
    for (auto const& path : non_empty_paths) {
        for (size_t i = 0; i < path.size(); ++i) {
            file << path[i];
            if (i + 1 < path.size()) file << " ";
        }
        file << "\n";
    }
    file.close();
    return true;
}

bool FileHandler::saveColors(std::string const& filename, std::vector<int> const& vertices, std::vector<int> const& colors) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (size_t i = 0; i < colors.size(); ++i) {
        file << vertices[i] << " " << colors[i] << "\n";
    }
    file.close();
    return true;
}

bool FileHandler::saveMatrix(std::string const& filename, std::vector<std::vector<double>> const& matrix) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (auto const& row : matrix) {
        for (size_t j = 0; j < row.size(); ++j) {
            file << row[j];
            if (j + 1 < row.size()) file << " ";
        }
        file << "\n";
    }
    file.close();
    return true;
}

bool FileHandler::saveFlowNetwork(std::string const& filename, std::vector<std::tuple<int, int, double, double, double>> const& flows) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (auto const& [from, to, capacity, flow, cost] : flows) {
        file << from << " " << to << " " << capacity << " " << flow << " " << cost << "\n";
    }
    file.close();
    return true;
}

bool FileHandler::saveAddedEdges(std::string const& filename, std::vector<std::pair<int, int>> const& edges) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    for (auto const& [u, v] : edges) {
        file << u << " " << v << "\n";
    }
    file.close();
    return true;
}

}  // namespace graph
