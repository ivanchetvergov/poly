#pragma once

#include <Graph.h>

namespace lab5 {

using graph::Graph;

class Runner {
public:
    Runner() = default;

    void runCheckEulerian(Graph& graph);
    void runCheckHamiltonian(Graph& graph);
    void runSolveTSP(Graph const& graph);
};

}  // namespace lab5
