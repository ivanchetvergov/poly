#include "Generator.h"
#include <stdexcept>
#include <algorithm>

namespace graph {

Generator::Generator(std::optional<unsigned> seed)
    : rng_(seed.value_or(std::random_device{}())),
      normalDist_(0.0, 1.0) {}

int Generator::randomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng_);
}


double Generator::randomReal(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng_);
}

std::unique_ptr<Graph> Generator::generateAcyclicGraph(int numVertices, int numEdges,
                                                       bool isDirected, bool allowNegative) {
    if (numVertices <= 0) {
        throw std::invalid_argument("Количество вершин должно быть положительным");
    }

    int minEdges = numVertices - 1;
    numEdges = std::max(numEdges, minEdges);

    int maxEdges = numVertices * (numVertices - 1) / 2;
    numEdges = std::min(numEdges, maxEdges);

    auto graph = std::make_unique<Graph>(isDirected);

    for (int i = 0; i < numVertices; ++i) {
        graph->addVertex(i);
    }

    for (int i = 1; i < numVertices; ++i) {
        int from = randomInt(0, i - 1);
        double weight = allowNegative ? randomReal(-10.0, 10.0) : randomReal(1.0, 10.0);

        graph->addEdge(from, i, weight);
    }

    int currentEdges = numVertices - 1;
    while (currentEdges < numEdges) {
        int from = randomInt(0, numVertices - 2);
        int to = randomInt(from + 1, numVertices - 1);

        if (!graph->hasEdge(from, to)) {
            double weight = allowNegative ? randomReal(-10.0, 10.0) : randomReal(1.0, 10.0);
            graph->addEdge(from, to, weight);
            currentEdges++;
        }
    }

    return graph;
}

} // namespace graph
