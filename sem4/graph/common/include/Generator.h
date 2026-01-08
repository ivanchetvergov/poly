#pragma once

#include "Graph.h"
#include <memory>
#include <optional>
#include <random>
#include <functional>
#include <vector>
#include <utility>

namespace graph {

class FlowNetwork;

class Generator {
public:
    explicit Generator(std::optional<unsigned> seed = std::nullopt);

    std::unique_ptr<Graph> generateAcyclicGraph(int numVertices, int numEdges,
                                                bool isDirected = false,
                                                bool allowNegative = false);

    std::unique_ptr<FlowNetwork> generateFlowNetwork(int numVertices, int numEdges);

    double randomReal(double min, double max);
    int randomInt(int min, int max);

private:
    template <typename GraphT, typename AddEdgeFunc, typename WeightGenFunc>
    std::unique_ptr<GraphT> generateAcyclicTemplate(
        int numVertices, int numEdges,
        AddEdgeFunc addEdge,
        WeightGenFunc genWeight,
        bool isDirected);

    std::mt19937 rng_;
    std::normal_distribution<double> normalDist_;

};

} // namespace graph

#include "../src/Generator.tpp"
