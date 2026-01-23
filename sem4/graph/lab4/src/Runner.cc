#include "Runner.h"

#include <iostream>
#include <algorithm>

#include <FileHandler.h>
#include <DrawDataConfig.h>
#include <Visualizer.h>

namespace lab4 {

using graph::KirchhoffTheorem;
using graph::GraphCombinatorics;
using graph::FileHandler;
using graph::DrawDataConfig;
using graph::Visualizer;

void Runner::runCountSpanningTrees(Graph const& graph) {
    int count = KirchhoffTheorem::countSpanningTrees(graph);
    std::cout << "Количество остовных деревьев: " << count << "\n";
}

void Runner::runFindMaxIndependentSet(Graph const& graph) {
    GraphCombinatorics gc;
    auto mis = gc.findMaxIndependentSetVertices(graph);
    std::cout << "Максимальное независимое множество вершин (размер " << mis.size() << "): ";
    for (int v : mis) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    auto data = DrawDataConfig::getConfigs().at(42);
    std::vector<int> colors(graph.vertexCount(), 0);
    for (int v : mis) {
        colors[v] = 1;
    }
    data.colors = colors;
    std::vector<int> vertices(graph.vertexCount());
    for (size_t i = 0; i < graph.vertexCount(); ++i) vertices[i] = static_cast<int>(i);
    FileHandler::saveGraph(data.txtFile, graph);
    FileHandler::saveColors(data.txtColorsFile, vertices, data.colors);
    Visualizer::drawColoredGraph(data, graph.isDirected());
    std::cout << "[OK] Независимое множество вершин визуализировано\n";
}

void Runner::runFindMaxIndependentSetEdges(Graph const& graph) {
    GraphCombinatorics gc;
    auto edges = gc.findMaxIndependentSetEdges(graph);
    std::cout << "Максимальное независимое множество рёбер (размер " << edges.size() << "): ";
    for (auto const& e : edges) {
        std::cout << "(" << e.first << "," << e.second << ") ";
    }
    std::cout << "\n";

    auto data = DrawDataConfig::getConfigs().at(43);
    data.addedEdges = edges;
    FileHandler::saveGraph(data.txtFile, graph);
    FileHandler::saveAddedEdges(data.txtGraphFile, data.addedEdges);
    Visualizer::draw(data, graph.isDirected(), graph::VisualizationType::Graph);
    std::cout << "[OK] Независимое множество рёбер визуализировано\n";
}

void Runner::runFindMinVertexCover(Graph const& graph) {
    GraphCombinatorics gc;
    auto vc = gc.findMinVertexCover(graph);
    std::cout << "Минимальное вершинное покрытие (размер " << vc.size() << "): ";
    for (int v : vc) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    auto data = DrawDataConfig::getConfigs().at(44);
    std::vector<int> colors(graph.vertexCount(), 0);
    for (int v : vc) {
        colors[v] = 1;
    }
    data.colors = colors;
    std::vector<int> vertices(graph.vertexCount());
    for (size_t i = 0; i < graph.vertexCount(); ++i) vertices[i] = static_cast<int>(i);
    FileHandler::saveGraph(data.txtFile, graph);
    FileHandler::saveColors(data.txtColorsFile, vertices, data.colors);
    Visualizer::drawColoredGraph(data, graph.isDirected());
    std::cout << "[OK] Вершинное покрытие визуализировано\n";
}

void Runner::runFindMinEdgeCover(Graph const& graph) {
    GraphCombinatorics gc;
    auto ec = gc.findMinEdgeCover(graph);
    std::cout << "Минимальное рёберное покрытие (размер " << ec.size() << "): ";
    for (auto const& e : ec) {
        std::cout << "(" << e.first << "," << e.second << ") ";
    }
    std::cout << "\n";

    auto data = DrawDataConfig::getConfigs().at(45);
    data.addedEdges = ec;
    FileHandler::saveGraph(data.txtFile, graph);
    FileHandler::saveAddedEdges(data.txtGraphFile, data.addedEdges);
    Visualizer::draw(data, graph.isDirected(), graph::VisualizationType::Graph);
    std::cout << "[OK] Рёберное покрытие визуализировано\n";
}

void Runner::runFindMinColoring(Graph const& graph) {
    GraphCombinatorics gc;
    auto coloring = gc.findMinColoring(graph);
    int chromatic_number = 0;
    for (int color : coloring) {
        chromatic_number = std::max(chromatic_number, color + 1);
    }
    std::cout << "Хроматическое число: " << chromatic_number << "\n";
    std::cout << "Раскраска: ";
    for (size_t i = 0; i < coloring.size(); ++i) {
        std::cout << "v" << i << ":" << coloring[i] << " ";
    }
    std::cout << "\n";

    auto data = DrawDataConfig::getConfigs().at(46);
    data.colors = coloring;
    std::vector<int> vertices(graph.vertexCount());
    for (size_t i = 0; i < graph.vertexCount(); ++i) vertices[i] = static_cast<int>(i);
    FileHandler::saveGraph(data.txtFile, graph);
    FileHandler::saveColors(data.txtColorsFile, vertices, data.colors);
    Visualizer::drawColoredGraph(data, graph.isDirected());
    std::cout << "[OK] Раскраска графа визуализирована\n";
}

}  // namespace lab4

