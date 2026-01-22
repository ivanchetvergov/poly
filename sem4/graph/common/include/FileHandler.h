#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace graph {

class FileHandler {
public:
    static bool saveToFile(std::string const& filename, std::string const& content);
    static bool loadFromFile(std::string const& filename, std::string& content);
    static bool appendToFile(std::string const& filename, std::string const& content);

    static bool saveKeyValuePairs(std::string const& filename, std::vector<std::pair<std::string, int>> const& pairs);
    static bool loadKeyValuePairs(std::string const& filename, std::vector<std::pair<std::string, int>>& pairs);

    static bool saveTreeStructure(std::string const& filename, std::vector<std::tuple<std::string, std::string, std::string>> const& nodes);
};

}  // namespace graph
