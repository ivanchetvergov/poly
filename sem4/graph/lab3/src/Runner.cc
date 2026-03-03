#include "../include/Runner.h"

#include "../include/MaxFlow.h"
#include "../include/MinCostFlow.h"

#include <iomanip>
#include <iostream>
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
    last_max_flow_ = max_flow_algo.fordFulkerson(source, sink, true);

    max_flow_algo.exportSnapshots("assets/txt/32_flow_snapshots.txt");

    std::cout << "\n[OK] Максимальный поток: " << std::fixed << std::setprecision(2) << last_max_flow_ << "\n";

    auto data = DrawDataConfig::getConfigs().at(32);
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << last_max_flow_;
    data.title = "Максимальный поток: " + ss.str();
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

    double default_target = (2.0 / 3.0) * last_max_flow_;
    std::cout << "Целевой поток [2/3 максимального = " << default_target << "], использовать его? (1 - да, 0 - нет): ";
    bool use_default = static_cast<bool>(graph::readInt(""));
    double target_flow = use_default ? default_target : graph::readInt("Введите целевой поток: ");

    MinCostFlow min_cost_flow_algo(*network_);
    auto result = min_cost_flow_algo.findMinCostFlow(source, sink, target_flow);

    std::cout << "\n=== Результат ===\n";
    std::cout << "Минимальная стоимость: " << result.cost << "\n";
    std::cout << "Достигнутый поток: " << result.flow << "\n";

    if (!result.path.empty()) {
        auto data = DrawDataConfig::getConfigs().at(33);
        data.path = result.path;
        FileHandler::saveFlowNetwork(data.txtFile, *network_);
        FileHandler::savePaths(data.txtPathsFile, {data.path});
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
