#pragma once

#include <string>
#include <vector>

namespace graph {

class Graph;
class FlowNetwork;

class Visualizer {
    using DistanceMatrix = std::vector<std::vector<std::optional<double>>>;

public:
    static void drawGraph(Graph const& graph,
                          std::string const& outputFile = "assets/png/graph.png",
                          std::string const& title = "");

    static void drawGraphWithPath(Graph const& graph, std::vector<int> const& path,
                                  std::string const& outputFile = "assets/png/path.png",
                                  std::string const& title = "");

    static void drawGraphWithPath(Graph const& graph, std::vector<int> const& path,
                                  std::vector<std::pair<int, int>> const& addedEdges,
                                  std::string const& outputFile = "assets/png/path.png",
                                  std::string const& title = "");

    static void drawGraphWithPaths(Graph const& graph, std::vector<std::vector<int>> const& paths,
                                   std::string const& outputFile = "assets/png/paths.png",
                                   std::string const& title = "");

    // ========================================================================

    static void drawAdjacencyMatrix(Graph const& graph,
                                    std::string const& outputFile = "assets/png/adjacency.png",
                                    std::string const& title = "");

    static void drawWeightMatrix(Graph const& graph,
                                 std::string const& outputFile = "assets/png/weights.png",
                                 std::string const& title = "");

    static void drawShimbellMatrix(DistanceMatrix const& shimMatrix,
                                   std::string const& outputFile = "assets/png/shimbell.png",
                                   std::string const& title = "");

    // ========================================================================

    static void drawFlowNetwork(FlowNetwork const& network,
                                std::string const& outputFile = "assets/png/flow.png",
                                std::string const& title = "");

    static void drawFlowNetworkWithPath(FlowNetwork const& network, std::vector<int> const& path,
                                        std::string const& outputFile = "assets/png/flow_path.png",
                                        std::string const& title = "");

    static void drawCapacityMatrix(FlowNetwork const& network,
                                   std::string const& outputFile = "assets/png/capacity_matrix.png",
                                   std::string const& title = "");

    static void drawCostMatrix(FlowNetwork const& network,
                               std::string const& outputFile = "assets/png/cost_matrix.png",
                               std::string const& title = "");

    static void drawColoredGraph(Graph const& graph, std::vector<int> const& colors,
                                 std::string const& outputFile = "assets/png/colored_graph.png",
                                 std::string const& title = "");

private:
    static void exportColors(std::vector<int> const& colors, std::string const& filename);
    static void exportEdges(Graph const& graph, std::string const& filename);
    static void exportPath(std::vector<int> const& path, std::string const& filename);
    static void exportPaths(std::vector<std::vector<int>> const& paths,
                            std::string const& filename);
    static void exportAddedEdges(std::vector<std::pair<int, int>> const& addedEdges,
                                 std::string const& filename);
    static void exportMatrix(std::vector<std::vector<double>> const& matrix,
                             std::string const& filename);
    static void exportFlow(FlowNetwork const& network, std::string const& filename);
};

}  // namespace graph
