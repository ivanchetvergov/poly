#pragma once

#include <algorithm>
#include <stdexcept>

namespace graph {

template <typename GraphT, typename AddEdgeFunc, typename WeightGenFunc>
std::unique_ptr<GraphT> Generator::generateAcyclicTemplate(int numVertices, int numEdges,
                                                           AddEdgeFunc addEdge,
                                                           WeightGenFunc genWeight,
                                                           bool isDirected) {
    if (numVertices <= 0) {
        throw std::invalid_argument("Количество вершин должно быть положительным");
    }

    int min_edges = numVertices - 1;
    numEdges = std::max(numEdges, min_edges);

    int max_edges = numVertices * (numVertices - 1) / 2;
    numEdges = std::min(numEdges, max_edges);

    auto graph = std::make_unique<GraphT>(isDirected);

    for (int i = 0; i < numVertices; ++i) {
        graph->addVertex(i);
    }

    for (int i = 1; i < numVertices; ++i) {
        int from = randomInt(0, i - 1);
        addEdge(*graph, from, i, genWeight());
    }

    std::vector<std::pair<int, int>> candidates;
    for (int from = 0; from < numVertices - 1; ++from) {
        for (int to = from + 1; to < numVertices; ++to) {
            if (!graph->hasEdge(from, to)) {
                candidates.emplace_back(from, to);
            }
        }
    }

    std::shuffle(candidates.begin(), candidates.end(), rng_);

    int to_add = numEdges - (numVertices - 1);
    for (int i = 0; i < to_add && i < static_cast<int>(candidates.size()); ++i) {
        auto [from, to] = candidates[i];
        addEdge(*graph, from, to, genWeight());
    }

    return graph;
}

}  // namespace graph
