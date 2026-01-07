#include "Visualizer.h"
#include "Graph.h"
#include "../../lab3/include/FlowNetwork.h"
#include "CollectionUtils.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

namespace graph {

void Visualizer::drawCapacityMatrix(const FlowNetwork& network, const std::string& outputFile) {
    auto vertices = network.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices,
        [&](int i, int j){ return network.getCapacity(i, j); });

    exportMatrix(matrix, "assets/txt/matrix.txt");
    if (runPythonScript("plot_matrix.py", "assets/txt/matrix.txt", outputFile) == 0) {
        std::cout << "[OK] Матрица пропускных способностей сохранена в " << outputFile << "\n";
    }
}

void Visualizer::drawCostMatrix(const FlowNetwork& network, const std::string& outputFile) {
    auto vertices = network.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices,
        [&](int i, int j){ return network.getCost(i, j); });

    exportMatrix(matrix, "assets/txt/matrix.txt");
    if (runPythonScript("plot_matrix.py", "assets/txt/matrix.txt", outputFile) == 0) {
        std::cout << "[OK] Матрица стоимостей сохранена в " << outputFile << "\n";
    }
}

void Visualizer::drawGraph(const Graph& graph, const std::string& outputFile) {
    exportEdges(graph, "assets/txt/graph.txt");

    if (runPythonScript("plot_graph.py", "assets/txt/graph.txt", outputFile) == 0) {
        std::cout << "[OK] Граф сохранён в " << outputFile << "\n";
    } else {
        std::cerr << "[FAIL] Не удалось сохранить граф в " << outputFile << "\n";
    }
}

void Visualizer::drawGraphWithPath(const Graph& graph, const std::vector<int>& path,
                                   const std::string& outputFile, const std::string& title)
{
    exportEdges(graph, "assets/txt/graph.txt");
    exportPath(path, "assets/txt/path.txt");

    if (runPythonScript("plot_graph_path.py", "assets/txt/graph.txt", outputFile, title) == 0) {
        std::cout << "[OK] Граф с путём сохранён в " << outputFile << "\n";
    } else {
        std::cerr << "[FAIL] Не удалось сохранить граф с путём\n";
    }
}

void Visualizer::drawAdjacencyMatrix(const Graph& graph, const std::string& outputFile) {
    auto vertices = graph.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices,
        [&](int i, int j)
        { return graph.hasEdge(i, j) ? 1.0 : 0.0; });

    exportMatrix(matrix, "assets/txt/matrix.txt");
    if (runPythonScript("plot_matrix.py", "assets/txt/matrix.txt", outputFile) == 0) {
        std::cout << "[OK] Матрица смежности сохранена в " << outputFile << "\n";
    }
}

void Visualizer::drawWeightMatrix(const Graph& graph, const std::string& outputFile) {
    auto vertices = graph.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices,
        [&](int i, int j){
        auto w = graph.getEdgeWeight(i, j);
        return w ? *w : 0.0;
    });

    exportMatrix(matrix, "assets/txt/matrix.txt");
    if (runPythonScript("plot_matrix.py", "assets/txt/matrix.txt", outputFile) == 0) {
        std::cout << "[OK] Матрица весов сохранена в " << outputFile << "\n";
    }
}

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

void Visualizer::drawFlowNetwork(const FlowNetwork& network, const std::string& outputFile) {
    exportFlow(network, "assets/txt/flow.txt");
    if (runPythonScript("plot_flow.py", "assets/txt/flow.txt", outputFile) == 0) {
        std::cout << "[OK] Визуализация потока сохранена в " << outputFile << "\n";
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
            if (from < to) {
                double capacity = network.getCapacity(from, to);
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
        cmd += " \"" + extraArg + "\"";
    }
    int ret = system(cmd.c_str());
    if (ret != 0) {
        std::cerr << "[FAIL] Не удалось запустить Python-скрипт: " << scriptName << "\n";
    }
    return ret;
}

} // namespace graph
