// PathCounter.h
#pragma once

#include <unordered_map>

#include <Graph.h>

namespace graph {

using PathMatrix = std::vector<std::vector<int>>;

class PathCounter {
public:
    explicit PathCounter(Graph const& graph);

    [[nodiscard]] int getPathCount(int from, int to);
    [[nodiscard]] PathMatrix getAllPaths(int from, int to);

private:
    Graph const& m_graph_;

    void backtrackAllPaths(int current, int target, std::unordered_map<int, bool>& visited,
                              std::vector<int>& currentPath, PathMatrix& allPaths);
};

}  // namespace graph

