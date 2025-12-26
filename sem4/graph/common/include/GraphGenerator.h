#pragma once

#include "Graph.h"

namespace graph {

[[nodiscard]] std::unique_ptr<Graph> generateRandomTree(int numVertices, int shape = 3, double rate = 1.5);
[[nodiscard]] std::unique_ptr<Graph> generateRandomGraph(int numVertices, int numEdges);

} // namespace graph
