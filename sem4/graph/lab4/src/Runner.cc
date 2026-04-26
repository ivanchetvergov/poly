#include "Runner.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>

#include <FileHandler.h>
#include <DrawDataConfig.h>
#include <Visualizer.h>

namespace lab4 {

namespace {

std::vector<std::vector<double>> buildKirchhoffMinor(graph::Graph const& graph) {
    auto vertices = graph.vertexIds();
    std::sort(vertices.begin(), vertices.end());

    if (vertices.empty()) {
        return {};
    }

    if (vertices.size() == 1) {
        return {{0.0}};
    }

    std::vector<std::vector<double>> minor(vertices.size() - 1,
                                           std::vector<double>(vertices.size() - 1, 0.0));

    for (size_t i = 0; i + 1 < vertices.size(); ++i) {
        int u = vertices[i];
        double degree = 0.0;

        for (size_t j = 0; j + 1 < vertices.size(); ++j) {
            if (i == j) {
                continue;
            }

            int v = vertices[j];
            double weight = graph.getEdgeWeight(u, v).value_or(0.0);
            minor[i][j] = -weight;
        }

        for (int v : vertices) {
            if (v == u) {
                continue;
            }
            degree += graph.getEdgeWeight(u, v).value_or(0.0);
        }

        minor[i][i] = degree;
    }

    return minor;
}

std::string buildPruferSequenceText(graph::PruferCode::PruferEncoding const& encoding) {
    std::ostringstream out;

    out << "Вершины: ";
    if (encoding.vertices.empty()) {
        out << "(пусто)";
    } else {
        for (int vertex : encoding.vertices) {
            out << vertex << ' ';
        }
    }

    out << "\nПоследовательность Прюфера: ";
    if (encoding.sequence.empty()) {
        out << "(пусто)";
    } else {
        for (int value : encoding.sequence) {
            out << value << ' ';
        }
    }
    out << '\n';

    return out.str();
}

}  // namespace

using graph::Boruvka;
using graph::PruferCode;
using graph::GraphCombinatorics;
using graph::KirchhoffTheorem;
using graph::FileHandler;
using graph::DrawDataConfig;
using graph::Visualizer;

void Runner::runCountSpanningTrees(Graph const& graph) {
    if (graph.isDirected() || graph.vertexCount() == 0) {
        std::cout << "[FAIL] Граф должен быть неориентированным и инициализированным\n";
        return;
    }

    size_t count = KirchhoffTheorem::countSpanningTrees(graph);
    std::cout << "\n[OK] Количество остовных деревьев (матричная теорема Кирхгофа): " << count << "\n";

    auto data = DrawDataConfig::getConfigs().at(41);
    data.pngFile = "assets/png/41_kirchhoff_matrix_minor.png";
    data.txtFile = "assets/txt/41_kirchhoff_matrix_minor.txt";
    data.title = "Минор матрицы Кирхгофа";

    auto minor = buildKirchhoffMinor(graph);
    FileHandler::saveMatrix(data.txtFile, minor);
    Visualizer::drawMatrix(data, data.title);
    std::cout << "[OK] Минор матрицы Кирхгофа визуализирован\n";
}

void Runner::runBuildMinimalSpanningTree(Graph const& graph) {
    if (graph.isDirected() || graph.vertexCount() < 2) {
        std::cout << "[FAIL] Граф должен быть неориентированным и содержать >= 2 вершин\n";
        return;
    }

    Boruvka boruvka;
    last_mst_ = boruvka.buildMST(graph);

    if (!last_mst_) {
        std::cout << "[FAIL] Не удалось построить минимальный остов\n";
        return;
    }

    double total_weight = 0.0;
    for (int u : last_mst_->vertexIds()) {
        for (auto const& [v, weight] : last_mst_->neighbors(u)) {
            if (u < v) {
                total_weight += weight;
            }
        }
    }

    std::cout << "\n[OK] Минимальный остов построен (алгоритм Борувки)\n";
    std::cout << "Количество рёбер: " << last_mst_->edgeCount() << "\n";
    std::cout << "Общий вес: " << std::fixed << std::setprecision(2) << total_weight << "\n";

    auto data = DrawDataConfig::getConfigs().at(41);
    std::ostringstream weight_stream;
    weight_stream << std::fixed << std::setprecision(2) << total_weight;
    data.title = "Минимальный остов (вес: " + weight_stream.str() + ")";
    FileHandler::saveGraph(data.txtFile, *last_mst_);
    Visualizer::draw(data, false, graph::VisualizationType::Graph);
    std::cout << "[OK] Минимальный остов визуализирован\n";
}

void Runner::runPruferEncode(Graph const& /* graph */) {
    if (!last_mst_) {
        std::cout << "[FAIL] Сначала постройте минимальный остов\n";
        return;
    }

    PruferCode prufer;
    last_encoding_ = prufer.encode(*last_mst_);

    std::cout << "\n[OK] Кодирование Прюфера:\n";
    std::cout << "Последовательность: ";
    if (last_encoding_.sequence.empty()) {
        std::cout << "(пусто - дерево с <= 2 вершинами)";
    } else {
        for (int v : last_encoding_.sequence) {
            std::cout << v << " ";
        }
    }
    std::cout << "\n";

    std::cout << "Сохранённые веса рёбер: " << last_encoding_.edgeWeights.size() << " ребро(а)\n";
    FileHandler::saveToFile("assets/txt/43_prufer_sequence.txt", buildPruferSequenceText(last_encoding_));
}

void Runner::runPruferDecode(Graph const& /*graph*/) {
    if (last_encoding_.vertices.empty()) {
        std::cout << "[FAIL] Сначала кодируйте дерево\n";
        return;
    }

    PruferCode prufer;
    auto decoded_tree = prufer.decode(last_encoding_);

    if (!decoded_tree) {
        std::cout << "[FAIL] Ошибка при декодировании\n";
        return;
    }

    double total_weight = 0.0;
    for (int u : decoded_tree->vertexIds()) {
        for (auto const& [v, weight] : decoded_tree->neighbors(u)) {
            if (u < v) {
                total_weight += weight;
            }
        }
    }

    std::cout << "\n[OK] Декодирование Прюфера завершено\n";
    std::cout << "Восстановленное дерево: " << decoded_tree->vertexCount() << " вершин, "
              << decoded_tree->edgeCount() << " рёбер\n";
    std::cout << "Общий вес: " << std::fixed << std::setprecision(2) << total_weight << "\n";

    auto data = DrawDataConfig::getConfigs().at(44);
    data.title = "Декодированное дерево (вес: " + std::to_string(static_cast<int>(total_weight)) + ")";
    FileHandler::saveGraph(data.txtFile, *decoded_tree);
    FileHandler::saveToFile("assets/txt/44_prufer_sequence.txt", buildPruferSequenceText(last_encoding_));
    Visualizer::draw(data, false, graph::VisualizationType::Graph);
    std::cout << "[OK] Декодированное дерево визуализировано\n";
}

void Runner::runFindMinColoring(Graph const& graph) {
    if (graph.vertexCount() == 0) {
        std::cout << "[FAIL] Граф не инициализирован\n";
        return;
    }

    GraphCombinatorics gc;
    auto coloring = gc.findMinColoring(graph);
    auto vertices = graph.vertexIds();
    std::sort(vertices.begin(), vertices.end());

    int chromatic_number = 0;
    for (int color : coloring) {
        chromatic_number = std::max(chromatic_number, color + 1);
    }

    std::cout << "\n[OK] Минимальная раскраска графа\n";
    std::cout << "Хроматическое число: " << chromatic_number << "\n";
    std::cout << "Раскраска вершин: ";
    for (size_t i = 0; i < coloring.size(); ++i) {
        std::cout << vertices[i] << ":" << coloring[i] << " ";
    }
    std::cout << "\n";

    auto data = DrawDataConfig::getConfigs().at(46);
    data.colors = coloring;
    FileHandler::saveGraph(data.txtFile, graph);
    FileHandler::saveColors(data.txtColorsFile, vertices, data.colors);
    Visualizer::drawColoredGraph(data, graph.isDirected());
    std::cout << "[OK] Минимальная раскраска визуализирована\n";
}

}  // namespace lab4
