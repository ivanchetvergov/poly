#include "../include/Runner.h"

#include "../include/MaxFlow.h"
#include "../include/MinCostFlow.h"

#include <iomanip>
#include <iostream>

#include <Generator.h>

namespace lab3 {

using graph::MaxFlow;
using graph::MinCostFlow;

FlowNetwork const* Runner::getNetwork() const {
    return network_;
}

double Runner::findMaxFlow(int source, int sink) {
    if (!network_) {
        throw std::runtime_error("Network not generated");
    }

    if (source == sink) {
        throw std::invalid_argument("Source and sink must be different");
    }

    MaxFlow max_flow_algo(*network_);
    last_max_flow_ = max_flow_algo.fordFulkerson(source, sink, true);

    max_flow_algo.exportSnapshots("assets/txt/flow_snapshots.txt");

    std::cout << "\n[OK] Максимальный поток: " << std::fixed << std::setprecision(2) << last_max_flow_
              << "\n";

    return last_max_flow_;
}

MinCostResult Runner::findMinCostFlow(int source, int sink, double targetFlow) {
    if (!network_) {
        throw std::runtime_error("Network not generated");
    }

    if (source == sink) {
        throw std::invalid_argument("Source and sink must be different");
    }

    MinCostFlow min_cost_flow_algo(*network_);
    auto result = min_cost_flow_algo.findMinCostFlow(source, sink, targetFlow);

    last_min_cost_result_ = {.cost = result.cost, .flow = result.flow, .path = result.path};

    return last_min_cost_result_;
}

}  // namespace lab3
