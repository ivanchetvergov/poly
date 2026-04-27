#include "GraphCombinatorics.h"

#include <algorithm>
#include <unordered_set>

using EdgesSet = std::vector<std::pair<int, int>>;
using VerticesSet = std::vector<int>;

namespace graph {

VerticesSet GraphCombinatorics::findMinColoring(Graph const& graph) {
    auto vertices = graph.vertexIds();
    std::sort(vertices.begin(), vertices.end());
    size_t n = vertices.size();

    std::unordered_map<int, int> vertex_to_index;
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertex_to_index[vertices[i]] = i;
    }

    VerticesSet colors(n, -1);
    VerticesSet best_colors(n, -1);

    for (int num_colors = 1; num_colors <= static_cast<int>(n); ++num_colors) {
        std::fill(colors.begin(), colors.end(), -1);
        if (backtrackColoring(graph, 0, vertices, colors, num_colors, vertex_to_index)) {
            best_colors = colors;
            break;
        }
    }
    return best_colors;
}

bool GraphCombinatorics::backtrackColoring(Graph const& graph, int index,
                                           VerticesSet const& vertices, VerticesSet& colors,
                                           int num_colors,
                                           std::unordered_map<int, int> const& vertex_to_index) {
    if (index == static_cast<int>(vertices.size())) {
        return isValidColoring(graph, colors, vertices, vertex_to_index);
    }

    int vertex = vertices[index];
    for (int color = 0; color < num_colors; ++color) {
        bool canColor = true;
        for (int neighbor : graph.getNeighbors(vertex)) {
            int neighbor_idx = vertex_to_index.at(neighbor);
            if (colors[neighbor_idx] == color) {
                canColor = false;
                break;
            }
        }
        if (canColor) {
            colors[index] = color;
            if (backtrackColoring(graph, index + 1, vertices, colors, num_colors, vertex_to_index)) {
                return true;
            }
            colors[index] = -1;
        }
    }
    return false;
}

bool GraphCombinatorics::isValidColoring(Graph const& graph, VerticesSet const& colors,
                                         VerticesSet const& vertices,
                                         std::unordered_map<int, int> const& vertex_to_index) const {
    for (size_t i = 0; i < vertices.size(); ++i) {
        int v = vertices[i];
        for (auto const& [neighbor, weight] : graph.neighbors(v)) {
            int neighbor_idx = vertex_to_index.at(neighbor);
            if (colors[i] != -1 && colors[i] == colors[neighbor_idx]) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace graph
