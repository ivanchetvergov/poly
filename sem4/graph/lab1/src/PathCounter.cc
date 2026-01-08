// PathCounter.cc
#include "PathCounter.h"

namespace graph {

PathCounter::PathCounter(const Graph& graph)
    : m_graph(graph) {
}

int PathCounter::getPathCount(int from, int to) {
    return getAllPaths(from, to).size();
}

std::vector<std::vector<int>> PathCounter::getAllPaths(int from, int to) {
    if (!m_graph.hasVertex(from) || !m_graph.hasVertex(to)) {
        return {};
    }

    std::vector<std::vector<int>> allPaths;
    std::vector<int> currentPath;
    std::unordered_map<int, bool> visited;

    getAllPathsRecursive(from, to, visited, currentPath, allPaths);
    return allPaths;
}

bool PathCounter::hasPath(int from, int to) {
    return getPathCount(from, to) > 0;
}

void PathCounter::getAllPathsRecursive(
    int current,
    int target,
    std::unordered_map<int, bool>& visited,
    std::vector<int>& currentPath,
    std::vector<std::vector<int>>& allPaths
) {
    currentPath.push_back(current);
    visited[current] = true;

    if (current == target) {
        allPaths.push_back(currentPath);
    } else {
        for (const auto& [neighborId, weight] : m_graph.neighbors(current)) {
            if (!visited[neighborId]) {
                getAllPathsRecursive(neighborId, target, visited, currentPath, allPaths);
            }
        }
    }

    currentPath.pop_back();
    visited[current] = false;
}

} // namespace graph
