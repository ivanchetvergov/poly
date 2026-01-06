#include "GraphGenerator.h"
#include <RandomGenerator.h>
#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>

namespace graph {

std::unique_ptr<Graph> generateRandomTree(int numVertices, int, double) {
    if (numVertices <= 0) {
        throw std::invalid_argument("Количество вершин должно быть положительным");
    }

    auto graph = std::make_unique<Graph>();
    RandomGenerator rng;

    for (int i = 0; i < numVertices; ++i) {
        graph->addVertex(i);
    }

    if (numVertices == 1) return graph;

    std::vector<int> inTree = {0};
    std::vector<int> notInTree(numVertices - 1);
    std::iota(notInTree.begin(), notInTree.end(), 1);

    while (!notInTree.empty()) {
        int fromVertex = inTree[rng.randomInt(0, inTree.size() - 1)];
        int toIndex = rng.randomInt(0, notInTree.size() - 1);
        int toVertex = notInTree[toIndex];

        graph->addEdge(fromVertex, toVertex, rng.randomDouble(1.0, 10.0));

        inTree.push_back(toVertex);
        notInTree.erase(notInTree.begin() + toIndex);
    }

    return graph;
}

std::unique_ptr<Graph> generateRandomGraph(int numVertices, int numEdges) {
    auto graph = std::make_unique<Graph>();
    RandomGenerator rng;

    for (int i = 0; i < numVertices; ++i) {
        graph->addVertex(i);
    }
    for (int i = 1; i < numVertices; ++i) {
        int from = rng.randomInt(0, i - 1);
        graph->addEdge(from, i, rng.randomDouble(1.0, 10.0));
    }

    int currentEdges = numVertices - 1;
    int maxEdges = numVertices * (numVertices - 1) / 2;
    numEdges = std::min(numEdges, maxEdges);

    while (currentEdges < numEdges) {
        int from = rng.randomInt(0, numVertices - 1);
        int to = rng.randomInt(0, numVertices - 1);

        if (from != to && !graph->hasEdge(from, to)) {
            graph->addEdge(from, to, rng.randomDouble(1.0, 10.0));
            currentEdges++;
        }
    }

    return graph;
}

} // namespace graph
