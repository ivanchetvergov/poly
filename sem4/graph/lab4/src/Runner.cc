#include "Runner.h"

#include <iostream>

namespace lab4 {

using graph::KirchhoffTheorem;
using graph::GraphCombinatorics;

int Runner::countSpanningTrees(Graph const& graph) {
    int count = KirchhoffTheorem::countSpanningTrees(graph);
    last_spanning_ = SpanningTreeResult{count};
    return count;
}

CombinatoricsResult Runner::findMaxIndependentSet(Graph const& graph) {
    GraphCombinatorics gc;
    auto mis = gc.findMaxIndependentSetVertices(graph);
    last_combinatorics_ = CombinatoricsResult{true, false, false, 0, mis, {}, {}, {}, {}, {}};
    return *last_combinatorics_;
}

CombinatoricsResult Runner::findMaxIndependentSetEdges(Graph const& graph) {
    GraphCombinatorics gc;
    auto edges = gc.findMaxIndependentSetEdges(graph);
    last_combinatorics_ = CombinatoricsResult{false, false, false, 0, {}, {}, {}, {}, edges, {}};
    return *last_combinatorics_;
}

CombinatoricsResult Runner::findMinVertexCover(Graph const& graph) {
    GraphCombinatorics gc;
    auto vc = gc.findMinVertexCover(graph);
    last_combinatorics_ = CombinatoricsResult{false, true, false, 0, {}, vc, {}, {}, {}, {}};
    return *last_combinatorics_;
}

CombinatoricsResult Runner::findMinEdgeCover(Graph const& graph) {
    GraphCombinatorics gc;
    auto ec = gc.findMinEdgeCover(graph);
    last_combinatorics_ = CombinatoricsResult{false, false, false, 0, {}, {}, {}, {}, {}, ec};
    return *last_combinatorics_;
}

CombinatoricsResult Runner::findMinColoring(Graph const& graph) {
    GraphCombinatorics gc;
    auto coloring = gc.findMinColoring(graph);
    int chromatic_number = 0;
    for (int color : coloring) {
        chromatic_number = std::max(chromatic_number, color + 1);
    }
    last_combinatorics_ = CombinatoricsResult{false, false, false, chromatic_number, {}, {}, {}, coloring, {}, {}};
    return *last_combinatorics_;
}

std::optional<SpanningTreeResult> const& Runner::getLastSpanningTree() const {
    return last_spanning_;
}

std::optional<CombinatoricsResult> const& Runner::getLastCombinatorics() const {
    return last_combinatorics_;
}

}  // namespace lab4

