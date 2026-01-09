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
    explicit Runner(Graph* graph) : graph_(graph) {}

    void setGraph(Graph* graph) { graph_ = graph; }
    Graph* getGraph() { return graph_; }
    Graph const* getGraph() const { return graph_; }

    void checkEulerian();
    void checkHamiltonian();
    void solveTSP();

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
    Graph* graph_ = nullptr;
    std::optional<std::vector<int>> last_eulerian_cycle_;
    std::optional<std::vector<int>> last_hamiltonian_cycle_;
    std::optional<std::vector<int>> last_tsp_cycle_;
    std::vector<std::pair<int, int>> last_eulerian_added_edges_;
    std::vector<std::pair<int, int>> last_hamiltonian_added_edges_;
};

}  // namespace lab5
