// GraphMetrics.cc
#include "GraphMetrics.h"

#include <algorithm>
#include <queue>

namespace graph {

GraphMetrics::GraphMetrics(Graph const& graph) : m_graph_(graph) {}

std::unordered_map<int, double> GraphMetrics::dijkstra(int src) const {
    constexpr double INF = std::numeric_limits<double>::infinity();

    std::unordered_map<int, double> dist;
    for (int v : m_graph_.vertexIds()) dist[v] = INF;
    dist[src] = 0.0;

    // (dist, vertex)
    using P = std::pair<double, int>;
    std::priority_queue<P, std::vector<P>, std::greater<>> pq;
    pq.push({0.0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;

        for (auto const& [v, w] : m_graph_.neighbors(u)) {
            double weight = w;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

MetricsResult GraphMetrics::compute() const {
    constexpr double INF = std::numeric_limits<double>::infinity();
    auto ids = m_graph_.vertexIds();

    MetricsResult result;
    double radius = INF, diameter = 0.0;

    for (int v : ids) {
        auto dist = dijkstra(v);
        double ecc = 0.0;
        for (int u : ids) {
            if (u == v) continue;
            double d = dist.at(u);
            if (d == INF) { ecc = INF; break; }
            ecc = std::max(ecc, d);
        }
        result.eccentricities[v] = ecc;
        if (ecc != INF) {
            radius   = std::min(radius, ecc);
            diameter = std::max(diameter, ecc);
        }
    }

    result.radius   = radius   == INF ? 0.0 : radius;
    result.diameter = diameter;

    for (int v : ids) {
        double ecc = result.eccentricities[v];
        if (ecc == result.radius)   result.center.push_back(v);
        if (ecc == result.diameter) result.diametralVerts.push_back(v);
    }

    std::sort(result.center.begin(), result.center.end());
    std::sort(result.diametralVerts.begin(), result.diametralVerts.end());

    return result;
}

}  // namespace graph
