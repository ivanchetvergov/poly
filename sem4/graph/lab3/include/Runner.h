#pragma once

#include "../../lab3/include/FlowNetwork.h"

#include <memory>

namespace lab3 {

using graph::FlowNetwork;

class Runner {
public:
    Runner() = default;

    void runGenerateFlowNetwork();
    void runGenerateFlowNetworkByDegrees();
    void runMaxFlow();
    void runMinCostFlow();
    void runVisualizeCapacityMatrix();
    void runVisualizeCostMatrix();

private:
    std::unique_ptr<FlowNetwork> network_;
    double last_max_flow_ = 0.0;
};

}  // namespace lab3
