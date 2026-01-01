#include <Utils.h>
#include "FlowNetwork.h"
#include "MaxFlow.h"
#include "MinCostFlow.h"

#include <iomanip>
#include <iostream>

using namespace graph;

int main() {
    try {
        printHeader("Lab 3: Потоки");
        
        int numVertices = readInt("\nКоличество вершин: ");
        
        FlowNetwork network;
        network.generateFromTree(numVertices);
        
        std::cout << "\nГраф сгенерирован\n";
        network.printCapacities();
        network.printCosts();
        
        // Максимальный поток
        std::cout << "\n=== Максимальный поток (Форд-Фалкерсон) ===\n";
        int source = readInt("Исток: ");
        int sink = readInt("Сток: ");
        
        MaxFlow maxFlowAlgo(network);
        double maxFlow = maxFlowAlgo.fordFulkerson(source, sink);
        
        std::cout << "\nМаксимальный поток: " << std::fixed << std::setprecision(2) 
                  << maxFlow << "\n";
        
        // Поток минимальной стоимости (2/3 от максимального)
        double targetFlow = (2.0 / 3.0) * maxFlow;
        
        std::cout << "\n=== Поток минимальной стоимости ===\n";
        std::cout << "Целевой поток (2/3 от max): " << std::fixed << std::setprecision(2) 
                  << targetFlow << "\n";
        
        MinCostFlow minCostFlowAlgo(network);
        auto result = minCostFlowAlgo.findMinCostFlow(source, sink, targetFlow);
        
        if (result.success) {
            std::cout << "Поток: " << result.flow << "\n";
            std::cout << "Минимальная стоимость: " << result.cost << "\n";
        } else {
            std::cout << "Не удалось достичь целевого потока\n";
            std::cout << "Достигнутый поток: " << result.flow << "\n";
            std::cout << "Стоимость: " << result.cost << "\n";
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
