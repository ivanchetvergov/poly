#pragma once
#include <Utils.h>
#include <RandomGenerator.h>
#include <GraphGenerator.h>
#include <GraphVisualizer.h>
#include "FlowNetwork.h"
#include "MaxFlow.h"
#include "MinCostFlow.h"
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

namespace lab3 {

using namespace graph;

void generateNetwork(FlowNetwork& network) {
    int numVertices = readInt("Количество вершин: ");
    if (numVertices < 2) {
        std::cout << "[FAIL] Минимум 2 вершины\n";
        return;
    }

    int numEdges = readInt("Количество ребер: ");
    if (numEdges < numVertices - 1) {
        std::cout << "[FAIL] Для связного графа нужно минимум " << (numVertices - 1) << " ребер\n";
        return;
    }

    int maxEdges = numVertices * (numVertices - 1) / 2;
    if (numEdges > maxEdges) {
        std::cout << "[FAIL] Максимум " << maxEdges << " ребер для " << numVertices << " вершин\n";
        return;
    }

    auto graph = generateRandomGraph(numVertices, numEdges);

    network = FlowNetwork();
    RandomGenerator rng;

    for (const auto& edge : graph->edges()) {
        double capacity = rng.randomDouble(5.0, 20.0);
        double cost = rng.randomDouble(1.0, 10.0);
        network.addEdge(edge.from, edge.to, capacity, cost);
    }

    std::cout << "[OK] Сеть потоков сгенерирована (" << numEdges << " рёбер)\n\n";
    network.printCapacities();
    network.printCosts();
}

double findMaxFlow(FlowNetwork& network) {
    int source = readInt("Исток: ");
    int sink = readInt("Сток: ");
    if (source == sink) {
        std::cerr << "Ошибка: исток и сток должны быть различными\n";
        return 0.0;
    }
    MaxFlow maxFlowAlgo(network);
    double maxFlow = maxFlowAlgo.fordFulkerson(source, sink);
    std::cout << "\n[OK] Максимальный поток: " << std::fixed << std::setprecision(2)
              << maxFlow << "\n";
    return maxFlow;
}

void findMinCostFlow(FlowNetwork& network, double lastMaxFlow) {
    int source = readInt("Исток: ");
    int sink = readInt("Сток: ");
    if (source == sink) {
        std::cerr << "Ошибка: исток и сток должны быть различными\n";
        return;
    }
    if (lastMaxFlow == 0.0) {
        std::cout << "Сначала найдите максимальный поток!\n";
        return;
    }
    double targetFlow = (2.0 / 3.0) * lastMaxFlow;
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
}

void printGraphInfo(const FlowNetwork& network) {
    std::cout << "\n=== Информация о графе ===\n";
    network.printCapacities();
    network.printCosts();
}

void Menu(FlowNetwork& network) {
    bool running = true;
    double lastMaxFlow = 0.0;
    while (running) {
        printHeader("Lab 3: Потоки");
        std::cout << "\n1 - Генерация связного ациклического графа";
        std::cout << "\n2 - Поиск максимального потока (Форд-Фалкерсон)";
        std::cout << "\n3 - Поиск потока минимальной стоимости (Беллман-Форд)";
        std::cout << "\n4 - Вывести граф (матрицы пропускных способностей и стоимостей)";
        std::cout << "\n5 - Визуализировать поток (сохранить в .png)";
        std::cout << "\n0 - Назад\n";
        int choice = readInt("Ваш выбор: ");
        switch (choice) {
            case 1: generateNetwork(network); lastMaxFlow = 0.0; break;
            case 2: lastMaxFlow = findMaxFlow(network); break;
            case 3: findMinCostFlow(network, lastMaxFlow); break;
            case 4: printGraphInfo(network); break;
            case 5: GraphVisualizer::drawFlowNetwork(network, "assets/flow.png"); break;
            case 0: running = false; break;
            default: std::cout << "Неверный выбор!\n";
        }
    }
}

} // namespace lab3
