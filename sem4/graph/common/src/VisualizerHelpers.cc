#include "Graph.h"
#include "Visualizer.h"
#include "FileHandler.h"

#include "../../lab3/include/FlowNetwork.h"

#include <fstream>
#include <iostream>

namespace graph {

void Visualizer::exportEdges(Graph const& graph, DrawData const& data) {
    std::vector<std::tuple<int, int, double>> edges;
    for (auto const& edge : graph.edges()) {
        edges.emplace_back(edge.from, edge.to, edge.weight);
    }
    if (!FileHandler::saveGraphEdges(data.txtFile, edges)) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtFile << "\n";
    }
}

void Visualizer::exportPath(DrawData const& data) {
    if (!FileHandler::savePath(data.txtPathsFile, data.path)) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtPathsFile << "\n";
    }
}

void Visualizer::exportPaths(DrawData const& data) {
    if (!FileHandler::savePaths(data.txtPathsFile, data.paths)) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtPathsFile << "\n";
    }
}

void Visualizer::exportAddedEdges(DrawData const& data) {
    if (data.txtGraphFile.empty()) {
        std::cerr << "[FAIL] txtGraphFile не указан в конфигурации\n";
        return;
    }
    if (!FileHandler::saveAddedEdges(data.txtGraphFile, data.addedEdges)) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtGraphFile << "\n";
    }
}

void Visualizer::exportMatrix(std::vector<std::vector<double>> const& matrix, DrawData const& data) {
    if (!FileHandler::saveMatrix(data.txtFile, matrix)) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtFile << "\n";
    }
}

void Visualizer::exportFlow(FlowNetwork const& network, DrawData const& data) {
    std::vector<std::tuple<int, int, double, double, double>> flows;
    auto vertices = network.vertexIds();
    for (int from : vertices) {
        for (int to : network.neighbors(from)) {
            double capacity = network.getCapacity(from, to);
            if (capacity <= 0.0) {
                continue;
            }
            if (network.isDirected() || from < to) {
                double flow = network.getFlow(from, to);
                double cost = network.getCost(from, to);
                flows.emplace_back(from, to, capacity, flow, cost);
            }
        }
    }
    if (!FileHandler::saveFlowNetwork(data.txtFile, flows)) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtFile << "\n";
    }
}

void Visualizer::exportColors(DrawData const& data, std::vector<int> const& vertices) {
    if (!FileHandler::saveColors(data.txtColorsFile, vertices, data.colors)) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtColorsFile << "\n";
    }
}

}  // namespace graph
