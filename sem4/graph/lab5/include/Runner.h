#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <Graph.h>

namespace lab5 {

using graph::Graph;

class Runner {
public:
    Runner() = default;

    void checkEulerian(Graph& graph);
    void checkHamiltonian(Graph& graph);
    void solveTSP(Graph const& graph);

    std::optional<std::vector<int>> const& getLastEulerianCycle() const;
    std::optional<std::vector<int>> const& getLastHamiltonianCycle() const;
    std::optional<std::vector<int>> const& getLastTSPCycle() const;

    std::vector<std::pair<int, int>> const& getLastEulerianAddedEdges() const {
        return last_eulerian_added_edges_;
    }
    std::vector<std::pair<int, int>> const& getLastHamiltonianAddedEdges() const {
        return last_hamiltonian_added_edges_;
    }

private:
    std::optional<std::vector<int>> last_eulerian_cycle_;
    std::optional<std::vector<int>> last_hamiltonian_cycle_;
    std::optional<std::vector<int>> last_tsp_cycle_;
    std::vector<std::pair<int, int>> last_eulerian_added_edges_;
    std::vector<std::pair<int, int>> last_hamiltonian_added_edges_;
};

}  // namespace lab5
