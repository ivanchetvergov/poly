#pragma once

#include "FlowNetwork.h"
#include <queue>

namespace graph {

class MaxFlow {
public:
    explicit MaxFlow(FlowNetwork& network) : m_network(network) {}

    [[nodiscard]] double fordFulkerson(int source, int sink);

private:
    FlowNetwork& m_network;

    [[nodiscard]] bool bfs(int source, int sink, std::unordered_map<int, int>& parent);
};

} // namespace graph
