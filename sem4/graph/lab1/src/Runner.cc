#include "../include/Runner.h"

#include "../include/PathCounter.h"
#include "../include/ShimbellMethod.h"

#include <iostream>

#include <Generator.h>
#include <PathUtils.h>
#include <Visualizer.h>
#include <FileHandler.h>
#include <DrawDataConfig.h>
#include <Menu.h>

namespace lab1 {

using graph::PathCounter;
using graph::ShimbellMethod;
using graph::Generator;
using graph::Visualizer;
using graph::FileHandler;
using graph::DrawDataConfig;
using graph::CollectionUtils;
using graph::VisualizationType;

std::unique_ptr<Graph> Runner::generateGraph() {
    bool is_directed = static_cast<bool>(graph::readInt("Ориентированный граф? (1 - да, 0 - нет): "));
    int num_vertices = graph::readInt("Количество вершин: ");
    int num_edges = graph::readInt("Количество рёбер: ");
    auto graph = gen_.generateAcyclicGraph(num_vertices, num_edges, is_directed);
    auto data = DrawDataConfig::getConfigs().at(1);
    FileHandler::saveGraph(data.txtFile, *graph);
    Visualizer::draw(data, graph->isDirected(), graph::VisualizationType::Graph);
    std::cout << "[OK] Граф отрисован в assets/png/01_graph.png\n";
    return graph;
}

void Runner::runVisualizeAdjacencyMatrix(Graph const& graph) {
    auto data = DrawDataConfig::getConfigs().at(2);
    FileHandler::saveAdjacencyMatrix(data.txtFile, graph);
    Visualizer::drawMatrix(data, "Матрица смежности");
    std::cout << "[OK] Матрица смежности сохранена в assets/png/02_adjacency_matrix.png\n";
}

void Runner::runVisualizeWeightMatrix(Graph const& graph) {
    auto data = DrawDataConfig::getConfigs().at(3);
    FileHandler::saveWeightMatrix(data.txtFile, graph);
    Visualizer::drawMatrix(data, "Матрица весов");
    std::cout << "[OK] Матрица весов сохранена в assets/png/03_weight_matrix.png\n";
}

void Runner::runShimbellMethod(Graph const& graph) {
    std::cout << "=== Метод Шимбелла ===\n";
    int path_length = graph::readInt("Длина пути: ");

    ShimbellMethod shimbell(graph);
    auto result = shimbell.compute(path_length);

    auto dataMin = DrawDataConfig::getConfigs().at(13);
    auto doubleMin = CollectionUtils::convertOptionalMatrixToDouble(result.min_distances);
    FileHandler::saveMatrix(dataMin.txtFile, doubleMin);
    Visualizer::drawMatrix(dataMin, "Матрица Шимбелла (минимум)");

    auto dataMax = DrawDataConfig::getConfigs().at(14);
    auto doubleMax = CollectionUtils::convertOptionalMatrixToDouble(result.max_distances);
    FileHandler::saveMatrix(dataMax.txtFile, doubleMax);
    Visualizer::drawMatrix(dataMax, "Матрица Шимбелла (максимум)");

    std::cout << "[OK] Матрицы Шимбелла сохранены в assets/png/13_shimbell_min.png и 14_shimbell_max.png\n";
}

void Runner::runPathsMethod(Graph const& graph) {
    std::cout << "=== Подсчёт путей ===\n";
    int from = graph::readInt("Начальная вершина: ");
    int to = graph::readInt("Конечная вершина: ");

    if (!graph.hasVertex(from) || !graph.hasVertex(to)) {
        std::cout << "[FAIL] Вершины не существуют\n";
        return;
    }

    PathCounter counter(graph);
    auto all_paths = counter.getAllPaths(from, to);
    int count = counter.getPathCount(from, to);

    if (!all_paths.empty()) {
        auto data = DrawDataConfig::getConfigs().at(12);
        data.paths = all_paths;
        FileHandler::saveGraph(data.txtFile, graph);
        FileHandler::savePaths(data.txtPathsFile, data.paths);
        Visualizer::drawPaths(data, graph.isDirected(), graph::VisualizationType::Graph);
        std::cout << "[OK] Пути визуализированы в assets/png/12_paths.png\n";
        std::cout << "Количество путей: " << count << "\n";
    } else {
        std::cout << "[FAIL] Не удалось найти пути\n";
    }

}

}  // namespace lab1
