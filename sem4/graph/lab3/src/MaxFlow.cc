#include "MaxFlow.h"
#include <PathUtils.h>
#include <algorithm>
#include <limits>
#include <unordered_set>

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

bool MaxFlow::bfs(int source, int sink, std::unordered_map<int, int>& parent) {
    std::unordered_set<int> visited;
    std::queue<int> q;

    q.push(source);
    visited.insert(source);
    parent.clear();

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : m_network.neighbors(u)) {
            if (visited.find(v) == visited.end()) {
                double diff = m_network.getCapacity(u, v) - m_network.getFlow(u, v);

                if (diff > 0) {
                    visited.insert(v);
                    parent[v] = u;

                    if (v == sink) return true

                    q.push(v);
                }
            }
        }
    }
    return false;
}

double MaxFlow::fordFulkerson(int source, int sink) {
    double maxFlow = 0.0;
    std::unordered_map<int, int> parent;

    resetFlows(m_network);

    while (bfs(source, sink, parent)) {
        auto calcResidual = [this](int u, int v) {
            return m_network.getCapacity(u, v) - m_network.getFlow(u, v);
        };

        double pathFlow = PathUtils<double>::getMinPathValue(source, sink, parent, calcResidual);

        PathUtils<double>::forEachEdgeInPath(source, sink, parent,
            [this, pathFlow](int u, int v) {
                m_network.addFlow(u, v, pathFlow);
            });

        maxFlow += pathFlow;
    }

    return maxFlow;
}

} // namespace graph
