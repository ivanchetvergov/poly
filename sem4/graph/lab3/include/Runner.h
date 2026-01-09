#pragma once

#include "../../lab3/include/FlowNetwork.h"

#include <memory>

namespace lab3 {

using graph::FlowNetwork;

struct MinCostResult {
    double cost;
    double flow;
    std::vector<int> path;
};

class Runner {
public:
    Runner() = default;

    [[nodiscard]] double findMaxFlow(FlowNetwork& network, int source, int sink);
    [[nodiscard]] MinCostResult findMinCostFlow(FlowNetwork& network, int source, int sink, double targetFlow);

    [[nodiscard]] double getLastMaxFlow() const { return last_max_flow_; }
    [[nodiscard]] MinCostResult getLastMinCostResult() const { return last_min_cost_result_; }

private:
    double last_max_flow_ = 0.0;
    MinCostResult last_min_cost_result_{0.0, 0.0, {}};
};

}  // namespace lab3
