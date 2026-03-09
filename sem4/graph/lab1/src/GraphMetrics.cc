// GraphMetrics.cc
#include "GraphMetrics.h"

#include <BFS.h>

#include <algorithm>

namespace graph {

GraphMetrics::GraphMetrics(Graph const& graph) : m_graph_(graph) {}

MetricsResult GraphMetrics::compute() const {
    constexpr double INF = std::numeric_limits<double>::infinity();
    auto ids = m_graph_.vertexIds();

    MetricsResult result;
    double radius = INF, diameter = 0.0;

    for (int v : ids) {
        BFS bfs(m_graph_);
        auto r = bfs.traverse(v);

        double ecc = 0.0;
        for (int u : ids) {
            if (u == v) continue;
            int steps = r.dist_steps.at(u);
            if (steps == -1) { ecc = INF; break; }
            ecc = std::max(ecc, static_cast<double>(steps));
        }
        result.eccentricities[v] = ecc;
        if (ecc != INF) {
            radius   = std::min(radius, ecc);
            diameter = std::max(diameter, ecc);
        }
    }

    result.radius   = radius == INF ? 0.0 : radius;
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

