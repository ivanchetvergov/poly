#pragma once

#include <Graph.h>
#include <unordered_map>

namespace graph {

using EdgesSet = std::vector<std::pair<int, int>>;
using VerticesSet = std::vector<int>;

class GraphCombinatorics {
public:
    explicit GraphCombinatorics() = default;


    [[nodiscard]] VerticesSet findMinColoring(Graph const& graph);


private:
    [[nodiscard]] bool backtrackColoring(Graph const& graph, int index, VerticesSet const& vertices,
                          VerticesSet& colors, int num_colors,
                          std::unordered_map<int, int> const& vertex_to_index);

    [[nodiscard]] bool isValidColoring(Graph const& graph, VerticesSet const& colors,
                                      VerticesSet const& vertices,
                                      std::unordered_map<int, int> const& vertex_to_index) const;
};

}  // namespace graph
