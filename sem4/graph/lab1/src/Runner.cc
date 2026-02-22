#include "../include/Runner.h"

#include "../include/PathCounter.h"
#include "../include/ShimbellMethod.h"
#include "../include/GraphMetrics.h"

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

std::unique_ptr<Graph> Runner::generateRayleighGraph() {
    bool is_directed = static_cast<bool>(graph::readInt("Ориентированный граф? (1 - да, 0 - нет): "));
    int num_vertices = graph::readInt("Количество вершин: ");
    int num_edges = graph::readInt("Количество рёбер: ");
    int z = graph::readInt("Параметр распределения Рэлея (z > 0, например 1): ");
    auto graph = gen_.generateAcyclicGraph(num_vertices, num_edges, is_directed, false, z);
    auto data = DrawDataConfig::getConfigs().at(1);
    FileHandler::saveGraph(data.txtFile, *graph);
    Visualizer::draw(data, graph->isDirected(), graph::VisualizationType::Graph);
    std::cout << "[OK] Граф (распределение Рэлея, z=" << z << ") отрисован в assets/png/01_graph.png\n";
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

void Runner::runGraphMetrics(Graph const& graph) {
    std::cout << "=== Метрики графа ===\n";

    graph::GraphMetrics metrics(graph);
    auto result = metrics.compute();

    std::cout << "\nЭксцентриситеты вершин:\n";
    for (int v : graph.vertexIds()) {
        double ecc = result.eccentricities.at(v);
        std::cout << "  v" << v << " : " << (ecc == std::numeric_limits<double>::infinity() ? "inf" : std::to_string(ecc)) << "\n";
    }

    std::cout << "\nРадиус графа   : " << result.radius   << "\n";
    std::cout << "Диаметр графа  : " << result.diameter  << "\n";

    std::cout << "\nЦентральные вершины (эксцентриситет = радиус): ";
    for (int v : result.center) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "Диаметральные вершины (эксцентриситет = диаметр): ";
    for (int v : result.diametralVerts) std::cout << v << " ";
    std::cout << "\n";

    std::vector<int> ids = graph.vertexIds();
    std::vector<int> colors(ids.size(), 0);
    for (int i = 0; i < static_cast<int>(ids.size()); ++i) {
        int v = ids[i];
        bool is_center    = std::find(result.center.begin(),
                                        result.center.end(), v) != result.center.end();
        bool is_diametral = std::find(result.diametralVerts.begin(), result.diametralVerts.end(), v) != result.diametralVerts.end();
        if (is_center)    colors[i] = 1;
        else if (is_diametral) colors[i] = 2;
    }

    auto data = DrawDataConfig::getConfigs().at(15);
    FileHandler::saveGraph(data.txtFile, graph);
    FileHandler::saveColors(data.txtColorsFile, ids, colors);
    data.colorLabels = {"Обычные вершины", "Центр графа (радиус)", "Диаметральные вершины"};
    Visualizer::drawColoredGraph(data, graph.isDirected());
    std::cout << "[OK] Визуализация метрик в assets/png/15_graph_metrics.png\n";
    std::cout << "     Цвет 1 = центр, Цвет 2 = диаметральные, Цвет 0 = остальные\n";
}

}  // namespace lab1
