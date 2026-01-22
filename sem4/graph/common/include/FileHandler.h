#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

namespace graph {

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

    static bool saveGraphEdges(std::string const& filename, std::vector<std::tuple<int, int, double>> const& edges);

    static bool savePath(std::string const& filename, std::vector<int> const& path);
    static bool savePaths(std::string const& filename, std::vector<std::vector<int>> const& paths);

    static bool saveColors(std::string const& filename, std::vector<int> const& vertices, std::vector<int> const& colors);

    static bool saveFlowNetwork(std::string const& filename, std::vector<std::tuple<int, int, double, double, double>> const& flows);

    static bool saveMatrix(std::string const& filename, std::vector<std::vector<double>> const& matrix);

    static bool saveAddedEdges(std::string const& filename, std::vector<std::pair<int, int>> const& edges);

};

}  // namespace graph
