#include "Graph.h"
#include "Visualizer.h"

#include "../../lab3/include/FlowNetwork.h"

#include <fstream>
#include <iostream>

namespace graph {

void Visualizer::exportEdges(Graph const& graph, DrawData const& data) {
    std::ofstream out(data.txtFile);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtFile << "\n";
        return;
    }
    for (auto const& edge : graph.edges()) {
        out << edge.from << " " << edge.to << " " << edge.weight << "\n";
    }
}

void Visualizer::exportPath(DrawData const& data) {
    std::ofstream out(data.txtPathsFile);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtPathsFile << "\n";
        return;
    }
    for (size_t i = 0; i < data.path.size(); ++i) {
        out << data.path[i];
        if (i + 1 < data.path.size())
            out << " ";
    }
    out << "\n";
}

void Visualizer::exportPaths(DrawData const& data) {
    std::ofstream out(data.txtPathsFile);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtPathsFile << "\n";
        return;
    }
    std::vector<std::vector<int>> non_empty_paths;
    for (auto const& path : data.paths) {
        if (!path.empty()) {
            non_empty_paths.push_back(path);
        }
    }

    out << non_empty_paths.size() << "\n";
    for (auto const& path : non_empty_paths) {
        for (size_t i = 0; i < path.size(); ++i) {
            out << path[i];
            if (i + 1 < path.size())
                out << " ";
        }
        out << "\n";
    }
}

void Visualizer::exportAddedEdges(DrawData const& data) {
    if (data.txtGraphFile.empty()) {
        std::cerr << "[FAIL] txtGraphFile не указан в конфигурации\n";
        return;
    }
    std::ofstream out(data.txtGraphFile);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtGraphFile << "\n";
        return;
    }
    for (auto const& [u, v] : data.addedEdges) {
        out << u << " " << v << "\n";
    }
}

void Visualizer::exportMatrix(std::vector<std::vector<double>> const& matrix, DrawData const& data) {
    std::ofstream out(data.txtFile);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtFile << "\n";
        return;
    }
    for (auto const& row : matrix) {
        for (size_t j = 0; j < row.size(); ++j) {
            out << row[j];
            if (j + 1 < row.size())
                out << " ";
        }
        out << "\n";
    }
}

void Visualizer::exportFlow(FlowNetwork const& network, DrawData const& data) {
    std::ofstream out(data.txtFile);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtFile << "\n";
        return;
    }
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
                out << from << " " << to << " " << capacity << " " << flow << " " << cost << "\n";
            }
        }
    }
}

void Visualizer::exportColors(DrawData const& data, std::vector<int> const& vertices) {
    std::ofstream out(data.txtColorsFile);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << data.txtColorsFile << "\n";
        return;
    }
    for (size_t i = 0; i < data.colors.size(); ++i) {
        out << vertices[i] << " " << data.colors[i] << "\n";
    }
    out.close();
}

}  // namespace graph
