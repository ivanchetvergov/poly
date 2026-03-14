#include "Generator.h"

#include "../../lab3/include/FlowNetwork.h"

#include <algorithm>
#include <cmath>
#include <numeric>
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

double Generator::riceWeight(double a, double h) {
    double u1 = normal_dist_(rng_);
    double u2 = normal_dist_(rng_);
    return std::sqrt(std::pow(h + a * u1, 2) + std::pow(a * u2, 2));
}

double Generator::applySign(double w, WeightSign sign) {
    w = std::abs(w);
    if (sign == WeightSign::Negative) return -w;
    if (sign == WeightSign::Mixed) {
        std::bernoulli_distribution coin(0.5);
        return coin(rng_) ? w : -w;
    }
    return w;
}

int Generator::sampleEdgeCount(int minE, int maxE, EdgeCountDist dist)
{
    if (minE >= maxE) return minE;
    if (dist == EdgeCountDist::Uniform) {
        return randomInt(minE, maxE);
    }
    double mu    = (minE + maxE) / 2.0;
    double sigma = (maxE - minE) / 6.0;
    double val;
    do {
        val = mu + sigma * normal_dist_(rng_);
    } while (val < minE || val > maxE);
    return static_cast<int>(std::round(val));
}

std::vector<int> Generator::computeDegreesFromRice(int n,
                                                   bool isDirected,
                                                   double a,
                                                   double h,
                                                   int totalEdges)
{
    if (n <= 1) return {};

    std::vector<double> prob(n);
    for (auto& v : prob) v = riceWeight(a, h);

    double maxVal = *std::max_element(prob.begin(), prob.end());
    double sumExp = 0.0;
    for (auto& v : prob) { v = std::exp(v - maxVal); sumExp += v; }
    for (auto& v : prob) v /= sumExp;

    std::vector<int> capacity(n);
    for (int i = 0; i < n; ++i) capacity[i] = n - 1 - i;

    std::vector<double> quota(n);
    double sumW = 0.0;
    for (int i = 0; i < n; ++i) { quota[i] = prob[i] * capacity[i]; sumW += quota[i]; }
    if (sumW > 0.0)
        for (auto& q : quota) q *= static_cast<double>(totalEdges) / sumW;

    std::vector<int> degrees(n);
    int allocated = 0;
    for (int i = 0; i < n; ++i) {
        degrees[i] = std::min(static_cast<int>(std::floor(quota[i])), capacity[i]);
        allocated += degrees[i];
    }

    int remainder = totalEdges - allocated;
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0); // 0, 1, 2, ..., n-1 (сохраним айдишники вершин)
    std::sort(order.begin(), order.end(), [&](int a, int b) {
        return (quota[a] - std::floor(quota[a])) > (quota[b] - std::floor(quota[b]));
    });
    for (int idx : order) {
        if (remainder <= 0) break;
        if (degrees[idx] < capacity[idx]) { ++degrees[idx]; --remainder; }
    }
    return degrees;
}
std::unique_ptr<Graph> Generator::generateAcyclicGraph(int numVertices,
                                                       int numEdges,
                                                       bool isDirected,
                                                       WeightSign sign)
{
    auto gen_weight = [this, sign]() {
        return applySign(randomReal(1.0, 10.0), sign);
    };
    auto add_edge = [](Graph& g, int from, int to, double w) { g.addEdge(from, to, w); };
    return generateAcyclicTemplate<Graph>(numVertices, numEdges, add_edge, gen_weight, isDirected);
}

std::unique_ptr<Graph> Generator::generateRiceGraphByDegrees(int numVertices,
                                                             bool isDirected,
                                                             double a,
                                                             double h,
                                                             EdgeCountDist dist,
                                                             WeightSign sign)
{
    auto gen_weight = [this, a, h, sign]() { return applySign(riceWeight(a, h), sign); };
    auto add_edge   = [](Graph& g, int from, int to, double w) { g.addEdge(from, to, w); };
    return buildByRice<Graph>(numVertices, isDirected, a, h, dist, gen_weight, add_edge);
}

// * FlowNetwork

std::unique_ptr<FlowNetwork> Generator::generateFlowNetwork(int numVertices,
                                                            int numEdges,
                                                            WeightSign sign)
{
    auto gen_weight = [this, sign]() {
        return std::make_pair(randomReal(5.0, 20.0),
                              applySign(randomReal(1.0, 10.0), sign));
    };
    auto add_edge = [](FlowNetwork& g, int from, int to, std::pair<double, double> w) {
        g.addEdge(from, to, w.first, w.second);
    };
    return generateAcyclicTemplate<FlowNetwork>(numVertices, numEdges, add_edge, gen_weight, true);
}

std::unique_ptr<FlowNetwork> Generator::generateFlowNetworkByDegrees(int numVertices,
                                                                     double a,
                                                                     double h,
                                                                     EdgeCountDist dist,
                                                                     WeightSign sign)
{
    auto gen_weight = [this, a, h, sign]() {
        return std::make_pair(std::abs(riceWeight(a, h)),
                              applySign(riceWeight(a, h), sign));
    };
    auto add_edge = [](FlowNetwork& g, int from, int to, std::pair<double, double> w) {
        g.addEdge(from, to, w.first, w.second);
    };
    return buildByRice<FlowNetwork>(numVertices, /*isDirected=*/true,
                                    a, h, dist, gen_weight, add_edge);
}

}  // namespace graph
