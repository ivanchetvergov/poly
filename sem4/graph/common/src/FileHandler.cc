#include "FileHandler.h"

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

}  // namespace graph
