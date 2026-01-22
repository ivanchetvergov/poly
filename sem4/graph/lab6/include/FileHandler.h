#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace dict {

using VectorOfKeyValuePair = std::vector<std::pair<std::string, int>>;
using VectorOfTreeStructure = std::vector<std::tuple<std::string, std::string, std::string>>;

class FileHandler {
public:
    static bool saveToFile(std::string const& filename, std::string const& content);
    static bool loadFromFile(std::string const& filename, std::string& content);
    static bool appendToFile(std::string const& filename, std::string const& content);

    static bool saveKeyValuePairs(std::string const& filename, VectorOfKeyValuePair const& pairs);
    static bool loadKeyValuePairs(std::string const& filename, VectorOfKeyValuePair& pairs);

    static bool saveTreeStructure(std::string const& filename, VectorOfTreeStructure const& nodes);
};

}  // namespace dict
