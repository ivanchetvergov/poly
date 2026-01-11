#include "GraphCombinatorics.h"

#include <algorithm>
#include <unordered_set>

using EdgesSet = std::vector<std::pair<int, int>>;
using VerticesSet = std::vector<int>;

namespace graph {

VerticesSet GraphCombinatorics::findMaxIndependentSetVertices(Graph const& graph) {
    auto vertices = graph.vertexIds();
    size_t n = vertices.size();

    if (n < 15) {
        // перебор всех подмножеств вершин O(2^n * n^2)
        VerticesSet best;
        for (int mask = 0; mask < (1 << n); ++mask) {
            VerticesSet subset;
            for (size_t i = 0; i < n; ++i) {
                if (mask & (1 << i)) {
                    subset.push_back(vertices[i]);
                }
            }
            if (isIndependentSet(graph, subset) && subset.size() > best.size()) {
                best = subset;
            }
        }
        return best;
    }

    // TODO: для n >= 15 жадный алгоритм:
    // sort вершин по степени (квазилинейно) затем перебор вершин O(n * m)
    VerticesSet independent_set;
    return independent_set;
}

EdgesSet GraphCombinatorics::findMaxIndependentSetEdges(Graph const& graph) {
    auto edges = graph.edges();
    size_t m = edges.size();

    if (m < 30) {
        // перебор всех подмножеств рёбер: O(2^m * m)
        EdgesSet best;
        for (int mask = 0; mask < (1 << m); ++mask) {
            EdgesSet subset;
            std::unordered_set<int> used_vertices;
            bool valid = true;

            for (size_t i = 0; i < m; ++i) {
                if (mask & (1 << i)) {
                    int u = edges[i].from;
                    int v = edges[i].to;
                    if (used_vertices.count(u) || used_vertices.count(v)) {
                        valid = false;
                        break;
                    }
                    subset.push_back({u, v});
                    used_vertices.insert(u);
                    used_vertices.insert(v);
                }
            }

            if (valid && subset.size() > best.size()) {
                best = subset;
            }
        }
        return best;
    }

    // TODO: для m >= 30 жадный алгоритм:
    // - sort рёбер степени (квазилинейно) и затем за линию по ребрам O(m)
    EdgesSet matching;
    return matching;
}

VerticesSet GraphCombinatorics::findMinVertexCover(Graph const& graph) {
    auto mis = findMaxIndependentSetVertices(graph);
    std::unordered_set<int> mis_set(mis.begin(), mis.end());

    VerticesSet cover;
    for (int vertex : graph.vertexIds()) {
        if (mis_set.count(vertex) == 0) {
            cover.push_back(vertex);
        }
    }
    return cover;
}

EdgesSet GraphCombinatorics::findMinEdgeCover(Graph const& graph) {
    auto matching = findMaxIndependentSetEdges(graph);
    std::unordered_set<int> coveredVertices;

    for (auto const& edge : matching) {
        coveredVertices.insert(edge.first);
        coveredVertices.insert(edge.second);
    }

    EdgesSet cover = matching;
    for (int vertex : graph.vertexIds()) {
        if (coveredVertices.count(vertex) == 0) {
            auto neighbors = graph.getNeighbors(vertex);
            if (!neighbors.empty()) {
                int neighbor = neighbors[0];
                cover.emplace_back(vertex, neighbor);
                coveredVertices.insert(vertex);
                coveredVertices.insert(neighbor);
            }
        }
    }
    return cover;
}

VerticesSet GraphCombinatorics::findMinColoring(Graph const& graph) {
    auto vertices = graph.vertexIds();
    size_t n = vertices.size();

    std::unordered_map<int, int> vertex_to_index;
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertex_to_index[vertices[i]] = i;
    }

    if (n < 15) {
        VerticesSet colors(n, -1);
        VerticesSet best_colors(n, -1);

        for (int max_colors = 1; max_colors <= static_cast<int>(n); ++max_colors) {
            std::fill(colors.begin(), colors.end(), -1);
            backtrackColoring(graph, 0, vertices, colors, max_colors, best_colors, vertex_to_index);
            if (best_colors[0] != -1) {
                return best_colors;
            }
        }
        return best_colors;
    }

    // TODO: жадный алгоритм
    VerticesSet colors(n, -1);
    return colors;
}

void GraphCombinatorics::backtrackColoring(Graph const& graph, int index,
                                           VerticesSet const& vertices, VerticesSet& colors,
                                           int& bestColors, VerticesSet& best_colors,
                                           std::unordered_map<int, int> const& vertex_to_index) {
    if (index == static_cast<int>(vertices.size())) {
        if (isValidColoring(graph, colors, vertices, vertex_to_index)) {
            int maxColor = *std::max_element(colors.begin(), colors.end());
            if (maxColor + 1 < bestColors) {
                bestColors = maxColor + 1;
                best_colors = colors;
            }
        }
        return;
    }

    int vertex = vertices[index];
    for (int color = 0; color < bestColors; ++color) {
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
            backtrackColoring(graph, index + 1, vertices, colors, bestColors, best_colors, vertex_to_index);
            colors[index] = -1;
        }
    }
}

bool GraphCombinatorics::isIndependentSet(Graph const& graph,
                                          VerticesSet const& vertices) const {
    for (size_t i = 0; i < vertices.size(); ++i) {
        for (size_t j = i + 1; j < vertices.size(); ++j) {
            if (graph.hasEdge(vertices[i], vertices[j])) {
                return false;
            }
        }
    }
    return true;
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
