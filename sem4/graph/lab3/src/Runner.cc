#include "../include/Runner.h"

#include "../include/MaxFlow.h"
#include "../include/MinCostFlow.h"

#include <iomanip>
#include <iostream>

#include <Generator.h>

namespace lab3 {

using graph::MaxFlow;
using graph::MinCostFlow;

double Runner::findMaxFlow(FlowNetwork& network, int source, int sink) {
    if (source == sink) {
        throw std::invalid_argument("Source and sink must be different");
    }

    MaxFlow max_flow_algo(network);
    last_max_flow_ = max_flow_algo.fordFulkerson(source, sink, true);

    max_flow_algo.exportSnapshots("assets/txt/32_flow_snapshots.txt");

    std::cout << "\n[OK] Максимальный поток: " << std::fixed << std::setprecision(2) << last_max_flow_
              << "\n";

    return last_max_flow_;
}

MinCostResult Runner::findMinCostFlow(FlowNetwork& network, int source, int sink, double targetFlow) {
    if (source == sink) {
        throw std::invalid_argument("Source and sink must be different");
    }

    MinCostFlow min_cost_flow_algo(network);
    auto result = min_cost_flow_algo.findMinCostFlow(source, sink, targetFlow);

    last_min_cost_result_ = {.cost = result.cost, .flow = result.flow, .path = result.path};

    return last_min_cost_result_;
}

}  // namespace lab3
