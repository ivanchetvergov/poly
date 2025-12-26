// PathCounter.h
#pragma once

#include <Graph.h>
#include <unordered_map>

namespace graph {

class PathCounter {
public:
    explicit PathCounter(const Graph& graph);
    
    [[nodiscard]] std::optional<int> countPaths(int from, int to);
    [[nodiscard]] bool hasPath(int from, int to);

private:
    const Graph& m_graph;
    std::unordered_map<int, std::unordered_map<int, int>> m_memo;
    
    int countPathsRecursive(int current, int target, std::unordered_map<int, bool>& visited);
};

} // namespace graph
