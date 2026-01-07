#include "../include/Runner.h"
#include "../include/MaxFlow.h"
#include "../include/MinCostFlow.h"
#include <Generator.h>
#include <iostream>
#include <iomanip>

namespace lab3 {

const FlowNetwork* Runner::getNetwork() const {
    return network_;
}

double Runner::findMaxFlow(int source, int sink) {
    if (!network_) {
        throw std::runtime_error("Network not generated");
    }

    if (source == sink) {
        throw std::invalid_argument("Source and sink must be different");
    }

    MaxFlow maxFlowAlgo(*network_);
    lastMaxFlow_ = maxFlowAlgo.fordFulkerson(source, sink);

    std::cout << "\n[OK] Максимальный поток: " << std::fixed << std::setprecision(2)
              << lastMaxFlow_ << "\n";

    return lastMaxFlow_;
}

MinCostResult Runner::findMinCostFlow(int source, int sink, double targetFlow) {
    if (!network_) {
        throw std::runtime_error("Network not generated");
    }

    if (source == sink) {
        throw std::invalid_argument("Source and sink must be different");
    }

    MinCostFlow minCostFlowAlgo(*network_);
    auto result = minCostFlowAlgo.findMinCostFlow(source, sink, targetFlow);

    if (result.success) {
        std::cout << "\n[OK] Результат:\n";
        std::cout << "  Поток: " << result.flow << "\n";
        std::cout << "  Минимальная стоимость: " << result.cost << "\n";
    } else {
        std::cout << "\n[WARN] Не удалось достичь целевого потока\n";
        std::cout << "  Достигнутый поток: " << result.flow << "\n";
        std::cout << "  Стоимость: " << result.cost << "\n";
    }

    return {result.cost, result.flow};
}

} // namespace lab3
