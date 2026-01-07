#pragma once
#include <Graph.h>
#include <GraphGenerator.h>
#include <Utils.h>
#include "include/PathCounter.h"
#include "include/ShimbellMethod.h"
#include <iostream>
#include <memory>

namespace lab1 {

using namespace graph;

void runShimbellMethod(const Graph& graph) {
    std::cout << "\n=== Метод Шимбелла ===\n";

    int pathLength = readInt("Введите длину пути (количество ребер): ");

    if (pathLength <= 0) {
        std::cout << "Длина пути должна быть положительной.\n";
        return;
    }

    ShimbellMethod shimbellMethod(graph);
    auto result = shimbellMethod.compute(pathLength);

    ShimbellMethod::printMatrix(result.minDistances,
        "Матрица минимальных расстояний");
    ShimbellMethod::printMatrix(result.maxDistances,
        "Матрица максимальных расстояний");
}

void countPathsBetweenVertices(const Graph& graph) {
    std::cout << "\n=== Подсчёт количества маршрутов ===\n";

    int from = readInt("Введите начальную вершину: ");
    int to = readInt("Введите конечную вершину: ");

    if (!graph.hasVertex(from) || !graph.hasVertex(to)) {
        std::cout << "Вершина не существует в графе.\n";
        return;
    }

    PathCounter counter(graph);
    auto pathCount = counter.countPaths(from, to);

    if (!pathCount.has_value() || pathCount.value() == 0) {
        std::cout << "Маршрут от вершины " << from << " до вершины " << to
                  << " не существует.\n";
    } else {
        std::cout << "Количество маршрутов от вершины " << from << " до вершины " << to
                  << ": " << pathCount.value() << "\n";
    }
}

void runLab1UI() {
    try {
        std::cout << "\n=== Лабораторная 1 ===\n";
        int numVertices = readInt("Введите количество вершин: ");
        int numEdges = readInt("Введите количество ребер: ");

        if (numVertices <= 0) {
            std::cout << "Количество вершин должно быть положительным.\n";
            return;
        }

        std::cout << "\nГенерация графа...\n";
        auto graph = generateRandomGraph(numVertices, numEdges);

        graph->printGraphInfo();
        graph->printGraph();

        runShimbellMethod(*graph);

        bool continueCountingPaths = true;
        while (continueCountingPaths) {
            countPathsBetweenVertices(*graph);

            std::cout << "\nПодсчитать пути для других вершин? (1 - да, 0 - нет): ";
            int choice;
            std::cin >> choice;
            continueCountingPaths = (choice == 1);
        }

        std::cout << "\nРабота программы завершена.\n";

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}

void Menu() {
    runLab1UI();
}

} // namespace lab1
