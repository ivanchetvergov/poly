#include "Generator.h"

#include "../../lab3/include/FlowNetwork.h"

#include <algorithm>
#include <stdexcept>

namespace graph {

Generator::Generator(std::optional<unsigned> seed)
    : rng_(seed.value_or(std::random_device{}())), normal_dist_(0.0, 1.0) {}

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
    auto gen_weight = [this, allowNegative]() {
        return allowNegative ? randomReal(-10.0, 10.0) : randomReal(1.0, 10.0);
    };
    auto add_edge = [](Graph& g, int from, int to, double w) { g.addEdge(from, to, w); };
    return generateAcyclicTemplate<Graph>(numVertices, numEdges, add_edge, gen_weight, isDirected);
}

std::unique_ptr<FlowNetwork> Generator::generateFlowNetwork(int numVertices, int numEdges) {
    auto gen_weight = [this]() {
        return std::make_pair(randomReal(5.0, 20.0), randomReal(1.0, 10.0));
    };
    auto add_edge = [](FlowNetwork& g, int from, int to, std::pair<double, double> w) {
        g.addEdge(from, to, w.first, w.second);  // cost and capacity
    };
    return generateAcyclicTemplate<FlowNetwork>(numVertices, numEdges, add_edge, gen_weight, true);
}

}  // namespace graph
