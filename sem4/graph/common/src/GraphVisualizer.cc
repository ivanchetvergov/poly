#include "GraphVisualizer.h"
#include "Graph.h"
#include "../../lab3/include/FlowNetwork.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

namespace graph {

void GraphVisualizer::drawGraph(const Graph& graph, const std::string& outputFile) {
    exportEdges(graph, "assets/graph.txt");
    if (runPythonScript("plot_graph.py", "assets/graph.txt", outputFile) == 0) {
        std::cout << "[OK] Граф сохранён в " << outputFile << "\n";
    } else {
        std::cerr << "[FAIL] Не удалось сохранить граф в " << outputFile << "\n";
    }
}

void GraphVisualizer::drawAdjacencyMatrix(const Graph& graph, const std::string& outputFile) {
    auto vertices = graph.vertexIds();
    size_t n = vertices.size();
    std::vector<std::vector<double>> matrix(n, std::vector<double>(n, 0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (graph.hasEdge(vertices[i], vertices[j])) {
                matrix[i][j] = 1;
            }
        }
    }

    exportMatrix(matrix, "assets/matrix.txt");
    if (runPythonScript("plot_matrix.py", "assets/matrix.txt", outputFile) == 0) {
        std::cout << "[OK] Матрица смежности сохранена в " << outputFile << "\n";
    }
}

void GraphVisualizer::drawWeightMatrix(const Graph& graph, const std::string& outputFile) {
    auto vertices = graph.vertexIds();
    size_t n = vertices.size();
    std::vector<std::vector<double>> matrix(n, std::vector<double>(n, 0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            auto weight = graph.getEdgeWeight(vertices[i], vertices[j]);
            if (weight) {
                matrix[i][j] = *weight;
            }
        }
    }

    exportMatrix(matrix, "assets/matrix.txt");
    if (runPythonScript("plot_matrix.py", "assets/matrix.txt", outputFile) == 0) {
        std::cout << "[OK] Матрица весов сохранена в " << outputFile << "\n";
    }
}

void GraphVisualizer::exportEdges(const Graph& graph, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    for (const auto& edge : graph.edges()) {
        out << edge.from << " " << edge.to << " " << edge.weight << "\n";
    }
}

void GraphVisualizer::exportMatrix(const std::vector<std::vector<double>>& matrix, const std::string& filename) {
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

void GraphVisualizer::drawFlowNetwork(const FlowNetwork& network, const std::string& outputFile) {
    exportFlow(network, "assets/flow.txt");
    if (runPythonScript("plot_flow.py", "assets/flow.txt", outputFile) == 0) {
        std::cout << "[OK] Визуализация потока сохранена в " << outputFile << "\n";
    }
}

void GraphVisualizer::exportFlow(const FlowNetwork& network, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << filename << "\n";
        return;
    }
    auto vertices = network.vertexIds();
    for (int from : vertices) {
        for (int to : network.neighbors(from)) {
            if (from < to) {
                double capacity = network.getCapacity(from, to);
                double flow = network.getFlow(from, to);
                double cost = network.getCost(from, to);
                out << from << " " << to << " " << capacity << " " << flow << " " << cost << "\n";
            }
        }
    }
}

int GraphVisualizer::runPythonScript(const std::string& scriptName,
                                    const std::string& inputFile,
                                    const std::string& outputFile)
{
    std::string cmd = "./venv/bin/python assets/" + scriptName + " " + inputFile + " " + outputFile;
    int ret = system(cmd.c_str());
    if (ret != 0) {
        std::cerr << "[FAIL] Не удалось запустить Python-скрипт: " << scriptName << "\n";
    }
    return ret;
}

} // namespace graph
