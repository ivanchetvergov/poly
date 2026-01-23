#pragma once

#include <Graph.h>
#include <unordered_map>

namespace graph {

using EdgesSet = std::vector<std::pair<int, int>>;
using VerticesSet = std::vector<int>;

class GraphCombinatorics {
public:
    explicit GraphCombinatorics() = default;

    [[nodiscard]] VerticesSet findMaxIndependentSetVertices(Graph const& graph);
    [[nodiscard]] EdgesSet findMaxIndependentSetEdges(Graph const& graph);
    [[nodiscard]] VerticesSet findMinVertexCover(Graph const& graph);
    [[nodiscard]] EdgesSet findMinEdgeCover(Graph const& graph);
    [[nodiscard]] VerticesSet findMinColoring(Graph const& graph);


private:
    void backtrackIndependentSet(Graph const& graph, int index, VerticesSet const& vertices,
                                 VerticesSet& current, VerticesSet& best);
    [[nodiscard]] bool backtrackColoring(Graph const& graph, int index, VerticesSet const& vertices,
                          VerticesSet& colors, int num_colors,
                          std::unordered_map<int, int> const& vertex_to_index);

    [[nodiscard]] bool isIndependentSet(Graph const& graph, VerticesSet const& vertices) const;

    [[nodiscard]] bool isValidColoring(Graph const& graph, VerticesSet const& colors,
                                      VerticesSet const& vertices,
                                      std::unordered_map<int, int> const& vertex_to_index) const;
};

}  // namespace graph
