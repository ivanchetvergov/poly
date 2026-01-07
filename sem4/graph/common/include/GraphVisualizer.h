#pragma once

#include <string>
#include <vector>

namespace graph {

class Graph;
class FlowNetwork;

class GraphVisualizer {
public:
    static void drawGraph(const Graph& graph, const std::string& outputFile = "assets/graph.png");
    static void drawAdjacencyMatrix(const Graph& graph, const std::string& outputFile = "assets/adjacency.png");
    static void drawWeightMatrix(const Graph& graph, const std::string& outputFile = "assets/weights.png");
    static void drawFlowNetwork(const FlowNetwork& network, const std::string& outputFile = "assets/flow.png");

private:
    static void exportEdges(const Graph& graph, const std::string& filename);
    static void exportMatrix(const std::vector<std::vector<double>>& matrix, const std::string& filename);
    static void exportFlow(const FlowNetwork& network, const std::string& filename);
    static int runPythonScript(const std::string& scriptName, const std::string& inputFile, const std::string& outputFile);
};

} // namespace graph
