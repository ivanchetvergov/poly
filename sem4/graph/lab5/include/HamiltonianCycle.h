#pragma once

#include <Graph.h>
#include <memory>
#include <optional>
#include <vector>

namespace graph {

class HamiltonianCycle {
public:
    explicit HamiltonianCycle(Graph& graph) : m_graph(graph) {}

    [[nodiscard]] bool isHamiltonian();
    [[nodiscard]] std::optional<std::vector<int>> findCycle();

    void makeHamiltonian();

private:
    Graph& m_graph;

    [[nodiscard]] bool hasHamiltonianCycle(std::vector<int>& path, std::vector<bool>& visited, const int pos);
};

} // namespace graph
