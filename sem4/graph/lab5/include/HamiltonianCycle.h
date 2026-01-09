#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <Graph.h>

namespace graph {

class HamiltonianCycle {
public:
    explicit HamiltonianCycle(Graph& graph) : m_graph_(graph) {}

    [[nodiscard]] bool isHamiltonian();
    [[nodiscard]] std::optional<std::vector<int>> findCycle();

    void makeHamiltonian();

    [[nodiscard]] std::vector<std::pair<int, int>> const& getAddedEdges() const {
        return m_added_edges_;
    }

private:
    Graph& m_graph_;
    std::vector<std::pair<int, int>> m_added_edges_;

    [[nodiscard]] bool hasHamiltonianCycle(std::vector<int>& path, std::vector<bool>& visited,
                                           int pos);
};

}  // namespace graph
