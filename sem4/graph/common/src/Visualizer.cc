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

void Visualizer::drawMatrixHelper(std::vector<std::vector<double>> const& matrix, DrawData const& data,
                             std::string const& defaultTitle) {
    exportMatrix(matrix, data);

    std::string matrix_title = data.title.empty() ? defaultTitle : data.title;
    std::vector<std::string> args = {data.txtFile, data.pngFile, "\"" + matrix_title + "\""};
    runPythonScript("plot_matrix.py", args);
}

// ============================================================================

void Visualizer::drawGraph(Graph const& graph, DrawData const& data) {
    exportEdges(graph, data);
    std::string graph_type = graph.isDirected() ? "directed" : "undirected";
    std::string graph_title = data.title.empty() ? "Граф (" + graph_type + ")" : data.title;

    drawGraphHelper("plot_graph.py", data.txtFile, data.pngFile, graph_type, graph_title,
                    "Граф (" + graph_type + ")");
}

void Visualizer::drawGraphWithPath(Graph const& graph, DrawData const& data) {
    if (!data.addedEdges.empty()) {
        exportAddedEdges(data);
    }
    DrawData pathsData = data;
    pathsData.paths = {data.path};
    drawGraphWithPaths(graph, pathsData);
}

void Visualizer::drawGraphWithPaths(Graph const& graph, DrawData const& data) {
    exportEdges(graph, data);
    if (!data.addedEdges.empty()) {
        exportAddedEdges(data);
    }
    if (!data.txtPathsFile.empty()) {
        exportPaths(data);
    }
    std::string graph_type = graph.isDirected() ? "directed" : "undirected";
    std::string graph_title = data.title.empty() ? (data.paths.size() == 1 ? "Граф с путём (" + graph_type + ")"
                                                                : "Все пути (" + graph_type + ")")
                                           : data.title;

    std::vector<std::string> args = {data.txtFile, data.txtPathsFile, data.pngFile,
                                     graph_type, "\"" + graph_title + "\"", data.txtGraphFile};
    runPythonScript("plot_graph_paths.py", args);
}

// ============================================================================

void Visualizer::drawAdjacencyMatrix(Graph const& graph, DrawData const& data) {
    auto vertices = graph.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(
        vertices, vertices, [&](int i, int j) { return graph.hasEdge(i, j) ? 1.0 : 0.0; });

    Visualizer::drawMatrixHelper(matrix, data, "Матрица смежности");
}

void Visualizer::drawWeightMatrix(Graph const& graph, DrawData const& data) {
    auto vertices = graph.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices, [&](int i, int j) {
        auto w = graph.getEdgeWeight(i, j);
        return w ? *w : 0.0;
    });

    Visualizer::drawMatrixHelper(matrix, data, "Матрица весов");
}

void Visualizer::drawShimbellMatrix(DistanceMatrix const& shimMatrix, DrawData const& data) {
    size_t n = shimMatrix.size();
    std::vector<int> indices(static_cast<size_t>(n));
    for (size_t i = 0; i < n; ++i)
        indices[i] = static_cast<int>(i);

    auto matrix = CollectionUtils::makeMatrix<double>(indices, indices, [&](int i, int j) {
        return shimMatrix[i][j].has_value() ? shimMatrix[i][j].value() : -1e9;
    });

    Visualizer::drawMatrixHelper(matrix, data, "Матрица Шимбелла");
}

// ============================================================================

void Visualizer::drawFlowNetwork(FlowNetwork const& network, DrawData const& data) {
    exportFlow(network, data);
    std::string graph_type = network.isDirected() ? "directed" : "undirected";
    std::string graph_title = data.title.empty() ? "Сеть потоков: толщина = поток, цвет = загрузка (" + graph_type + ")" : data.title;

    drawGraphHelper("plot_flow.py", data.txtFile, data.pngFile, graph_type, graph_title,
                    "Сеть потоков: толщина = поток, цвет = загрузка (" + graph_type + ")");
}

void Visualizer::drawFlowNetworkWithPath(FlowNetwork const& network, DrawData const& data) {
    exportFlow(network, data);
    exportPath(data);
    std::string graph_type = network.isDirected() ? "directed" : "undirected";

    std::string graph_title = data.title.empty() ? "Сеть потоков с путём (" + graph_type + ")" : data.title;
    std::vector<std::string> args = {data.txtFile, data.txtPathsFile, data.pngFile,
                                     graph_type, "\"" + graph_title + "\""};
    runPythonScript("plot_flow_path.py", args);
}

void Visualizer::drawCapacityMatrix(FlowNetwork const& network, DrawData const& data) {
    auto vertices = network.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(
        vertices, vertices, [&](int i, int j) { return network.getCapacity(i, j); });

    Visualizer::drawMatrixHelper(matrix, data, "Матрица пропускных способностей");
}

void Visualizer::drawCostMatrix(FlowNetwork const& network, DrawData const& data) {
    auto vertices = network.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(
        vertices, vertices, [&](int i, int j) { return network.getCost(i, j); });

    Visualizer::drawMatrixHelper(matrix, data, "Матрица стоимостей");
}

void Visualizer::drawColoredGraph(Graph const& graph, DrawData const& data) {
    exportEdges(graph, data);
    exportColors(data, graph.vertexIds());

    std::string graph_type = graph.isDirected() ? "directed" : "undirected";
    std::string graph_title = data.title.empty() ? "Раскраска графа" : data.title;

    std::vector<std::string> args = {data.txtFile, data.pngFile, data.txtColorsFile,
                                     graph_type, "\"" + graph_title + "\""};
    runPythonScript("plot_colored_graph.py", args);
}



}  // namespace graph
