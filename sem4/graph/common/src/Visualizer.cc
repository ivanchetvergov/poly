#include "Visualizer.h"
#include "Graph.h"
#include "../../lab3/include/FlowNetwork.h"
#include "CollectionUtils.h"
#include "Utils.h"
#include <fstream>
#include <iostream>

namespace graph {

static void drawGraphHelper(const std::string& scriptName,
                             const std::string& dataFile,
                             const std::string& outputFile,
                             const std::string& graphType,
                             const std::string& title,
                             const std::string& defaultTitle)
{
    std::string graphTitle = title.empty() ? defaultTitle : title;
    std::vector<std::string> args = {dataFile, outputFile, graphType, "\"" + graphTitle + "\""};
    runPythonScript(scriptName, args);
}

static void drawMatrixHelper(const std::vector<std::vector<double>>& matrix,
                             const std::string& matrixFile,
                             const std::string& outputFile,
                             const std::string& title,
                             const std::string& defaultTitle)
{
    std::ofstream out(matrixFile);
    if (!out) {
        std::cerr << "[FAIL] Не удалось открыть файл для записи: " << matrixFile << "\n";
        return;
    }
    for (const auto& row : matrix) {
        for (size_t j = 0; j < row.size(); ++j) {
            out << row[j];
            if (j + 1 < row.size()) out << " ";
        }
        out << "\n";
    }
    out.close();

    std::string matrixTitle = title.empty() ? defaultTitle : title;
    std::vector<std::string> args = {matrixFile, outputFile, "\"" + matrixTitle + "\""};
    runPythonScript("plot_matrix.py", args);
}

void Visualizer::drawGraph(const Graph& graph,
    const std::string& outputFile, const std::string& title)
{
    exportEdges(graph, "assets/txt/graph.txt");
    std::string graphType = graph.isDirected() ? "directed" : "undirected";

    drawGraphHelper("plot_graph.py", "assets/txt/graph.txt", outputFile, graphType, title,
                    "Граф (" + graphType + ")");
}

void Visualizer::drawGraphWithPath(const Graph& graph,
        const std::vector<int>& path, const std::string& outputFile, const std::string& title)
{
    exportEdges(graph, "assets/txt/graph.txt");
    exportPath(path, "assets/txt/path.txt");
    std::string graphType = graph.isDirected() ? "directed" : "undirected";

    drawGraphHelper("plot_graph_path.py", "assets/txt/graph.txt", outputFile, graphType, title,
                    "Граф с путём (" + graphType + ")");
}

void Visualizer::drawGraphWithPath(const Graph& graph,
        const std::vector<int>& path,
        const std::vector<std::pair<int, int>>& addedEdges,
        const std::string& outputFile, const std::string& title)
{
    exportEdges(graph, "assets/txt/graph.txt");
    exportPath(path, "assets/txt/path.txt");
    exportAddedEdges(addedEdges, "assets/txt/added_edges.txt");
    std::string graphType = graph.isDirected() ? "directed" : "undirected";

    std::string graphTitle = title.empty() ? "Граф с путём (" + graphType + ")" : title;
    std::vector<std::string> args = {"assets/txt/graph.txt", outputFile, graphType, "\"" + graphTitle + "\"", "assets/txt/added_edges.txt"};
    runPythonScript("plot_graph_path.py", args);
}

void Visualizer::drawFlowNetwork(const FlowNetwork& network,
    const std::string& outputFile, const std::string& title)
{
    exportFlow(network, "assets/txt/flow.txt");
    std::string graphType = network.isDirected() ? "directed" : "undirected";

    drawGraphHelper("plot_flow.py", "assets/txt/flow.txt", outputFile, graphType, title,
                    "Сеть потоков: толщина = поток, цвет = загрузка (" + graphType + ")");
}

void Visualizer::drawCapacityMatrix(const FlowNetwork& network,
        const std::string& outputFile, const std::string& title)
{
    auto vertices = network.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices,
        [&](int i, int j){ return network.getCapacity(i, j); });

    drawMatrixHelper(matrix, "assets/txt/matrix.txt", outputFile, title,
                     "Матрица пропускных способностей");
}

void Visualizer::drawCostMatrix(const FlowNetwork& network,
        const std::string& outputFile, const std::string& title)
{
    auto vertices = network.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices,
        [&](int i, int j){ return network.getCost(i, j); });

    drawMatrixHelper(matrix, "assets/txt/matrix.txt", outputFile, title,
                     "Матрица стоимостей");
}

void Visualizer::drawAdjacencyMatrix(const Graph& graph,
        const std::string& outputFile, const std::string& title)
{
    auto vertices = graph.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices,
        [&](int i, int j)
        { return graph.hasEdge(i, j) ? 1.0 : 0.0; });

    drawMatrixHelper(matrix, "assets/txt/matrix.txt", outputFile, title,
                     "Матрица смежности");
}

using DistanceMatrix = std::vector<std::vector<std::optional<double>>>;

void Visualizer::drawShimbellMatrix(const DistanceMatrix& shimMatrix,
        const std::string& outputFile, const std::string& title)
{
    size_t n = shimMatrix.size();
    std::vector<int> indices(static_cast<size_t>(n));
    for (size_t i = 0; i < n; ++i) indices[i] = static_cast<int>(i);

    auto matrix = CollectionUtils::makeMatrix<double>(indices, indices,
        [&](int i, int j) {
            return shimMatrix[i][j].has_value() ? shimMatrix[i][j].value() : -1e9;
        });

    drawMatrixHelper(matrix, "assets/txt/shimbell.txt", outputFile, title,
                     "Матрица Шимбелла");
}

void Visualizer::drawWeightMatrix(const Graph& graph,
    const std::string& outputFile, const std::string& title)
{
    auto vertices = graph.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices,
        [&](int i, int j){
        auto w = graph.getEdgeWeight(i, j);
        return w ? *w : 0.0;
    });

    drawMatrixHelper(matrix, "assets/txt/matrix.txt", outputFile, title,
                     "Матрица весов");
}

void Visualizer::drawFlowNetworkWithPath(const FlowNetwork& network, const std::vector<int>& path,
    const std::string& outputFile, const std::string& title)
{
    exportFlow(network, "assets/txt/flow.txt");
    exportPath(path, "assets/txt/path.txt");
    std::string graphType = network.isDirected() ? "directed" : "undirected";

    std::string graphTitle = title.empty() ? "Сеть потоков с путём (" + graphType + ")" : title;
    std::vector<std::string> args = {"assets/txt/flow.txt", outputFile, graphType, "\"" + graphTitle + "\""};
    runPythonScript("plot_flow_path.py", args);
}


} // namespace graph
