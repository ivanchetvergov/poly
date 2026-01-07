#pragma once
#include <GraphGenerator.h>
#include <Utils.h>
#include <Visualizer.h>
#include "include/EulerianCycle.h"
#include "include/HamiltonianCycle.h"
#include "include/TSPSolver.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>

namespace lab5 {

using namespace graph;

std::optional<std::vector<int>> lastEulerianCycle;
std::optional<std::vector<int>> lastHamiltonianCycle;
std::optional<std::vector<int>> lastTSPCycle;

void printCycle(const std::vector<int>& cycle, const char* title) {
    std::cout << "\n" << title << ": ";
    if (cycle.empty()) {
        std::cout << "Пусто\n";
        return;
    }
    for (size_t i = 0; i < cycle.size(); ++i) {
        std::cout << cycle[i];
        if (i < cycle.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";
}

void checkEulerian(Graph& graph) {
    std::cout << "\n[2] Проверка эйлеровости графа\n";

    EulerianCycle euler(graph);

    if (euler.isEulerian()) {
        std::cout << "[OK] Граф является эйлеровым\n";
    } else if (euler.isSemiEulerian()) {
        std::cout << "[INFO] Граф является полуэйлеровым (эйлеров путь существует)\n";
    } else {
        std::cout << "[FAIL] Граф не является эйлеровым\n";
        auto oddVertices = euler.getOddDegreeVertices();
        std::cout << "Вершины нечётной степени (" << oddVertices.size() << "): ";
        for (int v : oddVertices) std::cout << v << " ";
        std::cout << "\n\nМодификация графа...\n";
        euler.makeEulerian();
        std::cout << "[OK] Граф модифицирован (добавлены рёбра)\n";
    }

    auto cycle = euler.findCycle();
    if (cycle.has_value()) {
        lastEulerianCycle = cycle.value();
        printCycle(cycle.value(), "Эйлеров цикл");
    } else {
        lastEulerianCycle = std::nullopt;
        std::cout << "Не удалось найти эйлеров цикл\n";
    }
}

void checkHamiltonian(Graph& graph) {
    std::cout << "\n[3] Проверка гамильтоновости графа\n";

    HamiltonianCycle hamilton(graph);

    if (hamilton.isHamiltonian()) {
        std::cout << "[OK] Граф является гамильтоновым\n";
    } else {
        std::cout << "[FAIL] Граф не является гамильтоновым\n\nМодификация графа...\n";
        hamilton.makeHamiltonian();
        std::cout << "[OK] Граф модифицирован (добавлены рёбра для выполнения условия Дирака)\n";
    }

    auto cycle = hamilton.findCycle();
    if (cycle.has_value()) {
        lastHamiltonianCycle = cycle.value();
        printCycle(cycle.value(), "Гамильтонов цикл");
    } else {
        lastHamiltonianCycle = std::nullopt;
        std::cout << "Не удалось найти гамильтонов цикл\n";
    }
}

void solveTSP(const Graph& graph) {
    std::cout << "\n[4] Задача коммивояжёра\n";

    TSPSolver tsp(graph);

    std::cout << "Поиск всех гамильтоновых циклов...\n";
    auto solutions = tsp.findAllCycles(100);

    if (solutions.empty()) {
        std::cout << "Гамильтоновых циклов не найдено\n";
        return;
    }

    std::cout << "Найдено циклов: " << solutions.size() << "\n";

    if (solutions.size() <= 20) {
        std::cout << "\nВсе найденные циклы:\n";
        for (size_t i = 0; i < solutions.size(); ++i) {
            std::cout << i + 1 << ". ";
            for (size_t j = 0; j < solutions[i].path.size(); ++j) {
                std::cout << solutions[i].path[j];
                if (j < solutions[i].path.size() - 1) std::cout << " -> ";
            }
            std::cout << " (стоимость: " << std::fixed << std::setprecision(2)
                      << solutions[i].cost << ")\n";
        }
    } else {
        std::cout << "\nСохранение в файл tsp_results.txt...\n";
        std::ofstream out("tsp_results.txt");
        for (size_t i = 0; i < solutions.size(); ++i) {
            out << i + 1 << ". ";
            for (size_t j = 0; j < solutions[i].path.size(); ++j) {
                out << solutions[i].path[j];
                if (j < solutions[i].path.size() - 1) out << " -> ";
            }
            out << " (стоимость: " << std::fixed << std::setprecision(2)
                << solutions[i].cost << ")\n";
        }
        out.close();
        std::cout << "[OK] Результаты сохранены\n";
    }

    std::cout << "\n[OK] Лучшее решение:\n";
    std::cout << "Стоимость: " << std::fixed << std::setprecision(2)
              << solutions[0].cost << "\n";
    printCycle(solutions[0].path, "Путь");
    lastTSPCycle = solutions[0].path;
}

void runLab5UI() {
    try {
        printHeader("Lab 5: Эйлеровы и Гамильтоновы циклы");

        std::cout << "\n[1] Генерация графа\n";
        int numVertices = readInt("Количество вершин: ");
        int numEdges = readInt("Количество рёбер: ");

        if (numVertices < 3) {
            std::cerr << "Ошибка: необходимо минимум 3 вершины\n";
            return;
        }

        auto graph = generateRandomGraph(numVertices, numEdges);

        std::cout << "[OK] Граф сгенерирован\n";
        graph->printGraphInfo();

        checkEulerian(*graph);
        checkHamiltonian(*graph);
        solveTSP(*graph);

        std::cout << "\nРабота завершена.\n";

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}

void visualizeEulerianCycle(const Graph& graph) {
    if (!lastEulerianCycle.has_value()) {
        std::cout << "[FAIL] Сначала найдите эйлеров цикл (пункт 51)\n";
        return;
    }
    Visualizer::drawGraphWithPath(graph, lastEulerianCycle.value(),
                                  "assets/euler_cycle.png", "Эйлеров цикл");
}

void visualizeHamiltonianCycle(const Graph& graph) {
    if (!lastHamiltonianCycle.has_value()) {
        std::cout << "[FAIL] Сначала найдите гамильтонов цикл (пункт 52)\n";
        return;
    }
    Visualizer::drawGraphWithPath(graph, lastHamiltonianCycle.value(),
                                  "assets/hamilton_cycle.png", "Гамильтонов цикл");
}

void visualizeTSP(const Graph& graph) {
    if (!lastTSPCycle.has_value()) {
        std::cout << "[FAIL] Сначала решите задачу коммивояжёра (пункт 53)\n";
        return;
    }
    Visualizer::drawGraphWithPath(graph, lastTSPCycle.value(),
                                  "assets/tsp_cycle.png", "TSP (лучшее решение)");
}

void Menu() {
    runLab5UI();
}

} // namespace lab5
