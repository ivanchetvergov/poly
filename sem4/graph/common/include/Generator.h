#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <random>
#include <utility>
#include <vector>

namespace graph {

class FlowNetwork;
class Graph;

enum class EdgeCountDist {
    Uniform,
    TruncatedNormal,
};

enum class WeightSign {
    Positive,
    Negative,
    Mixed,
};

class Generator {
public:
    explicit Generator(std::optional<unsigned> seed = std::nullopt);

    std::unique_ptr<Graph> generateAcyclicGraph(int numVertices, int numEdges,
                                                bool isDirected = false,
                                                WeightSign sign = WeightSign::Positive);

    std::unique_ptr<Graph> generateRiceGraphByDegrees(int numVertices,
                                             bool isDirected = false,
                                             double a = 1.0, double h = 1.0,
                                             EdgeCountDist dist = EdgeCountDist::TruncatedNormal,
                                             WeightSign sign = WeightSign::Positive);

    std::unique_ptr<FlowNetwork> generateFlowNetwork(int numVertices, int numEdges,
                                                     WeightSign costSign = WeightSign::Positive);

    std::unique_ptr<FlowNetwork> generateFlowNetworkByDegrees(int numVertices,
                                                              double a = 1.0, double h = 1.0,
                                                              EdgeCountDist dist = EdgeCountDist::TruncatedNormal,
                                                              WeightSign costSign = WeightSign::Positive);

    int sampleEdgeCount(int minE, int maxE, EdgeCountDist dist);

    std::vector<int> computeDegreesFromRice(int n, bool isDirected, double a, double h,
                                            int totalEdges);

    double randomReal(double min, double max);
    int randomInt(int min, int max);

private:
    double normalWeight();
    double riceWeight(double a, double h);
    double applySign(double w, WeightSign sign);

    template <typename GraphT, typename AddEdgeFunc, typename WeightGenFunc>
    std::unique_ptr<GraphT> generateAcyclicTemplate(int numVertices, int numEdges,
                                                    AddEdgeFunc addEdge, WeightGenFunc genWeight,
                                                    bool isDirected);

    template <typename GraphT, typename AddEdgeFunc, typename WeightGenFunc>
    std::unique_ptr<GraphT> generateByDegreesTemplate(int numVertices, std::vector<int> degrees,
                                                      AddEdgeFunc addEdge, WeightGenFunc genWeight,
                                                      bool isDirected);

    template <typename GraphT, typename WeightFunc, typename AddFunc>
    std::unique_ptr<GraphT> buildByRice(int numVertices, bool isDirected,
                                        double a, double h, EdgeCountDist dist,
                                        WeightFunc genWeight, AddFunc addEdge);

    std::mt19937 rng_;
    std::normal_distribution<double> normal_dist_;
};

}  // namespace graph

#include "../src/Generator.tpp"
