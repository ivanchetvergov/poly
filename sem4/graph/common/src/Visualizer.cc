#include "Visualizer.h"

#include "CollectionUtils.h"
#include "Graph.h"
#include "Utils.h"

#include "../../lab3/include/FlowNetwork.h"

#include <iostream>
#include <numeric>

using Args = std::vector<std::string>;

namespace graph {

void Visualizer::drawMatrix(DrawData const& data, std::string const& defaultTitle) {
    std::string matrix_title = data.title.empty() ? defaultTitle : data.title;
    std::vector<std::string> args = {data.txtFile, data.pngFile, "\"" + matrix_title + "\""};
    runPythonScript("plot_matrix.py", args);
}

void Visualizer::draw(DrawData const& data, bool directed, VisualizationType type) {
    std::string graphType = directed ? "directed" : "undirected";
    std::string type_str;
    std::string default_title;
    bool is_network = false;

    switch (type) {
        case VisualizationType::Graph:
            type_str = "graph";
            default_title = "Граф (" + graphType + ")";
            is_network = true;
            break;
        case VisualizationType::FlowNetwork:
            type_str = "flow";
            default_title = "Сеть потоков(" + graphType + ")";
            is_network = true;
            break;
        case VisualizationType::Animation: {
            std::vector<std::string> args = {"--input", "assets/txt/32_flow_snapshots.txt", "--output",
                                             data.gifFile, "--graph_type", graphType, "--show"};
            runPythonScript("plot_flow_animation.py", args);
            break;
        }
        default:
            return;
    }

    if (is_network) {
        std::string graph_title = data.title.empty() ? default_title : data.title;
        Args args = {
            data.txtFile,
            data.pngFile,
            graphType,
            "\"" + graph_title + "\"",
            "--type", type_str
        };
        if (!data.txtGraphFile.empty()) {
            args.push_back("--added_edges_file");
            args.push_back(data.txtGraphFile);
        }
        runPythonScript("plot_network.py", args);
    }
}

void Visualizer::drawPaths(DrawData const& data, bool directed, VisualizationType type) {
    std::string graphType = directed ? "directed" : "undirected";
    std::string type_str;
    std::string default_title;

    switch (type) {
        case VisualizationType::Graph:
            type_str = "graph";
            default_title = "Все пути (" + graphType + ")";
            break;
        case VisualizationType::FlowNetwork:
            type_str = "flow";
            default_title = "Сеть потоков с путём (" + graphType + ")";
            break;
        default:
            return;
    }

    std::string graph_title = data.title.empty() ? default_title : data.title;
    Args args = {
        data.txtFile,
        data.txtPathsFile,
        data.pngFile,
        graphType,
        "\"" + graph_title + "\"",
        "--type", type_str
    };
    if (!data.txtGraphFile.empty()) {
        args.push_back("--added_edges_file");
        args.push_back(data.txtGraphFile);
    }
    runPythonScript("plot_paths.py", args);
}

void Visualizer::drawColoredGraph(DrawData const& data, bool directed) {
    std::string graphType = directed ? "directed" : "undirected";
    std::string graph_title = data.title.empty() ? "Раскраска графа" : data.title;

    Args args = {data.txtFile,
                data.pngFile,
                data.txtColorsFile,
                graphType,
                "\"" + graph_title + "\""
    };
    runPythonScript("plot_colored_graph.py", args);
}

void Visualizer::drawHashTable(DrawData const& data) {
    Args args = {data.txtFile, data.pngFile, "\"" + data.title + "\""};
    runPythonScript("plot_hashtable.py", args);
}

void Visualizer::drawRBTree(DrawData const& data, VisualizationType type) {
    if (type == VisualizationType::Animation) {
        Args args = {data.txtFile, data.gifFile, "\"" + data.title + "\""};
        runPythonScript("plot_rbtree_animation.py", args);
    } else {
        Args args = {data.txtFile, data.pngFile, "\"" + data.title + "\""};
        runPythonScript("plot_rbtree.py", args);
    }
}

}  // namespace graph
