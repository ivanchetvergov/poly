#include "MaxFlow.h"
#include <PathUtils.h>
#include <algorithm>
#include <limits>
#include <unordered_set>
#include <fstream>
#include <iomanip>

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
                double residual = m_network.getResidualCapacity(u, v);

                if (residual > 0) {
                    visited.insert(v);
                    parent[v] = u;

                    if (v == sink) return true;

                    q.push(v);
                }
            }
        }
    }
    return false;
}

void MaxFlow::captureSnapshot(int step, double totalFlow) {
    FlowSnapshot snapshot;
    snapshot.step = step;
    snapshot.totalFlow = totalFlow;

    for (int u : m_network.vertexIds()) {
        for (int v : m_network.neighbors(u)) {
            double flow = m_network.getFlow(u, v);
            double capacity = m_network.getCapacity(u, v);
            if (capacity > 0) {
                snapshot.edges.emplace_back(u, v, flow, capacity);
            }
        }
    }

    m_snapshots.push_back(snapshot);
}

double MaxFlow::fordFulkerson(int source, int sink, bool enableLogging) {
    double maxFlow = 0.0;
    std::unordered_map<int, int> parent;
    int step = 0;

    resetFlows(m_network);
    m_snapshots.clear();

    if (enableLogging) {
        captureSnapshot(step++, maxFlow);
    }

    while (bfs(source, sink, parent)) {
        auto calcResidual = [this](int u, int v) {
            return m_network.getResidualCapacity(u, v);
        };

        double pathFlow = PathUtils<double>::getMinPathValue(source, sink, parent, calcResidual);

        PathUtils<double>::forEachEdgeInPath(source, sink, parent,
            [this, pathFlow](int u, int v) {
                m_network.addFlow(u, v, pathFlow);
            });

        maxFlow += pathFlow;

        if (enableLogging) {
            captureSnapshot(step++, maxFlow);
        }
    }
    return maxFlow;
}

void MaxFlow::exportSnapshots(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    file << std::fixed << std::setprecision(2);
    file << m_snapshots.size() << "\n";

    for (const auto& snapshot : m_snapshots) {
        file << snapshot.step << " " << snapshot.totalFlow << "\n";
        file << snapshot.edges.size() << "\n";

        for (const auto& [u, v, flow, capacity] : snapshot.edges) {
            file << u << " " << v << " " << flow << " " << capacity << "\n";
        }
    }

    file.close();
}

} // namespace graph
