#include <iostream>
#include <memory>
#include <functional>
#include <Graph.h>
#include <Generator.h>
#include <Visualizer.h>
#include <Utils.h>
#include "lab1/include/Runner.h"
#include "lab3/include/Runner.h"
#include "lab5/include/Runner.h"

using namespace graph;

int main() {
    std::unique_ptr<Graph> graph;
    std::unique_ptr<FlowNetwork> flowNet;
    Generator gen;
    lab1::Runner lab1Runner;
    lab3::Runner lab3Runner;
    lab5::Runner lab5Runner;
    double lastMaxFlow = 0.0;
    bool running = true;
    const char* noGraphMsg = "Сначала сгенерируйте граф (пункт 1)";
    const char* noFlowMsg = "Сначала сгенерируйте сеть потоков (пункт 31)";

    while (running) {
        showMenu();
        int choice;
        std::cin >> choice;

        try {
            switch (choice) {
                case 1: {
                    bool isDirected = bool(readInt("Ориентированный граф? (1 - да, 0 - нет): "));
                    int numVertices = readInt("Количество вершин: ");
                    int numEdges = readInt("Количество рёбер: ");
                    graph = gen.generateAcyclicGraph(numVertices, numEdges, isDirected);
                    std::cout << "[OK] Граф сгенерирован\n";
                    graph->printGraphInfo();
                    break;
                }
                case 2:
                    checkAndRun(graph, [&]() {
                        Visualizer::drawGraph(*graph, "assets/png/graph.png", "Сгенерированный граф");
                    }, noGraphMsg);
                    break;
                case 3:
                    checkAndRun(graph, [&]() {
                        Visualizer::drawAdjacencyMatrix(*graph, "assets/png/adjacency.png", "Матрица смежности");
                    }, noGraphMsg);
                    break;
                case 4:
                    checkAndRun(graph, [&]() {
                        Visualizer::drawWeightMatrix(*graph, "assets/png/weights.png", "Матрица весов");
                    }, noGraphMsg);
                    break;
                case 11:
                    checkAndRun(graph, [&]() {
                        lab1Runner.setGraph(graph.get());
                        int length = readInt("Длина пути: ");
                        lab1Runner.runShimbellMethod(length);
                        auto shimbell = lab1Runner.getLastShimbell();
                        if (shimbell) {
                            Visualizer::drawShimbellMatrix(shimbell->minDistances,
                                "assets/png/lab1_shimbell_min.png", "Матрица Шимбелла (минимальные расстояния)");
                            Visualizer::drawShimbellMatrix(shimbell->maxDistances,
                                "assets/png/lab1_shimbell_max.png", "Матрица Шимбелла (максимальные расстояния)");
                        }
                    }, noGraphMsg);
                    break;
                case 12:
                    checkAndRun(graph, [&]() {
                        lab1Runner.setGraph(graph.get());
                        auto result = lab1Runner.countPaths(readInt("От вершины: "), readInt("До вершины: "));
                        std::cout << "Найдено путей: " << result.pathCount << "\n";
                    }, noGraphMsg);
                    break;
                case 13:
                    checkAndRun(graph, [&]() {
                        auto lastPath = lab1Runner.getLastPath();
                        if (lastPath && !lastPath->path.empty()) {
                            Visualizer::drawGraphWithPath(*graph, lastPath->path, "assets/png/lab1_path.png", "Найденный путь");
                        } else {
                            std::cout << "[FAIL] Сначала найдите путь\n";
                        }
                    }, noGraphMsg);
                    break;
                case 14: {
                    auto shimbell = lab1Runner.getLastShimbell();
                    if (shimbell) {
                        Visualizer::drawShimbellMatrix(shimbell->minDistances,
                                "assets/png/lab1_shimbell_min.png", "Матрица Шимбелла (минимальные расстояния)");
                        Visualizer::drawShimbellMatrix(shimbell->maxDistances,
                            "assets/png/lab1_shimbell_max.png", "Матрица Шимбелла (максимальные расстояния)");
                    } else {
                        std::cout << "[FAIL] Сначала выполните метод Шимбелла\n";
                    }
                    break;
                }
                case 31: {
                    int numVertices = readInt("Количество вершин: ");
                    int numEdges = readInt("Количество рёбер: ");
                    flowNet = gen.generateFlowNetwork(numVertices, numEdges);
                    lab3Runner.setNetwork(flowNet.get());
                    lastMaxFlow = 0.0;
                    std::cout << "[OK] Сеть потоков сгенерирована: Вершин: "
                            << flowNet->vertexCount() << ", Рёбер: " << flowNet->edgeCount() / 2 << "\n";
                    break;
                }
                case 32:
                    checkAndRun(flowNet, [&]() {
                        int source = readInt("Исток: ");
                        int sink = readInt("Сток: ");
                        lastMaxFlow = lab3Runner.findMaxFlow(source, sink);
                        std::cout << "Максимальный поток: " << lastMaxFlow << "\n";
                    }, noFlowMsg);
                    break;
                case 33:
                    if (flowNet && lastMaxFlow > 0.0) {
                        int source = readInt("Исток: ");
                        int sink = readInt("Сток: ");
                        double defaultTarget = (2.0 / 3.0) * lastMaxFlow;
                        std::cout << "Целевой поток [2/3 максимального = " << defaultTarget << "], использовать его? (1 - да, 0 - нет): ";
                        bool useDefault = bool(readInt(""));
                        double targetFlow = 0.0;
                        if (useDefault == 1) {
                            targetFlow = defaultTarget;
                        } else {
                            targetFlow = static_cast<double>(readInt("Введите целевой поток: "));
                        }
                        auto result = lab3Runner.findMinCostFlow(source, sink, targetFlow);
                        std::cout << "\n=== Результат ===\n";
                        std::cout << "Минимальная стоимость: " << result.cost << "\n";
                        std::cout << "Достигнутый поток: " << result.flow << "\n";
                        if (!result.path.empty()) {
                            std::cout << "Длина пути: " << result.path.size() << " узлов\n";
                            std::cout << "[OK] Путь найден\n";
                        } else {
                            std::cout << "[WARN] Путь не найден\n";
                        }
                    } else if (!flowNet) {
                        std::cout << "[FAIL] " << noFlowMsg << "\n";
                    } else {
                        std::cout << "[FAIL] Сначала найдите максимальный поток (пункт 32)\n";
                    }
                    break;
                case 34:
                    checkAndRun(flowNet, [&]() {
                        Visualizer::drawFlowNetwork(*flowNet, "assets/png/flow.png", "Сеть потоков");
                    }, noFlowMsg);
                    break;
                case 35:
                    if (flowNet) {
                        auto result = lab3Runner.getLastMinCostResult();
                        if (!result.path.empty()) {
                            Visualizer::drawFlowNetworkWithPath(*flowNet, result.path,
                                "assets/png/min_cost_flow_path.png", "Путь минимальной стоимости");
                        } else {
                            std::cout << "[FAIL] Сначала найдите путь минимальной стоимости (пункт 33)\n";
                        }
                    } else {
                        std::cout << "[FAIL] " << noFlowMsg << "\n";
                    }
                    break;
                case 36:
                    checkAndRun(flowNet, [&]() {
                        Visualizer::drawCapacityMatrix(*flowNet,
                            "assets/png/capacity_matrix.png", "Матрица пропускных способностей");
                    }, noFlowMsg);
                    break;
                case 37:
                    checkAndRun(flowNet, [&]() {
                        Visualizer::drawCostMatrix(*flowNet,
                            "assets/png/cost_matrix.png", "Матрица стоимостей");
                    }, noFlowMsg);
                    break;
                case 51:
                    checkAndRun(graph, [&]() {
                        lab5Runner.setGraph(graph.get());
                        lab5Runner.checkEulerian();
                    }, noGraphMsg);
                    break;
                case 52:
                    checkAndRun(graph, [&]() {
                        lab5Runner.setGraph(graph.get());
                        lab5Runner.checkHamiltonian();
                    }, noGraphMsg);
                    break;
                case 53:
                    checkAndRun(graph, [&]() {
                        lab5Runner.setGraph(graph.get());
                        lab5Runner.solveTSP();
                    }, noGraphMsg);
                    break;
                case 54:
                    checkAndRun(graph, [&]() {
                        auto tsp = lab5Runner.getLastTSPCycle();
                        if (tsp && !tsp->empty()) {
                            Visualizer::drawGraphWithPath(*graph, *tsp,
                                "assets/png/tsp_cycle.png", "TSP-цикл");
                        } else {
                            std::cout << "[FAIL] Сначала найдите TSP-цикл (пункт 53)\n";
                        }
                    }, noGraphMsg);
                    break;
                case 55:
                    checkAndRun(graph, [&]() {
                        auto hamilton = lab5Runner.getLastHamiltonianCycle();
                        auto addedEdges = lab5Runner.getLastHamiltonianAddedEdges();
                        if (hamilton && !hamilton->empty()) {
                            Visualizer::drawGraphWithPath(*graph, *hamilton, addedEdges,
                                "assets/png/hamilton_cycle.png", "Гамильтонов цикл");
                        } else {
                            std::cout << "[FAIL] Сначала найдите Гамильтонов цикл (пункт 52)\n";
                        }
                    }, noGraphMsg);
                    break;
                case 56:
                    checkAndRun(graph, [&]() {
                        auto euler = lab5Runner.getLastEulerianCycle();
                        auto addedEdges = lab5Runner.getLastEulerianAddedEdges();
                        if (euler && !euler->empty()) {
                            Visualizer::drawGraphWithPath(*graph, *euler, addedEdges,
                                "assets/png/euler_cycle.png", "Эйлеров цикл");
                        } else {
                            std::cout << "[FAIL] Сначала найдите Эйлеров цикл (пункт 51)\n";
                        }
                    }, noGraphMsg);
                    break;
                case 0:
                    std::cout << "Выход из программы.\n";
                    running = false;
                    break;
                default:
                    std::cout << "Неверный выбор.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] " << e.what() << "\n";
        }
    }

    return 0;
}
