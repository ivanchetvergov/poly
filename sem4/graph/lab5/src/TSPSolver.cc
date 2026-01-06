#include "TSPSolver.h"
#include <algorithm>

namespace graph {

std::vector<TSPSolver::Solution> TSPSolver::findAllCycles(int maxResults) {
    std::vector<Solution> solutions;
    auto vertices = m_graph.vertexIds();

    if (vertices.empty()) return solutions;

    std::vector<int> path = {vertices[0]};
    std::vector<bool> visited(vertices.size(), false);
    visited[0] = true;

    findAllHamiltonianCycles(solutions, path, visited, 0.0, maxResults);

    std::sort(solutions.begin(), solutions.end(),
        [](const Solution& a, const Solution& b) {
            return a.cost < b.cost;
        });

    return solutions;
}

TSPSolver::Solution TSPSolver::findBestCycle() {
    auto allCycles = findAllCycles(1000);

    if (allCycles.empty()) {
        return Solution{};
    }

    return allCycles[0];
}

void TSPSolver::findAllHamiltonianCycles(
    std::vector<Solution>& solutions,
    std::vector<int>& path,
    std::vector<bool>& visited,
    const double currentCost,
    const int maxResults)
{
    if (solutions.size() >= static_cast<size_t>(maxResults)) {
        return;
    }

    auto vertices = m_graph.vertexIds();
    int n = static_cast<int>(vertices.size());

    if (static_cast<int>(path.size()) == n) {
        auto edgeWeight = m_graph.getEdgeWeight(path.back(), path[0]);
        if (edgeWeight.has_value()) {
            double totalCost = currentCost + edgeWeight.value();
            auto cyclePath = path;
            cyclePath.push_back(path[0]);
            solutions.push_back({cyclePath, totalCost, true});
        }
        return;
    }

    for (int i = 0; i < n; ++i) {
        int v = vertices[i];
        if (!visited[i]) {
            auto edgeWeight = m_graph.getEdgeWeight(path.back(), v);
            if (edgeWeight.has_value()) {
                path.push_back(v);
                visited[i] = true;

                findAllHamiltonianCycles(solutions, path, visited,
                    currentCost + edgeWeight.value(), maxResults);

                path.pop_back();
                visited[i] = false;
            }
        }
    }
}

} // namespace graph
