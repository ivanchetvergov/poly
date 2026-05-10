#include "../include/Runner.h"

#include "../include/CutSystem.h"
#include "../include/EulerianCycle.h"
#include "../../lab4/include/Boruvka.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <cctype>

#include <FileHandler.h>
#include <DrawDataConfig.h>
#include <Utils.h>
#include <Visualizer.h>

namespace lab5 {

using graph::EulerianCycle;
using graph::CutSystem;
using graph::Boruvka;
using graph::FileHandler;
using graph::DrawDataConfig;
using graph::Visualizer;
using graph::DrawData;
using graph::VisualizationType;
using graph::readInt;

namespace {

std::string edgeToString(std::pair<int,int> const& e) {
    std::ostringstream out;
    out << "(" << e.first << ", " << e.second << ")";
    return out.str();
}

std::string cutEdgesToText(std::vector<std::pair<int,int>> const& edges) {
    std::ostringstream out;
    for (size_t i = 0; i < edges.size(); ++i) {
        out << edgeToString(edges[i]);
        if (i + 1 < edges.size()) out << " ";
    }
    return out.str();
}

std::string verticesToText(std::vector<int> const& vertices) {
    std::ostringstream out;
    out << "{";
    for (size_t i = 0; i < vertices.size(); ++i) {
        out << vertices[i];
        if (i + 1 < vertices.size()) out << ", ";
    }
    out << "}";
    return out.str();
}

std::string pathToArrowText(std::vector<int> const& path) {
    std::ostringstream out;
    for (size_t i = 0; i < path.size(); ++i) {
        out << path[i];
        if (i + 1 < path.size()) out << " -> ";
    }
    return out.str();
}

std::unique_ptr<Graph> cloneGraph(Graph const& src) {
    auto copy = std::make_unique<Graph>(src.isDirected());
    for (int v : src.vertexIds()) {
        copy->addVertex(v);
    }
    for (auto const& e : src.edges()) {
        copy->addEdge(e.from, e.to, e.weight);
    }
    return copy;
}

}  // namespace

void Runner::runCheckEulerian(Graph& graph) {
    if (graph.isDirected() || graph.vertexCount() == 0) {
        std::cout << "[FAIL] Граф должен быть неориентированным и инициализированным\n";
        return;
    }

    auto data = DrawDataConfig::getConfigs().at(51);
    auto workingGraph = cloneGraph(graph);
    EulerianCycle euler(*workingGraph);

    std::cout << "\n=== Проверка эйлеровости ===\n";

    FileHandler::saveGraph(data.txtFile, graph);

    Visualizer::draw(data, false, VisualizationType::Graph);
    try {
        DrawData orig = data;
        orig.pngFile = "assets/png/51_graph_original.png";
        FileHandler::saveGraph(orig.txtFile, graph);
        Visualizer::draw(orig, false, VisualizationType::Graph);
    } catch (...) {
    }

    // ── Достройка до замкнутого эйлерова цикла ───────────────────────────────
    if (euler.isEulerian()) {
        std::cout << "[OK] Граф уже является эйлеровым\n";
    } else if (euler.isSemiEulerian()) {
        std::cout << "[INFO] Граф полуэйлеровый — пытаемся замкнуть в эйлеров цикл, если это возможно\n";
    } else {
        std::cout << "[INFO] Граф не эйлеров, вершин с нечётной степенью: "
                  << euler.getOddDegreeVertices().size() << " — выполняется достройка\n";
    }

    euler.makeEulerian();

    auto const& added = euler.getAddedEdges();
    if (!added.empty()) {
        std::cout << "[INFO] Добавлено рёбер: " << added.size() << "\n";
        for (auto const& [u, v] : added) {
            std::cout << "  + " << u << " -- " << v << "\n";
        }
    }

    auto const& removed = euler.getRemovedEdges();
    if (!removed.empty()) {
        std::cout << "[INFO] Удалено рёбер: " << removed.size() << "\n";
        for (auto const& [u, v] : removed) {
            std::cout << "  - " << u << " -- " << v << "\n";
        }
    }

    if (!euler.isEulerian() && !euler.isSemiEulerian()) {
        std::cout << "[FAIL] После модификации граф не является ни эйлеровым, ни полуэйлеровым. "
                  << "Вершин с нечётной степенью: " << euler.getOddDegreeVertices().size() << "\n";
        return;
    }
    if (euler.isEulerian()) {
        std::cout << "[OK] После модификации граф эйлеровый\n";
    } else {
        std::cout << "[OK] После модификации граф полуэйлеровый\n";
    }

    auto cycle = euler.findCycle();
    if (!cycle.has_value()) {
        std::cout << "[FAIL] Не удалось найти эйлеров цикл после модификации\n";
        return;
    }

    bool closed = !cycle->empty() && cycle->front() == cycle->back();
    std::cout << "[OK] Найден маршрут, длина: " << cycle->size()
              << (closed ? " (замкнутый цикл)\n" : " (эйлеров путь)\n");
    std::cout << "[PATH] " << pathToArrowText(*cycle) << "\n";

    data.paths = {*cycle};
    FileHandler::savePaths(data.txtPathsFile, data.paths);
    FileHandler::saveAddedEdges(data.txtGraphFile, added);
    Visualizer::drawEulerCycle(data, VisualizationType::Graph);

    std::cout << "[OK] Результат визуализирован\n";
    std::cout << "[INFO] Исходный граф не изменён (модификация выполнялась на копии)\n";
}

void Runner::runFundamentalCuts(Graph const& graph) {
    if (graph.isDirected() || graph.vertexCount() < 2) {
        std::cout << "[FAIL] Граф должен быть неориентированным и содержать >= 2 вершин\n";
        return;
    }

    Boruvka boruvka;
    auto mst = boruvka.buildMST(graph);
    if (!mst || mst->edgeCount() == 0) {
        std::cout << "[FAIL] Не удалось построить кратчайший остов\n";
        return;
    }

    const size_t expectedMstEdges = graph.vertexCount() - 1;
    if (mst->edgeCount() != expectedMstEdges) {
        std::cout << "[FAIL] Некорректный остов: ожидалось " << expectedMstEdges
                  << " ребер, получено " << mst->edgeCount() << "\n";
        return;
    }

    std::cout << "\n=== Минимальный остов (Борувка) ===\n";
    double mstTotalWeight = 0.0;
    auto mstEdges = mst->edges();
    std::sort(mstEdges.begin(), mstEdges.end(), [](auto const& a, auto const& b) {
        if (a.from != b.from) return a.from < b.from;
        if (a.to != b.to) return a.to < b.to;
        return a.weight < b.weight;
    });
    for (auto const& e : mstEdges) {
        mstTotalWeight += e.weight;
        std::cout << "  " << e.from << " -- " << e.to << " (w=" << e.weight << ")\n";
    }
    std::cout << "[INFO] Вес остова: " << mstTotalWeight << "\n";

    // Снапшот MST для web: первая картинка слева.
    DrawData mstData{
        .pngFile = "assets/png/52_mst.png",
        .txtFile = "assets/txt/52_mst.txt",
        .txtColorsFile = "",
        .txtPathsFile = "",
        .txtGraphFile = "",
        .gifFile = "",
        .title = "Минимальный остов (Борувка)",
        .colors = {},
        .path = {},
        .addedEdges = {},
        .paths = {}
    };
    FileHandler::saveGraph(mstData.txtFile, *mst);
    Visualizer::draw(mstData, false, VisualizationType::Graph);

    CutSystem cutSystem(graph, *mst);
    auto fundamentalCuts = cutSystem.buildFundamentalCuts();

    std::ostringstream report;
    report << "Фундаментальная система разрезов:\n";

    std::cout << "\n=== Фундаментальная система разрезов ===\n";
    for (size_t i = 0; i < fundamentalCuts.size(); ++i) {
        auto const& c = fundamentalCuts[i];
         std::cout << i << ") Удаляем ребро остова " << edgeToString(c.treeEdge)
                << ". Остов распадается на две части:\n";
         std::cout << "    A = " << verticesToText(c.leftComponent)
                << ", B = " << verticesToText(c.rightComponent) << "\n";
         std::cout << "    Рёбра исходного графа, которые пересекают границу A|B: { "
                << cutEdgesToText(c.cutEdges) << " }\n";

         report << i << ") Удаляем ребро остова " << edgeToString(c.treeEdge)
             << ". Остов распадается на две части:\n";
         report << "    A = " << verticesToText(c.leftComponent)
             << ", B = " << verticesToText(c.rightComponent) << "\n";
         report << "    Рёбра исходного графа, которые пересекают границу A|B: { "
             << cutEdgesToText(c.cutEdges) << " }\n";
    }

    // Вычисляем XOR по ВСЕм разрезам
    std::vector<int> allIndices;
    for (size_t i = 0; i < fundamentalCuts.size(); ++i) {
        allIndices.push_back(static_cast<int>(i));
    }

    auto symDiff = cutSystem.symmetricDifference(fundamentalCuts, allIndices);
    std::cout << "\n[INFO] Симметрическая разность по всем разрезам (индексы: ";
    for (int idx : allIndices) std::cout << idx << " ";
    std::cout << "): { " << cutEdgesToText(symDiff) << " }\n";

    report << "\n[INFO] Симметрическая разность по всем разрезам (индексы: ";
    for (int idx : allIndices) report << idx << " ";
    report << "): { " << cutEdgesToText(symDiff) << " }\n";

    auto data = DrawDataConfig::getConfigs().at(52);
    data.addedEdges = symDiff;

    FileHandler::saveGraph(data.txtFile, graph);
    FileHandler::saveAddedEdges(data.txtGraphFile, data.addedEdges);
    FileHandler::saveToFile("assets/txt/52_fundamental_cuts.txt", report.str());
    Visualizer::draw(data, false, VisualizationType::Graph);
    std::cout << "[OK] Разрезы построены и визуализированы\n";
}

void Runner::runSymmetricDifferenceSubset(Graph const& graph) {
    if (graph.isDirected() || graph.vertexCount() < 2) {
        std::cout << "[FAIL] Граф должен быть неориентированным и содержать >= 2 вершин\n";
        return;
    }

    std::cout << "\n=== Симметрическая разность подмножества разрезов ===\n";

    // Строим MST и ФСР
    Boruvka boruvka;
    auto mst = boruvka.buildMST(graph);
    if (!mst || mst->edgeCount() == 0) {
        std::cout << "[FAIL] Не удалось построить кратчайший остов\n";
        return;
    }

    const size_t expectedMstEdges = graph.vertexCount() - 1;
    if (mst->edgeCount() != expectedMstEdges) {
        std::cout << "[FAIL] Некорректный остов: ожидалось " << expectedMstEdges
                  << " ребер, получено " << mst->edgeCount() << "\n";
        return;
    }

    CutSystem cutSystem(graph, *mst);
    auto fundamentalCuts = cutSystem.buildFundamentalCuts();

    std::cout << "\nФундаментальная система разрезов:\n";
    for (size_t i = 0; i < fundamentalCuts.size(); ++i) {
        auto const& c = fundamentalCuts[i];
        std::cout << i << ") Ребро остова " << edgeToString(c.treeEdge)
                  << " | Рёбра разреза: { " << cutEdgesToText(c.cutEdges) << " }\n";
    }

    std::cout << "\nВыберите подмножество разрезов для XOR:\n";
    std::cout << "Введите номера разрезов в одной строке через пробел или запятую: ";

    std::string line;
    std::cin >> std::ws;
    std::getline(std::cin, line);
    if (line == "empty") line = "";

    for (char& ch : line) {
        if (ch == ',') {
            ch = ' ';
        }
    }

    std::istringstream input(line);
    std::vector<int> selected;
    int idx = 0;
    while (input >> idx) {
        selected.push_back(idx);
    }

    std::vector<int> validSelected;
    validSelected.reserve(selected.size());
    for (int idx : selected) {
        if (idx < 0 || idx >= static_cast<int>(fundamentalCuts.size())) {
            std::cout << "[WARN] Игнорируется некорректный индекс разреза: " << idx << "\n";
            continue;
        }
        validSelected.push_back(idx);
    }

    if (selected.empty()) {
        std::cout << "[INFO] Введен пустой набор (пустое множество разрезов).\n";
        std::cout << "[INFO] Симметрическая разность пустого набора разрезов — это пустое множество рёбер.\n";
        std::cout << "[INFO] Алгебраически, пустое множество является корректным разрезом (соответствует нулевому вектору в пространстве разрезов, порождённом ФСР).\n";
    } else if (validSelected.empty()) {
        std::cout << "[FAIL] Нет корректных индексов разрезов\n";
        return;
    }

    std::cout << "\nВыбранные разрезы:\n";
    for (int idx : validSelected) {
        auto const& c = fundamentalCuts[idx];
        std::cout << idx << ") Ребро остова " << edgeToString(c.treeEdge)
                  << " | Рёбра разреза: { " << cutEdgesToText(c.cutEdges) << " }\n";
    }

    auto symDiff = cutSystem.symmetricDifference(fundamentalCuts, validSelected);

    std::cout << "\nПроцесс XOR:\n";
    std::unordered_map<unsigned long long, int> edgeCount;
    auto encode = [](std::pair<int, int> e) {
        return (static_cast<unsigned long long>(e.first) << 32U) |
               static_cast<unsigned int>(e.second);
    };

    for (int idx : validSelected) {
        for (auto const& edge : fundamentalCuts[idx].cutEdges) {
            edgeCount[encode(edge)]++;
        }
    }

    std::cout << "Вхождения рёбер в выбранные разрезы:\n";
    for (auto const& [code, count] : edgeCount) {
        int u = static_cast<int>(code >> 32U);
        int v = static_cast<int>(code & 0xFFFFFFFFU);
        std::cout << "  (" << u << ", " << v << "): " << count
                  << " раз" << (count == 1 ? "а" : "");

    }

    std::cout << "\n[RESULT] Симметрическая разность (индексы: ";
    for (int idx : validSelected) std::cout << idx << " ";
    std::cout << "): { " << cutEdgesToText(symDiff) << " }\n";

    std::ostringstream report;
    report << "Фундаментальная система разрезов:\n";
    for (size_t i = 0; i < fundamentalCuts.size(); ++i) {
        auto const& c = fundamentalCuts[i];
        report << i << ") Удаляем ребро остова " << edgeToString(c.treeEdge)
               << ". Остов распадается на две части:\n";
        report << "    A = " << verticesToText(c.leftComponent)
               << ", B = " << verticesToText(c.rightComponent) << "\n";
        report << "    Рёбра исходного графа, которые пересекают границу A|B: { "
               << cutEdgesToText(c.cutEdges) << " }\n";
    }
    report << "\nВыбранные индексы: ";
    for (int idx : validSelected) report << idx << " ";
    report << "\nСимметрическая разность: { " << cutEdgesToText(symDiff) << " }\n";

    auto data = DrawDataConfig::getConfigs().at(52);
    data.addedEdges = symDiff;

    FileHandler::saveGraph(data.txtFile, graph);
    FileHandler::saveAddedEdges(data.txtGraphFile, data.addedEdges);
    FileHandler::saveToFile("assets/txt/53_fundamental_cuts_subset.txt", report.str());
    Visualizer::draw(data, false, VisualizationType::Graph);
    std::cout << "[OK] Подмножество разрезов построено и визуализировано\n";
}

}  // namespace lab5
