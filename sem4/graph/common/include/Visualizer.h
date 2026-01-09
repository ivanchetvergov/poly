#pragma once

#include <string>
#include <vector>

namespace graph {

class Graph;
class FlowNetwork;

struct DrawData {
    std::string pngFile;
    std::string txtFile;
    std::string txtColorsFile;
    std::string txtPathsFile;
    std::string txtGraphFile;
    std::string gifFile;
    std::string title;
    std::vector<int> colors;
    std::vector<int> path;
    std::vector<std::pair<int, int>> addedEdges;
    std::vector<std::vector<int>> paths;

};

class Visualizer {
    using DistanceMatrix = std::vector<std::vector<std::optional<double>>>;

public:
    static void drawGraph(Graph const& graph, DrawData const& data);

    static void drawGraphWithPath(Graph const& graph, DrawData const& data);

    static void drawGraphWithPaths(Graph const& graph, DrawData const& data);

    // ========================================================================

    static void drawAdjacencyMatrix(Graph const& graph, DrawData const& data);

    static void drawWeightMatrix(Graph const& graph, DrawData const& data);

    static void drawShimbellMatrix(DistanceMatrix const& shimMatrix, DrawData const& data);

    // ========================================================================

    static void drawFlowNetwork(FlowNetwork const& network, DrawData const& data);

    static void drawFlowNetworkWithPath(FlowNetwork const& network, DrawData const& data);

    static void drawCapacityMatrix(FlowNetwork const& network, DrawData const& data);

    static void drawCostMatrix(FlowNetwork const& network, DrawData const& data);

    static void drawColoredGraph(Graph const& graph, DrawData const& data);

private:
    static void exportColors(DrawData const& data, std::vector<int> const& vertices);
    static void exportEdges(Graph const& graph, DrawData const& data);
    static void exportPath(DrawData const& data);
    static void exportPaths(DrawData const& data);
    static void exportAddedEdges(DrawData const& data);
    static void exportMatrix(std::vector<std::vector<double>> const& matrix, DrawData const& data);
    static void exportFlow(FlowNetwork const& network, DrawData const& data);

    static void drawMatrixHelper(std::vector<std::vector<double>> const& matrix, DrawData const& data,
                                 std::string const& defaultTitle);
};

}  // namespace graph
