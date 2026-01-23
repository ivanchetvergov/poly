#pragma once

#include "Graph.h"
#include "ShimbellMethod.h"
#include "PathCounter.h"

#include <memory>
#include <optional>
#include <vector>

#include <Generator.h>

namespace lab1 {

using graph::Graph;
using graph::Generator;

class Runner {
public:
    Runner() = default;

    std::unique_ptr<Graph> generateGraph();
    void runShimbellMethod(Graph const& graph);
    void runPathsMethod(Graph const& graph);
    void runVisualizeAdjacencyMatrix(Graph const& graph);
    void runVisualizeWeightMatrix(Graph const& graph);

private:
    Generator gen_;
};

}  // namespace lab1
