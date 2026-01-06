#pragma once

#include <Graph.h>
#include <limits>
#include <vector>

namespace graph {

class TSPSolver {
public:
    struct Solution {
        std::vector<int> path;
        double cost{std::numeric_limits<double>::infinity()};
        bool found{false};
    };

    explicit TSPSolver(const Graph& graph) : m_graph(graph) {}

    [[nodiscard]] std::vector<Solution> findAllCycles(int maxResults = 100);
    [[nodiscard]] Solution findBestCycle();

private:
    const Graph& m_graph;

    void findAllHamiltonianCycles(
        std::vector<Solution>& solutions,
        std::vector<int>& path,
        std::vector<bool>& visited,
        const double currentCost,
        const int maxResults);
};

} // namespace graph
