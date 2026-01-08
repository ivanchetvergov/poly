#include "Visualizer.h"
#include "Graph.h"
#include "../../lab3/include/FlowNetwork.h"
#include <fstream>
#include <iostream>

namespace graph {

void Visualizer::exportEdges(const Graph& graph, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    for (const auto& edge : graph.edges()) {
        out << edge.from << " " << edge.to << " " << edge.weight << "\n";
    }
}

void Visualizer::exportPath(const std::vector<int>& path, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    for (size_t i = 0; i < path.size(); ++i) {
        out << path[i];
        if (i + 1 < path.size()) out << " ";
    }
    out << "\n";
}

void Visualizer::exportAddedEdges(const std::vector<std::pair<int, int>>& addedEdges, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    for (const auto& [u, v] : addedEdges) {
        out << u << " " << v << "\n";
    }
}

void Visualizer::exportMatrix(const std::vector<std::vector<double>>& matrix, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    for (const auto& row : matrix) {
        for (size_t j = 0; j < row.size(); ++j) {
            out << row[j];
            if (j + 1 < row.size()) out << " ";
        }
        out << "\n";
    }
}

void Visualizer::exportFlow(const FlowNetwork& network, const std::string& filename) {
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

int Visualizer::runPythonScript(const std::string& scriptName,
                                 const std::string& inputFile,
                                 const std::string& outputFile,
                                 const std::string& extraArg)
{
    std::string cmd = "./venv/bin/python scripts/" + scriptName + " " + inputFile + " " + outputFile;
    if (!extraArg.empty()) {
        cmd += " " + extraArg;
    }
    int ret = system(cmd.c_str());
    if (ret != 0) {
        std::cerr << "[FAIL] Не удалось запустить Python-скрипт: " << scriptName << "\n";
    }
    return ret;
}

} // namespace graph
