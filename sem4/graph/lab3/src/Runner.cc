#include "../include/Runner.h"

#include "../include/MaxFlow.h"
#include "../include/MinCostFlow.h"

#include <iomanip>
#include <iostream>
#include <cmath>
#include <random>
#include <sstream>

#include <Generator.h>
#include <Menu.h>
#include <FileHandler.h>
#include <DrawDataConfig.h>
#include <Visualizer.h>

namespace lab3 {

using graph::MaxFlow;
using graph::MinCostFlow;
using graph::Generator;
using graph::FileHandler;
using graph::DrawDataConfig;
using graph::Visualizer;
using graph::VisualizationType;

void Runner::runGenerateFlowNetwork() {
    int num_vertices = graph::readInt("Количество вершин: ");
    int num_edges = graph::readInt("Количество рёбер: ");
    int cost_sign_val = graph::readInt("Знак стоимостей (0-положит., 1-отрицат., 2-смешанные): ");
    auto cost_sign = static_cast<graph::WeightSign>(cost_sign_val);
    network_ = Generator{}.generateFlowNetwork(num_vertices, num_edges, cost_sign);
    auto data = DrawDataConfig::getConfigs().at(31);
    FileHandler::saveFlowNetwork(data.txtFile, *network_);
    Visualizer::draw(data, network_->isDirected(), VisualizationType::FlowNetwork);
    std::cout << "[OK] Сеть потоков отрисована в assets/png/31_flow_network.png\n";
}

void Runner::runGenerateFlowNetworkByDegrees() {
    int num_vertices = graph::readInt("Количество вершин: ");
    int a = graph::readInt("Параметр a (масштаб, a > 0): ");
    int h = graph::readInt("Параметр h (форма, h > 0): ");
    int cost_sign_val = graph::readInt("Знак стоимостей (0-положит., 1-отрицат., 2-смешанные): ");
    auto cost_sign = static_cast<graph::WeightSign>(cost_sign_val);
    network_ = Generator{}.generateFlowNetworkByDegrees(
        num_vertices, a, h, graph::EdgeCountDist::TruncatedNormal, cost_sign);
    auto data = DrawDataConfig::getConfigs().at(31);
    FileHandler::saveFlowNetwork(data.txtFile, *network_);
    Visualizer::draw(data, network_->isDirected(), VisualizationType::FlowNetwork);
    std::cout << "[OK] Сеть потоков (Райс, a=" << a << ", h=" << h
              << ") отрисована в assets/png/31_flow_network.png\n";
}

void Runner::runBuildFlowNetworkFromGraph(graph::Graph const& graph) {
    if (graph.vertexCount() == 0 || graph.edgeCount() == 0) {
        std::cout << "[FAIL] Сначала сгенерируйте граф в Lab 1\n";
        return;
    }

    int a = graph::readInt("Параметр a (масштаб, a > 0): ");
    int h = graph::readInt("Параметр h (форма, h > 0): ");

    network_ = std::make_unique<FlowNetwork>(graph.isDirected());

    for (int v : graph.vertexIds()) {
        network_->addVertex(v);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> normal_dist(0.0, 1.0);

    auto riceCost = [&]() {
        double u1 = normal_dist(gen);
        double u2 = normal_dist(gen);
        return std::sqrt(std::pow(h + a * u1, 2) + std::pow(a * u2, 2));
    };

    for (auto const& edge : graph.edges()) {
        double capacity = std::abs(edge.weight);
        if (capacity <= 0.0) {
            capacity = 1.0;
        }
        double cost = riceCost();
        network_->addEdge(edge.from, edge.to, capacity, cost);
    }

    auto data = DrawDataConfig::getConfigs().at(31);
    data.title = "Сеть потоков, построенная из графа";
    FileHandler::saveFlowNetwork(data.txtFile, *network_);
    Visualizer::draw(data, network_->isDirected(), VisualizationType::FlowNetwork);
    std::cout << "[OK] Сеть fromNetwork построена из графа; стоимости сгенерированы по Райсу (a="
              << a << ", h=" << h << ")\n";
}

void Runner::runMaxFlow() {
    if (!network_) {
        std::cout << "[FAIL] Сначала сгенерируйте сеть\n";
        return;
    }
    int source = graph::readInt("Исток: ");
    int sink = graph::readInt("Сток: ");

    if (source == sink) {
        std::cout << "[FAIL] Исток и сток должны быть разными\n";
        return;
    }

    MaxFlow max_flow_algo(*network_);
    const double computed_max_flow = max_flow_algo.fordFulkerson(source, sink, true);
    last_max_flow_ = computed_max_flow;

    max_flow_algo.exportSnapshots("assets/txt/32_flow_snapshots.txt");

    std::cout << "\n[OK] Максимальный поток: " << std::fixed << std::setprecision(2) << last_max_flow_ << "\n";

    auto data = DrawDataConfig::getConfigs().at(32);
    std::ostringstream title_stream;
    title_stream << std::fixed << std::setprecision(2) << last_max_flow_;
    data.title = "Максимальный поток: " + title_stream.str();
    FileHandler::saveFlowNetwork(data.txtFile, *network_);
    Visualizer::draw(data, network_->isDirected(), VisualizationType::FlowNetwork);
    Visualizer::draw(data, network_->isDirected(), VisualizationType::Animation);
}

void Runner::runMinCostFlow() {
    if (!network_) {
        std::cout << "[FAIL] Сначала сгенерируйте сеть\n";
        return;
    }
    if (last_max_flow_ <= 0.0) {
        std::cout << "[FAIL] Сначала вычислите максимальный поток\n";
        return;
    }

    int source = graph::readInt("Исток: ");
    int sink = graph::readInt("Сток: ");

    if (source == sink) {
        std::cout << "[FAIL] Исток и сток должны быть разными\n";
        return;
    }

    double default_target = std::floor((2.0 / 3.0) * last_max_flow_);
    std::cout << "Целевой поток [floor(2/3 * max) = " << std::fixed << std::setprecision(2)
              << default_target << "], использовать его? (1 - да, 0 - нет): ";
    bool use_default = static_cast<bool>(graph::readInt(""));
    double target_flow = use_default ? default_target : graph::readDouble("Введите целевой поток: ");

    MinCostFlow min_cost_flow_algo(*network_);
    auto result = min_cost_flow_algo.findMinCostFlow(source, sink, target_flow);

    std::cout << "\n=== Результат ===\n";
    std::cout << "Минимальная стоимость: " << result.cost << "\n";
    std::cout << "Достигнутый поток: " << result.flow << "\n";
    std::cout << "Успех: " << (result.success ? "да" : "нет") << "\n";

    if (!result.steps.empty()) {
        std::cout << "\n=== Подробности по шагам ===\n";
        for (auto const& step : result.steps) {
            std::cout << "Шаг " << step.iteration << ": путь ";
            for (size_t i = 0; i < step.path.size(); ++i) {
                std::cout << step.path[i];
                if (i + 1 < step.path.size()) std::cout << " -> ";
            }
            const double delta_cost = step.pathFlow * step.pathCost;
            std::cout << "\n  Добавленный поток: " << step.pathFlow
                      << "\n  Стоимость пути за 1 ед.: " << step.pathCost
                      << "\n  Приращение стоимости: " << delta_cost
                      << "\n  Накопленный поток: " << step.cumulativeFlow
                      << "\n  Накопленная стоимость: " << step.cumulativeCost << "\n";
        }
    }

    if (!result.path.empty()) {
        auto data = DrawDataConfig::getConfigs().at(33);
        data.path = result.path;
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << result.cost;
        data.title = "Путь минимальной стоимости (цена: " + ss.str() + ")";
        FileHandler::saveFlowNetwork(data.txtFile, *network_);
        std::vector<std::vector<int>> paths;
        paths.reserve(result.steps.size());
        for (auto const& step : result.steps) {
            if (!step.path.empty()) {
                paths.push_back(step.path);
            }
        }
        if (paths.empty()) {
            paths.push_back(data.path);
        }
        FileHandler::savePaths(data.txtPathsFile, paths);
        Visualizer::drawPaths(data, network_->isDirected(), VisualizationType::FlowNetwork);
    } else {
        std::cout << "[WARN] Путь не найден\n";
    }
}

void Runner::runVisualizeCapacityMatrix() {
    if (!network_) {
        std::cout << "[FAIL] Сначала сгенерируйте сеть\n";
        return;
    }
    auto data = DrawDataConfig::getConfigs().at(34);
    FileHandler::saveCapacityMatrix(data.txtFile, *network_);
    Visualizer::drawMatrix(data, "Матрица пропускных способностей");
    std::cout << "[OK] Матрица пропускных способностей сохранена\n";
}

void Runner::runVisualizeCostMatrix() {
    if (!network_) {
        std::cout << "[FAIL] Сначала сгенерируйте сеть\n";
        return;
    }
    auto data = DrawDataConfig::getConfigs().at(35);
    FileHandler::saveCostMatrix(data.txtFile, *network_);
    Visualizer::drawMatrix(data, "Матрица стоимостей");
    std::cout << "[OK] Матрица стоимостей сохранена\n";
}

}  // namespace lab3
