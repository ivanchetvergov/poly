#include "../include/Runner.h"
#include "../include/Comparator.h"

#include <BFS.h>
#include <Dijkstra.h>
#include <DrawDataConfig.h>
#include <FileHandler.h>
#include <PathUtils.h>
#include <Utils.h>
#include <Visualizer.h>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <limits>

namespace lab2 {

void Runner::runBFS(Graph const& graph) {
    int start = graph::readInt("Начальная вершина: ");
    if (!graph.hasVertex(start)) {
        std::cout << "[FAIL] Вершина не существует\n";
        return;
    }

    graph::BFS bfs(graph);
    auto result = bfs.traverse(start);

    std::cout << "\nBFS-обход от вершины " << start << ":\n";
    std::cout << "Порядок посещения: ";
    for (int v : result.visited_order) std::cout << v << " ";
    std::cout << "\n";

    std::cout << "Итераций: " << result.iterations << "\n";

    auto data = graph::DrawDataConfig::getConfigs().at(21);
    graph::FileHandler::saveGraph(data.txtFile, graph);
    graph::FileHandler::saveAddedEdges(data.txtGraphFile, result.edges_traversed);
    graph::Visualizer::drawBFSAnimation(data, graph.isDirected());
    std::cout << "[OK] BFS-анимация сохранена в assets/gif/21_bfs.gif\n";
}

void Runner::runDijkstra(Graph const& graph) {
    int start = graph::readInt("Начальная вершина: ");
    int end   = graph::readInt("Конечная вершина: ");
    if (!graph.hasVertex(start) || !graph.hasVertex(end)) {
        std::cout << "[FAIL] Вершина не существует\n";
        return;
    }

    graph::Dijkstra dijk(graph);
    auto result = dijk.compute(start);

    constexpr double INF = std::numeric_limits<double>::infinity();
    std::cout << "\nДейкстра от вершины " << start << ":\n";
    for (int v : graph.vertexIds()) {
        double d = result.dist.at(v);
        std::cout << "  " << start << " -> " << v << " : "
                  << (d == INF ? "+inf" : std::to_string(d)) << "\n";
    }
    std::cout << "Итераций: " << result.iterations << "\n";

    if (result.dist.at(end) < INF) {
        auto path = graph::PathUtils<>::reconstructPath(start, end, result.parent);
        auto data = graph::DrawDataConfig::getConfigs().at(22);
        data.paths = {path};
        graph::FileHandler::saveGraph(data.txtFile, graph);
        graph::FileHandler::savePaths(data.txtPathsFile, data.paths);
        graph::Visualizer::drawPaths(data, graph.isDirected(), graph::VisualizationType::Graph);
        std::cout << "[OK] Путь Дейкстры визуализирован в assets/png/22_dijkstra.png\n";
    } else {
        std::cout << "[FAIL] Путь от " << start << " до " << end << " не найден\n";
    }
}

void Runner::runCompare(Graph const& graph) {
    int start = graph::readInt("Начальная вершина: ");
    if (!graph.hasVertex(start)) {
        std::cout << "[FAIL] Вершина не существует\n";
        return;
    }

    auto results = graph::Comparator::compare(graph, start);

    std::cout << "\nСравнение алгоритмов (старт: " << start << "):\n";
    std::cout << std::left << std::setw(16) << "Алгоритм"
              << std::setw(14) << "Итерации" << "\n";
    std::cout << std::string(30, '-') << "\n";
    for (auto const& r : results)
        std::cout << std::left << std::setw(16) << r.algorithm_name
                  << std::setw(14) << r.iterations << "\n";

    auto data = graph::DrawDataConfig::getConfigs().at(23);
    {
        std::ofstream f(data.txtFile);
        for (auto const& r : results)
            f << r.algorithm_name << " " << r.iterations << "\n";
    }
    graph::Visualizer::drawCompare(data);
    std::cout << "[OK] Диаграмма сравнения сохранена в assets/png/23_compare.png\n";
}

}  // namespace lab2
