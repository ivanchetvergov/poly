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
    for (int i = 0; i < to_add && i < candidates.size()); ++i) {
        auto [from, to] = candidates[i];
        addEdge(*graph, from, to, genWeight());
    }

    return graph;
}

template <typename GraphT, typename AddEdgeFunc, typename WeightGenFunc>
std::unique_ptr<GraphT> Generator::generateByDegreesTemplate(int numVertices,
                                                             std::vector<int> degrees,
                                                             AddEdgeFunc addEdge,
                                                             WeightGenFunc genWeight,
                                                             bool isDirected)
{
    if (numVertices <= 0)
        throw std::invalid_argument("Количество вершин должно быть положительным");
    if (degrees.size() != numVertices)
        throw std::invalid_argument("Размер вектора степеней должен совпадать с числом вершин");

    auto graph = std::make_unique<GraphT>(isDirected);
    for (int i = 0; i < numVertices; ++i) graph->addVertex(i);

    for (int i = 1; i < numVertices; ++i) {
        std::vector<int> with_budget;
        for (int j = 0; j < i; ++j)
            if (degrees[j] > 0) with_budget.push_back(j);

        int from;
        if (!with_budget.empty()) {
            from = with_budget[randomInt(0, with_budget.size() - 1)];
        } else {
            from = randomInt(0, i - 1);
        }
        addEdge(*graph, from, i, genWeight());
        if (degrees[from] > 0) --degrees[from];
    }

    for (int i = 0; i < numVertices - 1; ++i) {
        if (degrees[i] <= 0) continue;

        std::vector<int> available;
        for (int j = i + 1; j < numVertices; ++j)
            if (!graph->hasEdge(i, j)) available.push_back(j);

        std::shuffle(available.begin(), available.end(), rng_);
        int toAdd = std::min(degrees[i], available.size());

        for (int k = 0; k < toAdd; ++k)
            addEdge(*graph, i, available[k], genWeight());
    }

    return graph;
}


template <typename GraphT, typename WeightFunc, typename AddFunc>
std::unique_ptr<GraphT> Generator::buildByRice(int numVertices, bool isDirected,
                                               double a, double h, EdgeCountDist dist,
                                               WeightFunc genWeight, AddFunc addEdge) {
    if (numVertices <= 0)
        throw std::invalid_argument("Количество вершин должно быть положительным");
    int minE = numVertices - 1;
    int maxE = isDirected ? numVertices * (numVertices - 1)
                          : numVertices * (numVertices - 1) / 2;
    int totalEdges = sampleEdgeCount(minE, maxE, dist);
    auto degrees   = computeDegreesFromRice(numVertices, isDirected, a, h, totalEdges);
    return generateByDegreesTemplate<GraphT>(numVertices, std::move(degrees),
                                             addEdge, genWeight, isDirected);
}

}  // namespace graph
