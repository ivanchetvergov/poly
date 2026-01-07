#pragma once

#include "Graph.h"
#include <memory>
#include <optional>
#include <random>

namespace graph {

class Generator {
public:
    explicit Generator(std::optional<unsigned> seed = std::nullopt);

    std::unique_ptr<Graph> generateAcyclicGraph(int numVertices, int numEdges,
                                                bool isDirected = false,
                                                bool allowNegative = false);

    double randomReal(double min, double max);
    int randomInt(int min, int max);

private:
    std::mt19937 rng_;
    std::normal_distribution<double> normalDist_;

};

} // namespace graph
