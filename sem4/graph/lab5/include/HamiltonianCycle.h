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

    [[nodiscard]] const std::vector<std::pair<int, int>>& getAddedEdges() const { return m_addedEdges; }

private:
    Graph& m_graph;
    std::vector<std::pair<int, int>> m_addedEdges;

    [[nodiscard]] bool hasHamiltonianCycle(std::vector<int>& path, std::vector<bool>& visited, const int pos);
};

} // namespace graph
