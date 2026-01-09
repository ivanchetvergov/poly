#include "Visualizer.h"

#include "CollectionUtils.h"
#include "Graph.h"
#include "Utils.h"

#include "../../lab3/include/FlowNetwork.h"

#include <fstream>
#include <iostream>

namespace graph {

static void drawGraphHelper(std::string const& scriptName, std::string const& dataFile,
                            std::string const& outputFile, std::string const& graphType,
                            std::string const& title, std::string const& defaultTitle) {
    std::string graph_title = title.empty() ? defaultTitle : title;
    std::vector<std::string> args = {dataFile, outputFile, graphType, "\"" + graph_title + "\""};
    runPythonScript(scriptName, args);
}

static void drawMatrixHelper(std::vector<std::vector<double>> const& matrix,
                             std::string const& matrixFile, std::string const& outputFile,
                             std::string const& title, std::string const& defaultTitle) {
    std::ofstream out(matrixFile);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << matrixFile << "\n";
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
    out.close();

    std::string matrix_title = title.empty() ? defaultTitle : title;
    std::vector<std::string> args = {matrixFile, outputFile, "\"" + matrix_title + "\""};
    runPythonScript("plot_matrix.py", args);
}

// ============================================================================

void Visualizer::drawGraph(Graph const& graph, std::string const& outputFile,
                           std::string const& title) {
    exportEdges(graph, "assets/txt/graph.txt");
    std::string graph_type = graph.isDirected() ? "directed" : "undirected";

    drawGraphHelper("plot_graph.py", "assets/txt/graph.txt", outputFile, graph_type, title,
                    "Граф (" + graph_type + ")");
}

void Visualizer::drawGraphWithPath(Graph const& graph, std::vector<int> const& path,
                                   std::string const& outputFile, std::string const& title) {
    std::vector<std::vector<int>> paths = {path};
    drawGraphWithPaths(graph, paths, outputFile, title);
}

void Visualizer::drawGraphWithPath(Graph const& graph, std::vector<int> const& path,
                                   std::vector<std::pair<int, int>> const& addedEdges,
                                   std::string const& outputFile, std::string const& title) {
    exportAddedEdges(addedEdges, "assets/txt/added_edges.txt");
    std::vector<std::vector<int>> paths = {path};
    drawGraphWithPaths(graph, paths, outputFile, title);
}

void Visualizer::drawGraphWithPaths(Graph const& graph, std::vector<std::vector<int>> const& paths,
                                    std::string const& outputFile, std::string const& title) {
    exportEdges(graph, "assets/txt/graph.txt");
    exportPaths(paths, "assets/txt/paths.txt");
    std::string graph_type = graph.isDirected() ? "directed" : "undirected";
    std::string graph_title = title.empty() ? (paths.size() == 1 ? "Граф с путём (" + graph_type + ")"
                                                                : "Все пути (" + graph_type + ")")
                                           : title;

    std::vector<std::string> args = {"assets/txt/graph.txt", "assets/txt/paths.txt", outputFile,
                                     graph_type, "\"" + graph_title + "\""};
    runPythonScript("plot_graph_paths.py", args);
}

// ============================================================================

void Visualizer::drawAdjacencyMatrix(Graph const& graph, std::string const& outputFile,
                                     std::string const& title) {
    auto vertices = graph.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(
        vertices, vertices, [&](int i, int j) { return graph.hasEdge(i, j) ? 1.0 : 0.0; });

    drawMatrixHelper(matrix, "assets/txt/matrix.txt", outputFile, title, "Матрица смежности");
}

void Visualizer::drawWeightMatrix(Graph const& graph, std::string const& outputFile,
                                  std::string const& title) {
    auto vertices = graph.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices, [&](int i, int j) {
        auto w = graph.getEdgeWeight(i, j);
        return w ? *w : 0.0;
    });

    drawMatrixHelper(matrix, "assets/txt/matrix.txt", outputFile, title, "Матрица весов");
}

void Visualizer::drawShimbellMatrix(DistanceMatrix const& shimMatrix, std::string const& outputFile,
                                    std::string const& title) {
    size_t n = shimMatrix.size();
    std::vector<int> indices(static_cast<size_t>(n));
    for (size_t i = 0; i < n; ++i)
        indices[i] = static_cast<int>(i);

    auto matrix = CollectionUtils::makeMatrix<double>(indices, indices, [&](int i, int j) {
        return shimMatrix[i][j].has_value() ? shimMatrix[i][j].value() : -1e9;
    });

    drawMatrixHelper(matrix, "assets/txt/shimbell.txt", outputFile, title, "Матрица Шимбелла");
}

// ============================================================================

void Visualizer::drawFlowNetwork(FlowNetwork const& network, std::string const& outputFile,
                                 std::string const& title) {
    exportFlow(network, "assets/txt/flow.txt");
    std::string graph_type = network.isDirected() ? "directed" : "undirected";

    drawGraphHelper("plot_flow.py", "assets/txt/flow.txt", outputFile, graph_type, title,
                    "Сеть потоков: толщина = поток, цвет = загрузка (" + graph_type + ")");
}

void Visualizer::drawFlowNetworkWithPath(FlowNetwork const& network, std::vector<int> const& path,
                                         std::string const& outputFile, std::string const& title) {
    exportFlow(network, "assets/txt/flow.txt");
    exportPath(path, "assets/txt/path.txt");
    std::string graph_type = network.isDirected() ? "directed" : "undirected";

    std::string graph_title = title.empty() ? "Сеть потоков с путём (" + graph_type + ")" : title;
    std::vector<std::string> args = {"assets/txt/flow.txt", "assets/txt/path.txt", outputFile,
                                     graph_type, "\"" + graph_title + "\""};
    runPythonScript("plot_flow_path.py", args);
}

void Visualizer::drawCapacityMatrix(FlowNetwork const& network, std::string const& outputFile,
                                    std::string const& title) {
    auto vertices = network.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(
        vertices, vertices, [&](int i, int j) { return network.getCapacity(i, j); });

    drawMatrixHelper(matrix, "assets/txt/matrix.txt", outputFile, title,
                     "Матрица пропускных способностей");
}

void Visualizer::drawCostMatrix(FlowNetwork const& network, std::string const& outputFile,
                                std::string const& title) {
    auto vertices = network.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(
        vertices, vertices, [&](int i, int j) { return network.getCost(i, j); });

    drawMatrixHelper(matrix, "assets/txt/matrix.txt", outputFile, title, "Матрица стоимостей");
}

}  // namespace graph
