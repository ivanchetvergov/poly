#include "../include/Runner.h"

#include "../include/EulerianCycle.h"
#include "../include/HamiltonianCycle.h"
#include "../include/TSPSolver.h"

#include <iostream>

#include <FileHandler.h>
#include <DrawDataConfig.h>
#include <Visualizer.h>

namespace lab5 {

using graph::EulerianCycle;
using graph::HamiltonianCycle;
using graph::TSPSolver;
using graph::FileHandler;
using graph::DrawDataConfig;
using graph::Visualizer;
using graph::DrawData;
using graph::VisualizationType;

void Runner::runCheckEulerian(Graph& graph) {
    auto data = DrawDataConfig::getConfigs().at(51);
    EulerianCycle euler(graph);

    std::cout << "\n=== Проверка эйлеровости ===\n";

    if (euler.isEulerian()) {
        std::cout << "[OK] Граф является эйлеровым\n";
        auto cycle = euler.findCycle();
        if (cycle.has_value()) {
            std::cout << "Эйлеров цикл найден, длина: " << cycle->size() << "\n";

            data.path = *cycle;
            FileHandler::saveGraph(data.txtFile, graph);
            FileHandler::savePaths(data.txtPathsFile, {data.path});
            DrawData pathsData = data;
            pathsData.paths = {data.path};
            Visualizer::drawPaths(pathsData, graph.isDirected(), graph::VisualizationType::Graph);
            std::cout << "[OK] Эйлеров цикл визуализирован\n";
        }
    } else if (euler.isSemiEulerian()) {
        std::cout << "[INFO] Граф является полуэйлеровым (есть эйлеров путь, но нет цикла)\n";
        auto odd_vertices = euler.getOddDegreeVertices();
        std::cout << "Вершины с нечётной степенью: ";
        for (int v : odd_vertices) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "[INFO] Граф не является эйлеровым\n";
        auto odd_vertices = euler.getOddDegreeVertices();
        std::cout << "Вершин с нечётной степенью: " << odd_vertices.size() << "\n";

        std::cout << "\nПопытка модификации графа...\n";
        FileHandler::saveGraph(data.txtFile, graph);
        euler.makeEulerian();
        auto added_edges = euler.getAddedEdges();
        if (!added_edges.empty()) {
            std::cout << "[ИНФО] Добавлено рёбер: " << added_edges.size() << "\n";
        }
        auto cycle = euler.findCycle();
        if (cycle.has_value()) {
            std::cout << "[OK] После модификации найден эйлеров цикл, длина: " << cycle->size() << "\n";

            data.path = *cycle;
            data.addedEdges = added_edges;
            FileHandler::savePaths(data.txtPathsFile, {data.path});
            FileHandler::saveAddedEdges(data.txtPathsFile, data.addedEdges);
            DrawData pathsData = data;
            pathsData.paths = {data.path};
            Visualizer::drawPaths(pathsData, graph.isDirected(), graph::VisualizationType::Graph);
            std::cout << "[OK] Эйлеров цикл после модификации визуализирован\n";
        } else {
            std::cout << "[FAIL] Не удалось найти эйлеров цикл после модификации\n";
        }
    }
}

void Runner::runCheckHamiltonian(Graph& graph) {
    auto data = DrawDataConfig::getConfigs().at(52);
    HamiltonianCycle hamilton(graph);

    std::cout << "\n=== Проверка гамильтоновости ===\n";

    if (hamilton.isHamiltonian()) {
        std::cout << "[OK] Граф является гамильтоновым\n";
        auto cycle = hamilton.findCycle();
        if (cycle.has_value()) {
            std::cout << "Гамильтонов цикл найден\n";

            data.path = *cycle;
            FileHandler::saveGraph(data.txtFile, graph);
            FileHandler::savePaths(data.txtPathsFile, {data.path});
            DrawData pathsData = data;
            pathsData.paths = {data.path};
            Visualizer::drawPaths(pathsData, graph.isDirected(), graph::VisualizationType::Graph);
            std::cout << "[OK] Гамильтонов цикл визуализирован\n";
        }
    } else {
        std::cout << "[INFO] Граф не является гамильтоновым\n";
        std::cout << "Попытка модификации графа...\n";
        FileHandler::saveGraph(data.txtFile, graph);
        hamilton.makeHamiltonian();
        auto added_edges = hamilton.getAddedEdges();
        if (!added_edges.empty()) {
            std::cout << "[ИНФО] Добавлено рёбер: " << added_edges.size() << "\n";
        }
        auto cycle = hamilton.findCycle();
        if (cycle.has_value()) {
            std::cout << "[OK] После модификации найден гамильтонов цикл\n";

            data.path = *cycle;
            data.addedEdges = added_edges;
            FileHandler::savePaths(data.txtPathsFile, {data.path});
            FileHandler::saveAddedEdges("assets/txt/added_edges.txt", data.addedEdges);
            DrawData pathsData = data;
            pathsData.paths = {data.path};
            Visualizer::drawPaths(pathsData, graph.isDirected(), graph::VisualizationType::Graph);
            std::cout << "[OK] Гамильтонов цикл после модификации визуализирован\n";
        } else {
            std::cout << "[FAIL] Не удалось найти гамильтонов цикл после модификации\n";
        }
    }
}

void Runner::runSolveTSP(Graph const& graph) {
    std::cout << "\n=== Задача коммивояжёра (TSP) ===\n";

    TSPSolver solver(graph);
    auto result = solver.findAllCycles(100);

    if (result.empty()) {
        std::cout << "[INFO] Не найдено гамильтоновых циклов\n";
        return;
    }

    std::cout << "[OK] Найдено циклов: " << result.size() << "\n\n";

    size_t show_count = std::min(result.size(), static_cast<size_t>(5));
    std::cout << "Лучшие " << show_count << " циклов:\n";

    for (size_t i = 0; i < show_count; ++i) {
        auto const& cycle = result[i];
        std::cout << (i + 1) << ". Стоимость: " << cycle.cost << ", Путь: ";
        for (size_t j = 0; j < cycle.path.size(); ++j) {
            std::cout << cycle.path[j];
            if (j < cycle.path.size() - 1)
                std::cout << " -> ";
        }
        std::cout << "\n";
    }

    auto best_cycle = result.front().path;
    std::cout << "\nЛучший цикл визуализирован\n";

    auto data = DrawDataConfig::getConfigs().at(53);
    data.path = best_cycle;
    FileHandler::saveGraph(data.txtFile, graph);
    FileHandler::savePaths(data.txtPathsFile, {data.path});
    DrawData pathsData = data;
    pathsData.paths = {data.path};
    Visualizer::drawPaths(pathsData, graph.isDirected(), graph::VisualizationType::Graph);
    std::cout << "[OK] Лучший TSP цикл визуализирован\n";
}

}  // namespace lab5
