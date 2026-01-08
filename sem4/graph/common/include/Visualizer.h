#pragma once

#include <string>
#include <vector>

namespace graph {

class Graph;
class FlowNetwork;

class Visualizer {

using DistanceMatrix = std::vector<std::vector<std::optional<double>>>;

public:
    static void drawGraph(const Graph& graph,
        const std::string& outputFile = "assets/png/graph.png",
        const std::string& title = "");

    static void drawGraphWithPath(const Graph& graph, const std::vector<int>& path,
        const std::string& outputFile = "assets/png/path.png",
        const std::string& title = "");

    static void drawGraphWithPath(const Graph& graph, const std::vector<int>& path,
        const std::vector<std::pair<int, int>>& addedEdges,
        const std::string& outputFile = "assets/png/path.png",
        const std::string& title = "");

    static void drawGraphWithPaths(const Graph& graph, const std::vector<std::vector<int>>& paths,
        const std::string& outputFile = "assets/png/paths.png",
        const std::string& title = "");

    // ========================================================================

    static void drawAdjacencyMatrix(const Graph& graph,
        const std::string& outputFile = "assets/png/adjacency.png",
        const std::string& title = "");

    static void drawWeightMatrix(const Graph& graph,
        const std::string& outputFile = "assets/png/weights.png",
        const std::string& title = "");

    static void drawShimbellMatrix(const DistanceMatrix& shimMatrix,
        const std::string& outputFile = "assets/png/shimbell.png",
        const std::string& title = "");

    // ========================================================================

    static void drawFlowNetwork(const FlowNetwork& network,
        const std::string& outputFile = "assets/png/flow.png",
        const std::string& title = "");

    static void drawFlowNetworkWithPath(const FlowNetwork& network, const std::vector<int>& path,
        const std::string& outputFile = "assets/png/flow_path.png",
        const std::string& title = "");

    static void drawCapacityMatrix(const FlowNetwork& network,
        const std::string& outputFile = "assets/png/capacity_matrix.png",
        const std::string& title = "");

    static void drawCostMatrix(const FlowNetwork& network,
        const std::string& outputFile = "assets/png/cost_matrix.png",
        const std::string& title = "");

private:
    static void exportEdges(const Graph& graph, const std::string& filename);
    static void exportPath(const std::vector<int>& path, const std::string& filename);
    static void exportPaths(const std::vector<std::vector<int>>& paths, const std::string& filename);
    static void exportAddedEdges(const std::vector<std::pair<int, int>>& addedEdges, const std::string& filename);
    static void exportMatrix(const std::vector<std::vector<double>>& matrix, const std::string& filename);
    static void exportFlow(const FlowNetwork& network, const std::string& filename);
};

} // namespace graph
