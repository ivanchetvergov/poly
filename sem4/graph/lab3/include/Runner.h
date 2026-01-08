#pragma once

#include "../../lab3/include/FlowNetwork.h"
#include <memory>

namespace lab3 {

using namespace graph;

struct MinCostResult {
    double cost;
    double flow;
    std::vector<int> path;
};

class Runner {
public:
    Runner() = default;
    explicit Runner(FlowNetwork* network) : network_(network) {}

    void setNetwork(FlowNetwork* network) { network_ = network; }
    const FlowNetwork* getNetwork() const;

    [[nodiscard]] double findMaxFlow(int source, int sink);
    [[nodiscard]] MinCostResult findMinCostFlow(int source, int sink, double targetFlow);

    [[nodiscard]] double getLastMaxFlow() const { return lastMaxFlow_; }
    [[nodiscard]] MinCostResult getLastMinCostResult() const { return lastMinCostResult_; }

private:
    FlowNetwork* network_ = nullptr;

    double lastMaxFlow_ = 0.0;
    MinCostResult lastMinCostResult_{0.0, 0.0, {}};
};

} // namespace lab3
