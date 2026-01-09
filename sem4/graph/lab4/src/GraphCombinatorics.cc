#include "GraphCombinatorics.h"

#include <algorithm>
#include <unordered_set>

using EdgesSet = std::vector<std::pair<int, int>>;
using VerticesSet = std::vector<int>;

namespace graph {

VerticesSet GraphCombinatorics::findMaxIndependentSetVertices(Graph const& graph) {
    auto vertices = graph.vertexIds();
    size_t n = vertices.size();

    if (n < 10) {
        VerticesSet current, best;
        backtrackIndependentSet(graph, 0, vertices, current, best);
        return best;
    }

    std::sort(vertices.begin(), vertices.end(),
              [&](int a, int b) { return graph.degree(a) > graph.degree(b); });

    VerticesSet independent_set;
    std::unordered_set<int> selected;

    for (int vertex : vertices) {
        bool canAdd = true;
        for (int neighbor : graph.getNeighbors(vertex)) {
            if (selected.count(neighbor)) {
                canAdd = false;
                break;
            }
        }
        if (canAdd) {
            independent_set.push_back(vertex);
            selected.insert(vertex);
        }
    }
    return independent_set;
}

EdgesSet GraphCombinatorics::findMaxIndependentSetEdges(Graph const& graph) {
    EdgesSet matching;
    std::unordered_set<int> used_vertices;

    auto edges = graph.edges();
    for (auto const& edge : edges) {
        int u = edge.from;
        int v = edge.to;
        if (used_vertices.count(u) == 0 && used_vertices.count(v) == 0) {
            matching.emplace_back(u, v);
            used_vertices.insert(u);
            used_vertices.insert(v);
        }
    }
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
    // Cover remaining vertices
    for (int vertex : graph.vertexIds()) {
        if (coveredVertices.count(vertex) == 0) {
            auto neighbors = graph.getNeighbors(vertex);
            if (!neighbors.empty()) {
                // Add edge to first neighbor
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

    // Create mapping from vertex ID to index
    std::unordered_map<int, int> vertex_to_index;
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertex_to_index[vertices[i]] = i;
    }

    if (n < 10) {
        VerticesSet colors(n, -1);
        VerticesSet best_colors(n, -1);
        int bestColors = static_cast<int>(n);
        backtrackColoring(graph, 0, vertices, colors, bestColors, best_colors, vertex_to_index);
        return best_colors;
    }

    VerticesSet colors(n, -1);
    std::sort(vertices.begin(), vertices.end(),
              [&graph](int a, int b) { return graph.degree(a) > graph.degree(b); });

    for (int vertex : vertices) {
        std::unordered_set<int> used_colors;
        for (auto const& [neighbor, weight] : graph.neighbors(vertex)) {
            int neighbor_idx = vertex_to_index[neighbor];
            if (colors[neighbor_idx] != -1) {
                used_colors.insert(colors[neighbor_idx]);
            }
        }
        int color = 0;
        while (used_colors.count(color)) {
            ++color;
        }
        colors[vertex_to_index[vertex]] = color;
    }
    return colors;
}



void GraphCombinatorics::backtrackIndependentSet(Graph const& graph, int index,
                                                  VerticesSet const& vertices,
                                                  VerticesSet& current, VerticesSet& best) {
    if (index == static_cast<int>(vertices.size())) {
        if (current.size() > best.size() && isIndependentSet(graph, current)) {
            best = current;
        }
        return;
    }

    int vertex = vertices[index];
    backtrackIndependentSet(graph, index + 1, vertices, current, best);

    bool canAdd = true;
    for (int v : current) {
        if (graph.hasEdge(vertex, v)) {
            canAdd = false;
            break;
        }
    }
    if (canAdd) {
        current.push_back(vertex);
        backtrackIndependentSet(graph, index + 1, vertices, current, best);
        current.pop_back();
    }
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
