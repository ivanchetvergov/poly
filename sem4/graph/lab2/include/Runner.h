#pragma once

#include <Graph.h>
#include <memory>

namespace lab2 {

using graph::Graph;

class Runner {
public:
    Runner() = default;

    void runBFS(Graph const& graph);
    void runDijkstra(Graph const& graph);
    void runCompare(Graph const& graph);
};

}  // namespace lab2
