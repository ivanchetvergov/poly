#pragma once

#include "Graph.h"
#include "GraphCombinatorics.h"
#include "KirchhoffTheorem.h"
#include "Boruvka.h"
#include "PruferCode.h"

#include <memory>

namespace lab4 {

using graph::Graph;

class Runner {
public:
    Runner() = default;

    // Lab 4 requirements
    void runCountSpanningTrees(Graph const& graph);
    void runBuildMinimalSpanningTree(Graph const& graph);
    void runPruferEncode(Graph const& graph);
    void runPruferDecode(Graph const& graph);
    void runFindMinColoring(Graph const& graph);

private:
    std::unique_ptr<Graph> last_mst_;
    graph::PruferCode::PruferEncoding last_encoding_;
};

}  // namespace lab4
