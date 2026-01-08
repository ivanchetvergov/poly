#include "MinCostFlow.h"
#include <PathUtils.h>
#include <algorithm>
#include <limits>

namespace graph {

namespace {
    void resetFlows(FlowNetwork& network) {
        for (int u : network.vertexIds()) {
            for (int v : network.neighbors(u)) {
                network.setFlow(u, v, 0.0);
            }
        }
    }
} // anonymous namespace

bool MinCostFlow::bellmanFord(int source, int sink,
                                std::unordered_map<int, double>& dist,
                                std::unordered_map<int, int>& parent) {
    dist.clear();
    parent.clear();

    for (int v : m_network.vertexIds()) {
        dist[v] = std::numeric_limits<double>::infinity();
    }
    dist[source] = 0.0;

    int n = static_cast<int>(m_network.vertexCount());

    for (int i = 0; i < n - 1; ++i) {
        bool updated = false;

        for (int u : m_network.vertexIds()) {
            if (dist[u] == std::numeric_limits<double>::infinity()) continue;

            for (int v : m_network.neighbors(u)) {
                double residual = m_network.getResidualCapacity(u, v);

                if (residual > 0) {
                    double newDist = dist[u] + m_network.getCost(u, v);

                    if (newDist < dist[v]) {
                        dist[v] = newDist;
                        parent[v] = u;
                        updated = true;
                    }
                }
            }
        }

        if (!updated) break;
    }

    for (int u : m_network.vertexIds()) {
        if (dist[u] == std::numeric_limits<double>::infinity()) continue;

        for (int v : m_network.neighbors(u)) {
            double residual = m_network.getResidualCapacity(u, v);

            if (residual > 0) {
                double newDist = dist[u] + m_network.getCost(u, v);

                if (newDist < dist[v]) {
                    return false;
                }
            }
        }
    }

    return dist[sink] != std::numeric_limits<double>::infinity();
}

MinCostFlow::Result MinCostFlow::findMinCostFlow(int source, int sink, double targetFlow) {
    Result result;

    resetFlows(m_network);

    double currentFlow = 0.0;
    double totalCost = 0.0;
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;

    std::vector<int> lastPath;

    while (currentFlow < targetFlow && bellmanFord(source, sink, dist, parent)) {
        auto calcResidual = [this](int u, int v) {
            return m_network.getResidualCapacity(u, v);
        };

        double pathFlow = std::min(targetFlow - currentFlow,
                                   PathUtils<double>::getMinPathValue(source, sink, parent, calcResidual));

        lastPath = PathUtils<double>::reconstructPath(source, sink, parent);

        PathUtils<double>::forEachEdgeInPath(source, sink, parent,
            [this, &totalCost, pathFlow](int u, int v) {
                m_network.addFlow(u, v, pathFlow);
                totalCost += pathFlow * m_network.getCost(u, v);
            });

        currentFlow += pathFlow;
    }

    result.flow = currentFlow;
    result.cost = totalCost;
    result.success = (currentFlow == targetFlow);
    result.path = lastPath;

    return result;
}

} // namespace graph
