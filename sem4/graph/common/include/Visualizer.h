#pragma once

#include <optional>
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

enum class VisualizationType {
    Graph,
    FlowNetwork,
    Animation
};

class Visualizer {
public:
    static void visualize(DrawData const& data, bool directed, VisualizationType type);
    static void drawPaths(DrawData const& data, bool directed, VisualizationType type);
    static void drawColoredGraph(DrawData const& data, bool directed);

    static void drawMatrix(DrawData const& data, std::string const& defaultTitle);

};

}  // namespace graph
