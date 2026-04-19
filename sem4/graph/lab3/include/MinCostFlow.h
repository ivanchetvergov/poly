#pragma once

#include "FlowNetwork.h"

namespace graph {

class MinCostFlow {
public:
    explicit MinCostFlow(FlowNetwork& network) : m_network_(network) {}

    struct AugmentationStep {
        int iteration{0};
        std::vector<int> path;
        double pathFlow{0.0};
        double pathCost{0.0};
        double cumulativeFlow{0.0};
        double cumulativeCost{0.0};
    };

    struct Result {
        double flow{0.0};
        double cost{0.0};
        bool success{false};
        std::vector<int> path;
        std::vector<AugmentationStep> steps;
    };

    [[nodiscard]] Result findMinCostFlow(int source, int sink, double targetFlow);

private:
    FlowNetwork& m_network_;

    [[nodiscard]] bool bellmanFord(int source, int sink, std::unordered_map<int, double>& dist,
                                   std::unordered_map<int, int>& parent);
};

}  // namespace graph
