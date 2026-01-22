#pragma once

#include "Graph.h"
#include "../../lab3/include/FlowNetwork.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <optional>

namespace graph {

using VectorOfKeyValuePair = std::vector<std::pair<std::string, int>>;
using VectorOfTreeStructure = std::vector<std::tuple<std::string, std::string, std::string>>;
using SparseMatrix = std::vector<std::vector<std::optional<double>>>;

template <typename T>
using TMatrix = std::vector<std::vector<T>>;

// enum class MatrixType { Adjacency, Weight, Capacity, Cost };

class FileHandler {
public:
    // General file operations
    static bool saveToFile(std::string const& filename, std::string const& content);
    static bool loadFromFile(std::string const& filename, std::string& content);
    static bool appendToFile(std::string const& filename, std::string const& content);

    // Key-value and structured data
    static bool saveKeyValuePairs(std::string const& filename, VectorOfKeyValuePair const& pairs);
    static bool loadKeyValuePairs(std::string const& filename, VectorOfKeyValuePair& pairs);
    static bool saveTreeStructure(std::string const& filename, VectorOfTreeStructure const& nodes);

    // Graph operations
    static bool saveGraphEdges(std::string const& filename, std::vector<std::tuple<int, int, double>> const& edges);
    static bool saveGraph(std::string const& filename, Graph const& graph);
    static bool saveAdjacencyMatrix(std::string const& filename, Graph const& graph);
    static bool saveWeightMatrix(std::string const& filename, Graph const& graph);
    static bool saveDistanceMatrix(std::string const& filename, SparseMatrix const& matrix);

    // FlowNetwork operations
    static bool saveCapacityMatrix(std::string const& filename, FlowNetwork const& network);
    static bool saveCostMatrix(std::string const& filename, FlowNetwork const& network);
    static bool saveFlowNetwork(std::string const& filename, FlowNetwork const& network);

    // Paths and colors
    static bool savePath(std::string const& filename, std::vector<int> const& path);
    static bool savePaths(std::string const& filename, std::vector<std::vector<int>> const& paths);
    static bool saveColors(std::string const& filename, std::vector<int> const& vertices, std::vector<int> const& colors);

    // Matrices and utilities
    static bool saveMatrix(std::string const& filename, std::vector<std::vector<double>> const& matrix);

    static bool saveAddedEdges(std::string const& filename, std::vector<std::pair<int, int>> const& edges);

private:
    template <typename T, typename GraphOrNetwork, typename Getter>
    static bool saveSquareMatrix(std::string const& filename,
                                 GraphOrNetwork const& graph_or_network,
                                 Getter getter)
    {
        auto sorted_vertex_ids = graph_or_network.vertexIds();
        std::sort(sorted_vertex_ids.begin(), sorted_vertex_ids.end());
        auto matrix = CollectionUtils::makeSquareMatrix<T>(sorted_vertex_ids, getter);
        return saveMatrix(filename, matrix);
    }
    template <typename T>
    static bool saveMatrix(std::string const& filename, TMatrix<T> const& matrix) {
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
};

}  // namespace graph
