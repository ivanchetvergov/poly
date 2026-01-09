#pragma once

#include "Graph.h"
#include "KirchhoffTheorem.h"
#include "GraphCombinatorics.h"

#include <memory>
#include <optional>
#include <vector>
#include <unordered_set>

namespace lab4 {

using graph::Graph;

struct SpanningTreeResult {
    int count;
};

struct CombinatoricsResult {
    bool is_independent;
    bool is_cover;
    bool is_clique;
    int chromatic_number;
    std::vector<int> independent_set;
    std::vector<int> vertex_cover;
    std::vector<int> clique;
    std::vector<int> coloring;
    std::vector<std::pair<int, int>> independent_edges;
    std::vector<std::pair<int, int>> edge_cover;
};

class Runner {
public:
    Runner() = default;

    int countSpanningTrees(Graph const& graph);

    CombinatoricsResult findMaxIndependentSet(Graph const& graph);
    CombinatoricsResult findMaxIndependentSetEdges(Graph const& graph);
    CombinatoricsResult findMinVertexCover(Graph const& graph);
    CombinatoricsResult findMinEdgeCover(Graph const& graph);
    CombinatoricsResult findMinColoring(Graph const& graph);

    std::optional<SpanningTreeResult> const& getLastSpanningTree() const;
    std::optional<CombinatoricsResult> const& getLastCombinatorics() const;

private:
    std::optional<SpanningTreeResult> last_spanning_;
    std::optional<CombinatoricsResult> last_combinatorics_;
};

}  // namespace lab4
