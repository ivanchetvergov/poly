#pragma once

#include <limits>
#include <vector>

#include <Graph.h>

namespace graph {

class TSPSolver {
public:
    struct Solution {
        std::vector<int> path;
        double cost{std::numeric_limits<double>::infinity()};
        bool found{false};
    };

    explicit TSPSolver(Graph const& graph) : m_graph_(graph) {}

    [[nodiscard]] std::vector<Solution> findAllCycles(int maxResults = 100);
    [[nodiscard]] Solution findBestCycle();

private:
    Graph const& m_graph_;

    void findAllHamiltonianCycles(std::vector<Solution>& solutions, std::vector<int>& path,
                                  std::vector<bool>& visited, double currentCost, int maxResults);
};

}  // namespace graph
