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

        std::cout << "\n[1] Генерация связного ациклического графа\n";
        int numVertices = readInt("Количество вершин: ");

        if (numVertices < 2) {
            std::cerr << "Ошибка: необходимо минимум 2 вершины\n";
            return 1;
        }

        FlowNetwork network;
        network.generateFromTree(numVertices);

        std::cout << "[OK] Граф сгенерирован (связное дерево)\n";
        network.printCapacities();
        network.printCosts();

        // Максимальный поток
        std::cout << "\n[2] Поиск максимального потока (Форд-Фалкерсон)\n";
        int source = readInt("Исток: ");
        int sink = readInt("Сток: ");

        if (source == sink) {
            std::cerr << "Ошибка: исток и сток должны быть различными\n";
            return 1;
        }

        MaxFlow maxFlowAlgo(network);
        double maxFlow = maxFlowAlgo.fordFulkerson(source, sink);

        std::cout << "\n[OK] Максимальный поток: " << std::fixed << std::setprecision(2)
                  << maxFlow << "\n";

        // Поток минимальной стоимости (2/3 от максимального)
        double targetFlow = (2.0 / 3.0) * maxFlow;

        std::cout << "\n[3] Поиск потока минимальной стоимости (Беллман-Форд)\n";
        std::cout << "Целевой поток (2/3 от max): " << std::fixed << std::setprecision(2)
                  << targetFlow << "\n";

        MinCostFlow minCostFlowAlgo(network);
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

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
