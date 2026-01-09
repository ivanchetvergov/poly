// PathCounter.cc
#include "PathCounter.h"

namespace graph {

PathCounter::PathCounter(Graph const& graph) : m_graph_(graph) {}

int PathCounter::getPathCount(int from, int to) {
    return getAllPaths(from, to).size();
}

std::vector<std::vector<int>> PathCounter::getAllPaths(int from, int to) {
    if (!m_graph_.hasVertex(from) || !m_graph_.hasVertex(to)) {
        return {};
    }

    std::vector<std::vector<int>> all_paths;
    std::vector<int> current_path;
    std::unordered_map<int, bool> visited;

    getAllPathsRecursive(from, to, visited, current_path, all_paths);
    return all_paths;
}

bool PathCounter::hasPath(int from, int to) {
    return getPathCount(from, to) > 0;
}

void PathCounter::getAllPathsRecursive(
    int current, int target, std::unordered_map<int, bool>& visited, std::vector<int>& currentPath,
    std::vector<std::vector<int>>& allPaths) {  // NOLINT(misc-no-recursion)
    currentPath.push_back(current);
    visited[current] = true;

    if (current == target) {
        allPaths.push_back(currentPath);
    } else {
        for (auto const& [neighborId, weight] : m_graph_.neighbors(current)) {
            if (!visited[neighborId]) {
                getAllPathsRecursive(neighborId, target, visited, currentPath, allPaths);
            }
        }
    }

    currentPath.pop_back();
    visited[current] = false;
}

}  // namespace graph
