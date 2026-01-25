#include "MinCostFlow.h"

#include <algorithm>
#include <limits>

#include <PathUtils.h>

namespace graph {

namespace {
void resetFlows(FlowNetwork& network) {
    for (int u : network.vertexIds()) {
        for (int v : network.neighbors(u)) {
            network.setFlow(u, v, 0.0);
        }
    }
}
}  // anonymous namespace

bool MinCostFlow::bellmanFord(int source, int sink, std::unordered_map<int, double>& dist,
                              std::unordered_map<int, int>& parent) {
    dist.clear();
    parent.clear();

    for (int v : m_network_.vertexIds()) {
        dist[v] = std::numeric_limits<double>::infinity();
    }
    dist[source] = 0.0;

    int n = static_cast<int>(m_network_.vertexCount());

    for (int i = 0; i < n - 1; ++i) {
        bool updated = false;

        for (int u : m_network_.vertexIds()) {
            if (dist[u] == std::numeric_limits<double>::infinity())
                continue;

            for (int v : m_network_.neighbors(u)) {
                double residual = m_network_.getResidualCapacity(u, v);

                if (residual > 0) {
                    double new_dist = dist[u] + m_network_.getCost(u, v);

                    if (new_dist < dist[v]) {
                        dist[v] = new_dist;
                        parent[v] = u;
                        updated = true;
                    }
                }
            }
        }

        if (!updated)
            break;
    }

    for (int u : m_network_.vertexIds()) {
        if (dist[u] == std::numeric_limits<double>::infinity())
            continue;

        for (int v : m_network_.neighbors(u)) {
            double residual = m_network_.getResidualCapacity(u, v);

            if (residual > 0) {
                double new_dist = dist[u] + m_network_.getCost(u, v);

                if (new_dist < dist[v]) {
                    return false;
                }
            }
        }
    }

    return dist[sink] != std::numeric_limits<double>::infinity();
}

MinCostFlow::Result MinCostFlow::findMinCostFlow(int source, int sink, double targetFlow) {
    Result result;

    resetFlows(m_network_);

    double current_flow = 0.0;
    double total_cost = 0.0;
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;

    std::vector<int> last_path;

    while (current_flow < targetFlow && bellmanFord(source, sink, dist, parent)) {
        auto calc_residual = [this](int u, int v) { return m_network_.getResidualCapacity(u, v); };

        double path_flow = std::min(targetFlow - current_flow,
                    PathUtils<double>::getMinPathValue(source, sink, parent, calc_residual));

        last_path = PathUtils<double>::reconstructPath(source, sink, parent);

        PathUtils<double>::forEachEdgeInPath(source, sink, parent,
                                             [this, &total_cost, path_flow](int u, int v) {
                                                 m_network_.addFlow(u, v, path_flow);
                                                 total_cost += path_flow * m_network_.getCost(u, v);
                                             });

        current_flow += path_flow;
    }

    result.flow = current_flow;
    result.cost = total_cost;
    result.success = (current_flow == targetFlow);
    result.path = last_path;

    return result;
}

}  // namespace graph
