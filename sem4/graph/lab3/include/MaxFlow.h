#pragma once

#include "FlowNetwork.h"
#include <queue>
#include <vector>
#include <string>

namespace graph {

struct FlowSnapshot {
    int step;
    double totalFlow;
    std::vector<std::tuple<int, int, double, double>> edges; // (u, v, flow, capacity)
};

class MaxFlow {
public:
    explicit MaxFlow(FlowNetwork& network) : m_network(network) {}

    [[nodiscard]] double fordFulkerson(int source, int sink, bool enableLogging = false);

    const std::vector<FlowSnapshot>& getSnapshots() const { return m_snapshots; }
    void exportSnapshots(const std::string& filename) const;

private:
    FlowNetwork& m_network;
    std::vector<FlowSnapshot> m_snapshots;

    [[nodiscard]] bool bfs(int source, int sink, std::unordered_map<int, int>& parent);
    void captureSnapshot(int step, double totalFlow);
};

} // namespace graph
