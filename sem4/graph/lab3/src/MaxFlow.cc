#include "MaxFlow.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>
#include <unordered_set>

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

bool MaxFlow::bfs(int source, int sink, std::unordered_map<int, int>& parent) {
    std::unordered_set<int> visited;
    std::queue<int> q;

    q.push(source);
    visited.insert(source);
    parent.clear();

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : m_network_.neighbors(u)) {
            if (visited.find(v) == visited.end()) {
                double residual = m_network_.getResidualCapacity(u, v);

                if (residual > 0) {
                    visited.insert(v);
                    parent[v] = u;

                    if (v == sink)
                        return true;

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
    snapshot.total_flow = totalFlow;

    for (int u : m_network_.vertexIds()) {
        for (int v : m_network_.neighbors(u)) {
            double flow = m_network_.getFlow(u, v);
            double capacity = m_network_.getCapacity(u, v);
            if (capacity > 0) {
                snapshot.edges.emplace_back(u, v, flow, capacity);
            }
        }
    }

    m_snapshots_.push_back(snapshot);
}

double MaxFlow::fordFulkerson(int source, int sink, bool enableLogging) {
    double max_flow = 0.0;
    std::unordered_map<int, int> parent;
    int step = 0;

    resetFlows(m_network_);
    m_snapshots_.clear();

    if (enableLogging) {
        captureSnapshot(step++, max_flow);
    }

    while (bfs(source, sink, parent)) {
        auto calc_residual = [this](int u, int v) { return m_network_.getResidualCapacity(u, v); };

        double path_flow = PathUtils<double>::getMinPathValue(source, sink, parent, calc_residual);

        PathUtils<double>::forEachEdgeInPath(source, sink, parent, [this, path_flow](int u, int v) {
            m_network_.addFlow(u, v, path_flow);
        });

        max_flow += path_flow;

        if (enableLogging) {
            captureSnapshot(step++, max_flow);
        }
    }
    return max_flow;
}

void MaxFlow::exportSnapshots(std::string const& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    file << std::fixed << std::setprecision(2);
    file << m_snapshots_.size() << "\n";

    for (auto const& snapshot : m_snapshots_) {
        file << snapshot.step << " " << snapshot.total_flow << "\n";
        file << snapshot.edges.size() << "\n";

        for (auto const& [u, v, flow, capacity] : snapshot.edges) {
            file << u << " " << v << " " << flow << " " << capacity << "\n";
        }
    }

    file.close();
}

}  // namespace graph
