// PathCounter.h
#pragma once

#include <unordered_map>

#include <Graph.h>

namespace graph {

class PathCounter {
public:
    explicit PathCounter(Graph const& graph);

    [[nodiscard]] int getPathCount(int from, int to);
    [[nodiscard]] std::vector<std::vector<int>> getAllPaths(int from, int to);
    [[nodiscard]] bool hasPath(int from, int to);

private:
    Graph const& m_graph_;

    void getAllPathsRecursive(int current, int target, std::unordered_map<int, bool>& visited,
                              std::vector<int>& currentPath,
                              std::vector<std::vector<int>>& allPaths);
};

}  // namespace graph
