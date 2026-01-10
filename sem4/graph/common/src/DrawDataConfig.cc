#include "DrawDataConfig.h"

namespace graph {

std::map<int, DrawData> DrawDataConfig::configs_ = {
    {1, {
        .pngFile = "assets/png/01_graph.png",
        .txtFile = "assets/txt/01_graph.txt",
        .txtColorsFile = "",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Сгенерированный граф",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {2, {
        .pngFile = "assets/png/02_adjacency_matrix.png",
        .txtFile = "assets/txt/02_adjacency_matrix.txt",
        .txtColorsFile = "",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Матрица смежности",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {3, {
        .pngFile = "assets/png/03_weight_matrix.png",
        .txtFile = "assets/txt/03_weight_matrix.txt",
        .txtColorsFile = "",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Матрица весов",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {12, {
        .pngFile = "assets/png/12_paths.png",
        .txtFile = "assets/txt/12_graph.txt",
        .txtColorsFile = "",
        .txtPathsFile = "assets/txt/12_paths.txt",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Найденные пути",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {13, {
        .pngFile = "assets/png/13_shimbell_min.png",
        .txtFile = "assets/txt/13_shimbell_min.txt",
        .txtColorsFile = "",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Матрица минимальных расстояний Шимбелла",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {14, {
        .pngFile = "assets/png/13_shimbell_max.png",
        .txtFile = "assets/txt/13_shimbell_max.txt",
        .txtColorsFile = "",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Матрица максимальных расстояний Шимбелла",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {31, {
        .pngFile = "assets/png/31_flow_network.png",
        .txtFile = "assets/txt/31_flow.txt",
        .txtColorsFile = "",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Сеть потоков",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {32, {
        .pngFile = "assets/png/32_max_flow.png",
        .txtFile = "assets/txt/31_flow.txt",
        .txtColorsFile = "",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Максимальный поток",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {33, {
        .pngFile = "assets/png/33_min_cost_flow_path.png",
        .txtFile = "assets/txt/33_flow.txt",
        .txtColorsFile = "",
        .txtPathsFile = "assets/txt/33_path.txt",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Путь минимальной стоимости",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {34, {
        .pngFile = "assets/png/34_capacity_matrix.png",
        .txtFile = "assets/txt/34_capacity_matrix.txt",
        .txtColorsFile = "",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Матрица пропускных способностей",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {35, {
        .pngFile = "assets/png/35_cost_matrix.png",
        .txtFile = "assets/txt/35_cost_matrix.txt",
        .txtColorsFile = "",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Матрица стоимостей",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {42, {
        .pngFile = "assets/png/42_independent_set.png",
        .txtFile = "assets/txt/42_graph.txt",
        .txtColorsFile = "assets/txt/42_colors.txt",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Максимальное независимое множество вершин",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {43, {
        .pngFile = "assets/png/43_independent_edges.png",
        .txtFile = "assets/txt/43_graph.txt",
        .txtColorsFile = "",
        .txtPathsFile = "assets/txt/43_paths.txt",
        .txtGraphFile = "assets/txt/43_added_edges.txt",
        .gifFile = "",
        .title = "Максимальное независимое множество рёбер",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {44, {
        .pngFile = "assets/png/44_vertex_cover.png",
        .txtFile = "assets/txt/44_graph.txt",
        .txtColorsFile = "assets/txt/44_colors.txt",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Минимальное вершинное покрытие",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {45, {
        .pngFile = "assets/png/45_edge_cover.png",
        .txtFile = "assets/txt/45_graph.txt",
        .txtColorsFile = "",
        .txtPathsFile = "assets/txt/45_paths.txt",
        .txtGraphFile = "assets/txt/45_added_edges.txt",
        .gifFile = "",
        .title = "Минимальное реберное покрытие",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {46, {
        .pngFile = "assets/png/46_coloring.png",
        .txtFile = "assets/txt/46_graph.txt",
        .txtColorsFile = "assets/txt/46_colors.txt",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Раскраска графа",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {51, {
        .pngFile = "assets/png/51_euler_cycle.png",
        .txtFile = "assets/txt/51_graph.txt",
        .txtColorsFile = "",
        .txtPathsFile = "assets/txt/51_path.txt",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Эйлеров цикл",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {52, {
        .pngFile = "assets/png/52_hamilton_cycle.png",
        .txtFile = "assets/txt/52_graph.txt",
        .txtColorsFile = "",
        .txtPathsFile = "assets/txt/52_path.txt",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Гамильтонов цикл",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }},
    {53, {
        .pngFile = "assets/png/53_tsp_cycle.png",
        .txtFile = "assets/txt/53_graph.txt",
        .txtColorsFile = "",
        .txtPathsFile = "assets/txt/53_path.txt",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "TSP-цикл",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    }}
};

const std::map<int, DrawData>& DrawDataConfig::getConfigs() {
    return configs_;
}

}  // namespace graph
