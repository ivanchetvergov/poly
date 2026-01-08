#include "Visualizer.h"
#include "Graph.h"
#include "../../lab3/include/FlowNetwork.h"
#include "CollectionUtils.h"
#include <fstream>
#include <iostream>

namespace graph {

static void drawGraphHelper(const std::string& scriptName,
                             const std::string& dataFile,
                             const std::string& outputFile,
                             const std::string& graphType,
                             const std::string& title,
                             const std::string& defaultTitle,
                             const std::string& successMsg)
{
    std::string graphTitle = title.empty() ? defaultTitle : title;
    std::string args = graphType + " \"" + graphTitle + "\"";

    std::string cmd = "./venv/bin/python scripts/" + scriptName + " " + dataFile + " " + outputFile + " " + args;
    int ret = system(cmd.c_str());
    if (ret == 0) {
        std::cout << successMsg << outputFile << "\n";
    } else {
        std::cerr << "[FAIL] Не удалось запустить Python-скрипт: " << scriptName << "\n";
    }
}

static void drawMatrixHelper(const std::vector<std::vector<double>>& matrix,
                             const std::string& matrixFile,
                             const std::string& outputFile,
                             const std::string& title,
                             const std::string& defaultTitle,
                             const std::string& successMsg)
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
    std::string args = "\"" + matrixTitle + "\"";

    std::string cmd = "./venv/bin/python scripts/plot_matrix.py " + matrixFile + " " + outputFile + " " + args;
    int ret = system(cmd.c_str());
    if (ret == 0) {
        std::cout << successMsg << outputFile << "\n";
    } else {
        std::cerr << "[FAIL] Не удалось запустить Python-скрипт\n";
    }
}

void Visualizer::drawGraph(const Graph& graph,
    const std::string& outputFile, const std::string& title)
{
    exportEdges(graph, "assets/txt/graph.txt");
    std::string graphType = graph.isDirected() ? "directed" : "undirected";

    drawGraphHelper("plot_graph.py", "assets/txt/graph.txt", outputFile, graphType, title,
                    "Граф (" + graphType + ")", "[OK] Граф сохранён в ");
}

void Visualizer::drawGraphWithPath(const Graph& graph,
        const std::vector<int>& path, const std::string& outputFile, const std::string& title)
{
    exportEdges(graph, "assets/txt/graph.txt");
    exportPath(path, "assets/txt/path.txt");
    std::string graphType = graph.isDirected() ? "directed" : "undirected";

    drawGraphHelper("plot_graph_path.py", "assets/txt/graph.txt", outputFile, graphType, title,
                    "Граф с путём (" + graphType + ")", "[OK] Граф с путём сохранён в ");
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
    std::string args = graphType + " \"" + graphTitle + "\" assets/txt/added_edges.txt";

    std::string cmd = "./venv/bin/python scripts/plot_graph_path.py assets/txt/graph.txt " + outputFile + " " + args;
    int ret = system(cmd.c_str());
    if (ret == 0) {
        std::cout << "[OK] Граф с путём сохранён в " << outputFile << "\n";
    } else {
        std::cerr << "[FAIL] Не удалось запустить Python-скрипт: plot_graph_path.py\n";
    }
}

void Visualizer::drawFlowNetwork(const FlowNetwork& network,
    const std::string& outputFile, const std::string& title)
{
    exportFlow(network, "assets/txt/flow.txt");
    std::string graphType = network.isDirected() ? "directed" : "undirected";

    drawGraphHelper("plot_flow.py", "assets/txt/flow.txt", outputFile, graphType, title,
                    "Сеть потоков: толщина = поток, цвет = загрузка (" + graphType + ")",
                    "[OK] Визуализация потока сохранена в ");
}

void Visualizer::drawCapacityMatrix(const FlowNetwork& network,
        const std::string& outputFile, const std::string& title)
{
    auto vertices = network.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices,
        [&](int i, int j){ return network.getCapacity(i, j); });

    drawMatrixHelper(matrix, "assets/txt/matrix.txt", outputFile, title,
                     "Матрица пропускных способностей",
                     "[OK] Матрица пропускных способностей сохранена в ");
}

void Visualizer::drawCostMatrix(const FlowNetwork& network,
        const std::string& outputFile, const std::string& title)
{
    auto vertices = network.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices,
        [&](int i, int j){ return network.getCost(i, j); });

    drawMatrixHelper(matrix, "assets/txt/matrix.txt", outputFile, title,
                     "Матрица стоимостей",
                     "[OK] Матрица стоимостей сохранена в ");
}

void Visualizer::drawAdjacencyMatrix(const Graph& graph,
        const std::string& outputFile, const std::string& title)
{
    auto vertices = graph.vertexIds();
    auto matrix = CollectionUtils::makeMatrix<double>(vertices, vertices,
        [&](int i, int j)
        { return graph.hasEdge(i, j) ? 1.0 : 0.0; });

    drawMatrixHelper(matrix, "assets/txt/matrix.txt", outputFile, title,
                     "Матрица смежности",
                     "[OK] Матрица смежности сохранена в ");
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
                     "Матрица Шимбелла",
                     "[OK] Матрица Шимбелла сохранена в ");
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
                     "Матрица весов",
                     "[OK] Матрица весов сохранена в ");
}

void Visualizer::drawFlowNetworkWithPath(const FlowNetwork& network, const std::vector<int>& path,
    const std::string& outputFile, const std::string& title)
{
    exportFlow(network, "assets/txt/flow.txt");
    exportPath(path, "assets/txt/path.txt");
    std::string graphType = network.isDirected() ? "directed" : "undirected";

    std::string graphTitle = title.empty() ? "Сеть потоков с путём (" + graphType + ")" : title;
    std::string args = graphType + " \"" + graphTitle + "\"";

    std::string cmd = "./venv/bin/python scripts/plot_flow_path.py assets/txt/flow.txt " + outputFile + " " + args;
    int ret = system(cmd.c_str());
    if (ret == 0) {
        std::cout << "[OK] Сеть потоков с путём сохранена в " << outputFile << "\n";
    } else {
        std::cerr << "[FAIL] Не удалось запустить Python-скрипт: plot_flow_path.py\n";
    }
}

} // namespace graph
