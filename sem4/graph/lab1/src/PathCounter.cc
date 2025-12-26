// PathCounter.cc
#include "PathCounter.h"

namespace graph {

PathCounter::PathCounter(const Graph& graph)
    : m_graph(graph) {
}

std::optional<int> PathCounter::countPaths(int from, int to) {
    if (!m_graph.hasVertex(from) || !m_graph.hasVertex(to)) {
        return std::nullopt;
    }

    if (from == to) {
        return 1;  
    }

    m_memo.clear();

    std::unordered_map<int, bool> visited;
    return countPathsRecursive(from, to, visited);
}

bool PathCounter::hasPath(int from, int to) {
    auto paths = countPaths(from, to);
    return paths.has_value() && paths.value() > 0;
}

// алгоритм бектрекинга
int PathCounter::countPathsRecursive(
    int current, 
    int target, 
    std::unordered_map<int, bool>& visited
) {
    if (current == target) {
        return 1;
    }

    if (!visited[current] && m_memo[current].find(target) != m_memo[current].end()) {
        return m_memo[current][target];
    }

    visited[current] = true;

    int pathCount = 0;
    auto neighbors = m_graph.neighbors(current);

    for (const auto& [neighborId, weight] : neighbors) {
        if (!visited[neighborId]) {
            pathCount += countPathsRecursive(neighborId, target, visited);
        }
    }

    visited[current] = false;
    m_memo[current][target] = pathCount;

    return pathCount;
}

} // namespace graph
