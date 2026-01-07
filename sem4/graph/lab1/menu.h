#pragma once
#include <Graph.h>
#include <GraphGenerator.h>
#include <Utils.h>
#include <Visualizer.h>
#include "include/PathCounter.h"
#include "include/ShimbellMethod.h"
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace lab1 {

using namespace graph;

struct LastPathResult {
    int from, to;
    std::vector<int> path;
};

std::optional<LastPathResult> lastFoundPath;

std::optional<std::vector<int>> findPath(const Graph& g, int from, int to) {
    std::unordered_map<int, int> parent{{from, -1}};
    std::queue<int> q; q.push(from);

    while (!q.empty()) {
        int curr = q.front(); q.pop();
        if (curr == to) {
            std::vector<int> path;
            for (int v = to; v != -1; v = parent[v]) path.push_back(v);
            std::reverse(path.begin(), path.end());
            return path;
        }
        for (const auto& [nb, _] : g.neighbors(curr)) {
            if (parent.find(nb) == parent.end()) {
                parent[nb] = curr;
                q.push(nb);
            }
        }
    }
    return std::nullopt;
}

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
        lastFoundPath = std::nullopt;
    } else {
        std::cout << "Количество маршрутов от вершины " << from << " до вершины " << to
                  << ": " << pathCount.value() << "\n";

        auto path = findPath(graph, from, to);
        if (path.has_value()) {
            lastFoundPath = LastPathResult{from, to, path.value()};
            std::cout << "Один из путей: ";
            for (size_t i = 0; i < path->size(); ++i) {
                std::cout << (*path)[i] << (i + 1 < path->size() ? " -> " : "\n");
            }
        }
    }
}

void visualizeFoundPath(const Graph& graph) {
    if (!lastFoundPath.has_value()) {
        std::cout << "[FAIL] Сначала найдите путь между вершинами (пункт 12)\n";
        return;
    }

    std::string title = "Путь от " + std::to_string(lastFoundPath->from) +
                       " до " + std::to_string(lastFoundPath->to);
    Visualizer::drawGraphWithPath(graph, lastFoundPath->path,
                                  "assets/path.png", title);
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
