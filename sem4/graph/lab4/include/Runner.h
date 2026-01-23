#pragma once

#include "Graph.h"
#include "GraphCombinatorics.h"
#include "KirchhoffTheorem.h"

#include <memory>

namespace lab4 {

using graph::Graph;

class Runner {
public:
    Runner() = default;

    void runCountSpanningTrees(Graph const& graph);
    void runFindMaxIndependentSet(Graph const& graph);
    void runFindMaxIndependentSetEdges(Graph const& graph);
    void runFindMinVertexCover(Graph const& graph);
    void runFindMinEdgeCover(Graph const& graph);
    void runFindMinColoring(Graph const& graph);

private:
};

}  // namespace lab4
