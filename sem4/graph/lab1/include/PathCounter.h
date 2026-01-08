// PathCounter.h
#pragma once

#include <Graph.h>
#include <unordered_map>

namespace graph {

class PathCounter {
public:
    explicit PathCounter(const Graph& graph);

    [[nodiscard]] int getPathCount(int from, int to);
    [[nodiscard]] std::vector<std::vector<int>> getAllPaths(int from, int to);
    [[nodiscard]] bool hasPath(int from, int to);

private:
    const Graph& m_graph;

    void getAllPathsRecursive(int current, int target, std::unordered_map<int, bool>& visited, std::vector<int>& currentPath, std::vector<std::vector<int>>& allPaths);
};

} // namespace graph
