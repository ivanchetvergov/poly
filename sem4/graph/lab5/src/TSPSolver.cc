#include "TSPSolver.h"

#include <algorithm>
#include <utility>

namespace graph {

std::vector<TSPSolver::Solution> TSPSolver::findAllCycles(int maxResults) {
    std::vector<Solution> solutions;
    auto vertices = m_graph_.vertexIds();

    if (vertices.empty())
        return solutions;

    std::vector<int> path = {vertices[0]};
    std::vector<bool> visited(vertices.size(), false);
    visited[0] = true;

    findAllHamiltonianCycles(solutions, path, visited, 0.0, maxResults);

    std::sort(solutions.begin(), solutions.end(),
              [](Solution const& a, Solution const& b) { return a.cost < b.cost; });

    return solutions;
}

TSPSolver::Solution TSPSolver::findBestCycle() {
    auto all_cycles = findAllCycles(1000);

    if (all_cycles.empty()) {
        return Solution{};
    }

    return all_cycles[0];
}

void TSPSolver::findAllHamiltonianCycles(std::vector<Solution>& solutions, std::vector<int>& path,
                                         std::vector<bool>& visited, double const currentCost,
                                         int const maxResults) {
    if (solutions.size() >= static_cast<size_t>(maxResults)) {
        return;
    }

    auto vertices = m_graph_.vertexIds();
    int n = static_cast<int>(vertices.size());

    if (std::cmp_equal(path.size(), n)) {
        auto edge_weight = m_graph_.getEdgeWeight(path.back(), path[0]);
        if (edge_weight.has_value()) {
            double total_cost = currentCost + edge_weight.value();
            auto cycle_path = path;
            cycle_path.push_back(path[0]);
            solutions.push_back({cycle_path, total_cost, true});
        }
        return;
    }

    for (int i = 0; i < n; ++i) {
        int v = vertices[i];
        if (!visited[i]) {
            auto edge_weight = m_graph_.getEdgeWeight(path.back(), v);
            if (edge_weight.has_value()) {
                path.push_back(v);
                visited[i] = true;

                findAllHamiltonianCycles(solutions, path, visited, currentCost + edge_weight.value(),
                                         maxResults);

                path.pop_back();
                visited[i] = false;
            }
        }
    }
}

}  // namespace graph
