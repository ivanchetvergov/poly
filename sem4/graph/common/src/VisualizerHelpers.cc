#include "Graph.h"
#include "Visualizer.h"

#include "../../lab3/include/FlowNetwork.h"

#include <fstream>
#include <iostream>

namespace graph {

void Visualizer::exportEdges(Graph const& graph, std::string const& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    for (auto const& edge : graph.edges()) {
        out << edge.from << " " << edge.to << " " << edge.weight << "\n";
    }
}

void Visualizer::exportPath(std::vector<int> const& path, std::string const& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    for (size_t i = 0; i < path.size(); ++i) {
        out << path[i];
        if (i + 1 < path.size())
            out << " ";
    }
    out << "\n";
}

void Visualizer::exportPaths(std::vector<std::vector<int>> const& paths,
                             std::string const& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    out << paths.size() << "\n";
    for (auto const& path : paths) {
        for (size_t i = 0; i < path.size(); ++i) {
            out << path[i];
            if (i + 1 < path.size())
                out << " ";
        }
        out << "\n";
    }
}

void Visualizer::exportAddedEdges(std::vector<std::pair<int, int>> const& addedEdges,
                                  std::string const& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    for (auto const& [u, v] : addedEdges) {
        out << u << " " << v << "\n";
    }
}

void Visualizer::exportMatrix(std::vector<std::vector<double>> const& matrix,
                              std::string const& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
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

void Visualizer::exportFlow(FlowNetwork const& network, std::string const& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
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

void Visualizer::exportColors(std::vector<int> const& colors, std::string const& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    for (size_t i = 0; i < colors.size(); ++i) {
        out << i << " " << colors[i] << "\n";
    }
    out.close();
}

}  // namespace graph
