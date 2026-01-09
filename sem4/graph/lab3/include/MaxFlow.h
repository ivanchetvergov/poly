#pragma once

#include "FlowNetwork.h"

#include <queue>
#include <string>
#include <vector>

namespace graph {

struct FlowSnapshot {
    int step;
    double total_flow;
    std::vector<std::tuple<int, int, double, double>> edges;  // (u, v, flow, capacity)
};

class MaxFlow {
public:
    explicit MaxFlow(FlowNetwork& network) : m_network_(network) {}

    [[nodiscard]] double fordFulkerson(int source, int sink, bool enableLogging = false);

    std::vector<FlowSnapshot> const& getSnapshots() const { return m_snapshots_; }
    void exportSnapshots(std::string const& filename) const;

private:
    FlowNetwork& m_network_;
    std::vector<FlowSnapshot> m_snapshots_;

    [[nodiscard]] bool bfs(int source, int sink, std::unordered_map<int, int>& parent);
    void captureSnapshot(int step, double totalFlow);
};

}  // namespace graph
