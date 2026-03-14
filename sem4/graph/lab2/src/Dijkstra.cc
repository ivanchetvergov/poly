// Dijkstra.cc
#include "../include/Dijkstra.h"

#include <functional>
#include <limits>
#include <queue>
#include <utility>

namespace graph {

namespace {
constexpr double INF = std::numeric_limits<double>::infinity();
}

Dijkstra::Dijkstra(Graph const& graph) : m_graph_(graph) {}

DijkstraResult Dijkstra::compute(int start) const {
    DijkstraResult result;

    for (int v : m_graph_.vertexIds()) {
        result.dist[v]   = INF;
        result.parent[v] = -1;
    }
    result.dist[start] = 0.0;

    // min-heap: (distance, vertex)
    using Entry = std::pair<double, int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;
    pq.push({0.0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        ++result.iterations;  // heap pop operation

        if (d > result.dist[u]) continue;  // stale entry

        for (auto const& [v, w] : m_graph_.neighbors(u)) {
            ++result.iterations;  // edge relaxation attempt
            double nd = result.dist[u] + w;
            if (nd < result.dist[v]) {
                result.dist[v]   = nd;
                result.parent[v] = u;
                pq.push({nd, v});
            }
        }
    }

    return result;
}

}  // namespace graph
