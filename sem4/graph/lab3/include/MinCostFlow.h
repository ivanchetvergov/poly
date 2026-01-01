#pragma once

#include "FlowNetwork.h"

namespace graph {

class MinCostFlow {
public:
    explicit MinCostFlow(FlowNetwork& network) : m_network(network) {}
    
    struct Result {
        double flow{0.0};
        double cost{0.0};
        bool success{false};
    };

    [[nodiscard]] Result findMinCostFlow(int source, int sink, double targetFlow);
    
private:
    FlowNetwork& m_network;

    [[nodiscard]] bool bellmanFord(int source, int sink, 
                                    std::unordered_map<int, double>& dist,
                                    std::unordered_map<int, int>& parent);
};

} // namespace graph
