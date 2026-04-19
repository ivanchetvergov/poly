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
    std::vector<std::string> colorLabels{};
};


enum class VisualizationType {
    Graph,
    FlowNetwork,
    Animation
};

class Visualizer {
public:
    static void draw(DrawData const& data, bool directed, VisualizationType type);
    static void drawPaths(DrawData const& data, bool directed, VisualizationType type);
    static void drawBFSAnimation(DrawData const& data, bool directed);
    static void drawCompare(DrawData const& data);
    static void drawColoredGraph(DrawData const& data, bool directed);
    static void drawMatrix(DrawData const& data, std::string const& defaultTitle,
                           bool hideOffDiagonalZeros = false);

    static void drawHashTable(DrawData const& data);
    static void drawRBTree(DrawData const& data, VisualizationType type);
};

}  // namespace graph
