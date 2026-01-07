#pragma once

#include "../../lab3/include/FlowNetwork.h"
#include <memory>

namespace lab3 {

using namespace graph;

struct MinCostResult {
    double cost;
    double flow;
};

class Runner {
public:
    Runner() = default;
    explicit Runner(FlowNetwork* network) : network_(network) {}

    void setNetwork(FlowNetwork* network) { network_ = network; }
    const FlowNetwork* getNetwork() const;

    double findMaxFlow(int source, int sink);
    MinCostResult findMinCostFlow(int source, int sink, double targetFlow);

    double getLastMaxFlow() const { return lastMaxFlow_; }

private:
    FlowNetwork* network_ = nullptr;
    double lastMaxFlow_ = 0.0;
};

} // namespace lab3
