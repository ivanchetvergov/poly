#pragma once

#include <Graph.h>
#include <memory>
#include <optional>
#include <vector>

namespace lab5 {

using namespace graph;

class Runner {
public:
    Runner() = default;
    explicit Runner(Graph* graph) : graph_(graph) {}

    void setGraph(Graph* graph) { graph_ = graph; }
    Graph* getGraph() { return graph_; }
    const Graph* getGraph() const { return graph_; }

    void checkEulerian();
    void checkHamiltonian();
    void solveTSP();

    const std::optional<std::vector<int>>& getLastEulerianCycle() const;
    const std::optional<std::vector<int>>& getLastHamiltonianCycle() const;
    const std::optional<std::vector<int>>& getLastTSPCycle() const;

private:
    Graph* graph_ = nullptr;
    std::optional<std::vector<int>> lastEulerianCycle_;
    std::optional<std::vector<int>> lastHamiltonianCycle_;
    std::optional<std::vector<int>> lastTSPCycle_;
};

} // namespace lab5
