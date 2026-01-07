#pragma once

#include <string>
#include <vector>

namespace graph {

class Graph;
class FlowNetwork;

class Visualizer {
public:
    static void drawGraph(const Graph& graph,
        const std::string& outputFile = "assets/png/graph.png");

    static void drawGraphWithPath(const Graph& graph, const std::vector<int>& path,
        const std::string& outputFile = "assets/png/path.png", const std::string& title = "Путь");

    static void drawFlowNetwork(const FlowNetwork& network,
        const std::string& outputFile = "assets/png/flow.png");

    static void drawCapacityMatrix(const FlowNetwork& network,
        const std::string& outputFile = "assets/png/capacity_matrix.png");

    static void drawCostMatrix(const FlowNetwork& network,
        const std::string& outputFile = "assets/png/cost_matrix.png");

    static void drawAdjacencyMatrix(const Graph& graph,
        const std::string& outputFile = "assets/png/adjacency.png");

    static void drawWeightMatrix(const Graph& graph,
        const std::string& outputFile = "assets/png/weights.png");

private:
    static void exportEdges(const Graph& graph, const std::string& filename);
    static void exportPath(const std::vector<int>& path, const std::string& filename);
    static void exportMatrix(const std::vector<std::vector<double>>& matrix, const std::string& filename);
    static void exportFlow(const FlowNetwork& network, const std::string& filename);
    static int runPythonScript(const std::string& scriptName, const std::string& inputFile, const std::string& outputFile, const std::string& extraArg = "");
};

} // namespace graph
