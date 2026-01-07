#include <iostream>
#include <memory>
#include <Graph.h>
#include <GraphGenerator.h>
#include <GraphVisualizer.h>
#include <Utils.h>
#include "lab1/menu.h"
#include "lab3/menu.h"
#include "lab5/menu.h"

using namespace graph;

int main() {
    std::unique_ptr<Graph> graph;
    std::unique_ptr<FlowNetwork> flowNet;
    double lastMaxFlow = 0.0;
    bool running = true;

    while (running) {
        std::cout << "\n=== Единое меню лабораторных работ ===\n";
        std::cout << "\n[Общее]\n";
        std::cout << "1 - Сгенерировать граф (для Lab 1 и Lab 5)\n";
        std::cout << "2 - Вывести граф\n";
        std::cout << "3 - Вывести матрицу смежности (1 - есть ребро, - - нет)\n";
        std::cout << "4 - Вывести матрицу весов\n";
        std::cout << "5 - Визуализировать граф (assets/graph.png)\n";
        std::cout << "6 - Визуальизировать матрицу смежности (assets/adjacency.png)\n";
        std::cout << "7 - Визуализировать матрицу весов (assets/weights.png)\n";
        std::cout << "\n[Lab 1 - Метод Шимбелла и подсчёт путей]\n";
        std::cout << "11 - Метод Шимбелла\n";
        std::cout << "12 - Подсчёт количества маршрутов\n";
        std::cout << "\n[Lab 3 - Потоки (отдельный граф)]\n";
        std::cout << "31 - Сгенерировать сеть потоков\n";
        std::cout << "32 - Поиск максимального потока (Форд-Фалкерсон)\n";
        std::cout << "33 - Поиск потока минимальной стоимости (Беллман-Форд)\n";
        std::cout << "34 - Вывести матрицы пропускных способностей и стоимостей\n";
        std::cout << "35 - Визуализировать поток (assets/flow.png)\n";
        std::cout << "\n[Lab 5 - Циклы]\n";
        std::cout << "51 - Проверка эйлеровости\n";
        std::cout << "52 - Проверка гамильтоновости\n";
        std::cout << "53 - Задача коммивояжёра (TSP)\n";
        std::cout << "\n0 - Выход\n";
        std::cout << "\nВаш выбор: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int numVertices = readInt("Количество вершин: ");
                int numEdges = readInt("Количество рёбер: ");
                graph = generateRandomGraph(numVertices, numEdges);
                std::cout << "[OK] Граф сгенерирован\n";
                graph->printGraphInfo();
                break;
            }
            case 2:
                if (graph) {
                    graph->printGraph();
                } else {
                    std::cout << "[FAIL] Сначала сгенерируйте граф (пункт 1)\n";
                }
                break;
            case 3:
                if (graph) {
                    graph->printAdjacencyMatrix();
                } else {
                    std::cout << "[FAIL] Сначала сгенерируйте граф (пункт 1)\n";
                }
                break;
            case 4:
                if (graph) {
                    graph->printWeightMatrix();
                } else {
                    std::cout << "[FAIL] Сначала сгенерируйте граф (пункт 1)\n";
                }
                break;
            case 5:
                if (graph) {
                    GraphVisualizer::drawGraph(*graph, "assets/graph.png");
                } else {
                    std::cout << "[FAIL] Сначала сгенерируйте граф (пункт 1)\n";
                }
                break;
            case 6:
                if (graph) {
                    GraphVisualizer::drawAdjacencyMatrix(*graph, "assets/adjacency.png");
                } else {
                    std::cout << "[FAIL] Сначала сгенерируйте граф (пункт 1)\n";
                }
                break;
            case 7:
                if (graph) {
                    GraphVisualizer::drawWeightMatrix(*graph, "assets/weights.png");
                } else {
                    std::cout << "[FAIL] Сначала сгенерируйте граф (пункт 1)\n";
                }
                break;
            case 11:
                if (graph) lab1::runShimbellMethod(*graph);
                else std::cout << "[FAIL] Сначала сгенерируйте граф (пункт 1)\n";
                break;
            case 12:
                if (graph) lab1::countPathsBetweenVertices(*graph);
                else std::cout << "[FAIL] Сначала сгенерируйте граф (пункт 1)\n";
                break;
            case 31: {
                if (!flowNet) flowNet = std::make_unique<FlowNetwork>();
                lab3::generateNetwork(*flowNet);
                lastMaxFlow = 0.0;
                break;
            }
            case 32: {
                if (!flowNet || flowNet->vertexIds().empty()) {
                    std::cout << "[FAIL] Сначала сгенерируйте сеть потоков (пункт 31)\n";
                } else {
                    lastMaxFlow = lab3::findMaxFlow(*flowNet);
                }
                break;
            }
            case 33: {
                if (!flowNet || flowNet->vertexIds().empty()) {
                    std::cout << "[FAIL] Сначала сгенерируйте сеть потоков (пункт 31)\n";
                } else if (lastMaxFlow == 0.0) {
                    std::cout << "[FAIL] Сначала найдите максимальный поток (пункт 32)\n";
                } else {
                    lab3::findMinCostFlow(*flowNet, lastMaxFlow);
                }
                break;
            }
            case 34: {
                if (!flowNet || flowNet->vertexIds().empty()) {
                    std::cout << "[FAIL] Сначала сгенерируйте сеть потоков (пункт 31)\n";
                } else {
                    lab3::printGraphInfo(*flowNet);
                }
                break;
            }
            case 35:
                if (!flowNet || flowNet->vertexIds().empty()) {
                    std::cout << "[FAIL] Сначала сгенерируйте сеть потоков (пункт 31)\n";
                } else {
                    GraphVisualizer::drawFlowNetwork(*flowNet, "assets/flow.png");
                }
                break;
            case 51:
                if (graph) lab5::checkEulerian(*graph);
                else std::cout << "[FAIL] Сначала сгенерируйте граф (пункт 1)\n";
                break;
            case 52:
                if (graph) lab5::checkHamiltonian(*graph);
                else std::cout << "[FAIL] Сначала сгенерируйте граф (пункт 1)\n";
                break;
            case 53:
                if (graph) lab5::solveTSP(*graph);
                else std::cout << "[FAIL] Сначала сгенерируйте граф (пункт 1)\n";
                break;
            case 0:
                std::cout << "Выход из программы.\n";
                running = false;
                break;
            default:
                std::cout << "Неверный выбор.\n";
        }
    }

    return 0;
}
