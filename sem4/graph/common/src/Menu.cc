#include "Menu.h"
#include "DrawDataConfig.h"

namespace graph {

Menu::Menu() {
    initializeActions();
}

void Menu::initializeActions() {
    actions_[1] = [this]() {
        bool is_directed = static_cast<bool>(readInt("Ориентированный граф? (1 - да, 0 - нет): "));
        int num_vertices = readInt("Количество вершин: ");
        int num_edges = readInt("Количество рёбер: ");
        graph_ = gen_.generateAcyclicGraph(num_vertices, num_edges, is_directed);
        checkAndRun(
            graph_,
            [&]() {
                auto data = DrawDataConfig::getConfigs().at(1);
                Visualizer::drawGraph(*graph_, data);
                std::cout << "[OK] Граф отрисован в assets/png/01_graph.png\n";
            },
            no_graph_msg_);
    };
    actions_[2] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto data = DrawDataConfig::getConfigs().at(2);
                Visualizer::drawAdjacencyMatrix(*graph_, data);
                std::cout << "[OK] Матрица смежности сохранена в assets/png/03_adjacency_matrix.png\n";
            },
            no_graph_msg_);
    };
    actions_[3] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto data = DrawDataConfig::getConfigs().at(3);
                Visualizer::drawWeightMatrix(*graph_, data);
                std::cout << "[OK] Матрица весов сохранена в assets/png/04_weight_matrix.png\n";
            },
            no_graph_msg_);
    };
    actions_[11] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                int path_length = readInt("Длина пути: ");
                lab1_runner_.runShimbellMethod(*graph_, path_length);
            },
            no_graph_msg_);
    };
    actions_[12] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                int from = readInt("Начальная вершина: ");
                int to = readInt("Конечная вершина: ");
                int result = lab1_runner_.countPaths(*graph_, from, to);
                std::cout << "Количество путей: " << result << "\n";
            },
            no_graph_msg_);
    };
    actions_[13] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto const& all_paths = lab1_runner_.getAllPaths();
                if (!all_paths.empty()) {
                    auto data = DrawDataConfig::getConfigs().at(13);
                    data.paths = {all_paths[0]};
                    Visualizer::drawGraphWithPaths(*graph_, data);
                    std::cout << "[OK] Путь сохранен в assets/png/13_paths.png\n";
                } else {
                    std::cout << "[FAIL] Сначала найдите пути (пункт 12)\n";
                }
            },
            no_graph_msg_);
    };
    actions_[14] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto const* const shimbell = lab1_runner_.getLastShimbell();
                if (shimbell) {
                    auto dataMin = DrawDataConfig::getConfigs().at(14);
                    dataMin.title = "Мин матрица Шимбелла";
                    dataMin.pngFile = "assets/png/14_shimbell_min.png";
                    dataMin.txtFile = "assets/txt/14_shimbell_min.txt";

                    auto dataMax = DrawDataConfig::getConfigs().at(14);
                    dataMax.title = "Макс матрица Шимбелла";
                    dataMax.pngFile = "assets/png/14_shimbell_max.png";
                    dataMax.txtFile = "assets/txt/14_shimbell_max.txt";

                    Visualizer::drawShimbellMatrix(shimbell->min_distances, dataMin);
                    Visualizer::drawShimbellMatrix(shimbell->max_distances, dataMax);

                } else {
                    std::cout << "[FAIL] Сначала вычислите матрицу Шимбелла (пункт 11)\n";
                }
            },
            no_graph_msg_);
    };
    actions_[31] = [this]() {
        int num_vertices = readInt("Количество вершин: ");
        int num_edges = readInt("Количество рёбер: ");
        flow_net_ = gen_.generateFlowNetwork(num_vertices, num_edges);
        last_max_flow_ = 0.0;
        auto data = DrawDataConfig::getConfigs().at(31);
        Visualizer::drawFlowNetwork(*flow_net_, data);
    };
    actions_[32] = [this]() {
        checkAndRun(
            flow_net_,
            [&]() {
                int source = readInt("Исток: ");
                int sink = readInt("Сток: ");
                last_max_flow_ = lab3_runner_.findMaxFlow(*flow_net_, source, sink);
                std::cout << "Максимальный поток: " << last_max_flow_ << "\n";
                auto data = DrawDataConfig::getConfigs().at(32);
                Visualizer::drawFlowNetwork(*flow_net_, data);
                Animator::animateFlowGrowth();
            },
            no_flow_msg_);
    };
    actions_[33] = [this]() {
        if (flow_net_ && last_max_flow_ > 0.0) {
            int source = readInt("Исток: ");
            int sink = readInt("Сток: ");
            double default_target = (2.0 / 3.0) * last_max_flow_;
            std::cout << "Целевой поток [2/3 максимального = " << default_target
                      << "], использовать его? (1 - да, 0 - нет): ";
            bool use_default = static_cast<bool>(readInt(""));
            double target_flow = 0.0;
            if (use_default == 1) {
                target_flow = default_target;
            } else {
                target_flow = static_cast<double>(readInt("Введите целевой поток: "));
            }
            auto result = lab3_runner_.findMinCostFlow(*flow_net_, source, sink, target_flow);
            std::cout << "\n=== Результат ===\n";
            std::cout << "Минимальная стоимость: " << result.cost << "\n";
            std::cout << "Достигнутый поток: " << result.flow << "\n";
            if (!result.path.empty()) {
                std::cout << "Длина пути: " << result.path.size() << " узлов\n";
                auto data = DrawDataConfig::getConfigs().at(34);
                data.path = result.path;
                Visualizer::drawFlowNetworkWithPath(*flow_net_, data);
            } else {
                std::cout << "[WARN] Путь не найден\n";
            }
        } else if (!flow_net_) {
            std::cout << "[FAIL] " << no_flow_msg_ << "\n";
        } else {
            std::cout << "[FAIL] " << no_max_flow_msg_ << "\n";
        }
    };
    actions_[35] = [this]() {
        checkAndRun(
            flow_net_,
            [&]() {
                auto data = DrawDataConfig::getConfigs().at(35);
                Visualizer::drawCapacityMatrix(*flow_net_, data);
            },
            no_flow_msg_);
    };
    actions_[36] = [this]() {
        checkAndRun(
            flow_net_,
            [&]() {
                auto data = DrawDataConfig::getConfigs().at(36);
                Visualizer::drawCostMatrix(*flow_net_, data);
            },
            no_flow_msg_);
    };
    actions_[37] = [this]() {
        if (flow_net_) {
            try {
                Animator::animateFlowGrowth();
            } catch (std::exception const& e) {
                std::cout << "[FAIL] " << e.what() << "\n";
                std::cout << "[INFO] " << no_max_flow_msg_ << "\n";
            }
        } else {
            std::cout << "[FAIL] " << no_flow_msg_ << "\n";
        }
    };
    actions_[41] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                int count = lab4_runner_.countSpanningTrees(*graph_);
                std::cout << "Количество остовных деревьев: " << count << "\n";
            },
            no_graph_msg_);
    };
    actions_[42] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto result = lab4_runner_.findMaxIndependentSet(*graph_);
                std::cout << "Максимальное независимое множество вершин (размер " << result.independent_set.size() << "): ";
                for (int v : result.independent_set) {
                    std::cout << v << " ";
                }
                std::cout << "\n";

                auto data = DrawDataConfig::getConfigs().at(42);
                std::vector<int> colors(graph_->vertexCount(), 0);
                for (int v : result.independent_set) {
                    colors[v] = 1;
                }
                data.colors = colors;
                Visualizer::drawColoredGraph(*graph_, data);
                std::cout << "[OK] Независимое множество сохранено в assets/png/42_independent_set.png\n";
            },
            no_graph_msg_);
    };
    actions_[43] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto result = lab4_runner_.findMaxIndependentSetEdges(*graph_);
                std::cout << "Максимальное независимое множество рёбер (размер " << result.independent_edges.size() << "): ";
                for (auto const& [u, v] : result.independent_edges) {
                    std::cout << "(" << u << "," << v << ") ";
                }
                std::cout << "\n";

                auto data = DrawDataConfig::getConfigs().at(43);
                data.addedEdges = result.independent_edges;
                data.paths = {{}};
                Visualizer::drawGraphWithPath(*graph_, data);
                std::cout << "[OK] Независимые рёбра сохранены в assets/png/43_independent_edges.png\n";
            },
            no_graph_msg_);
    };
    actions_[44] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto result = lab4_runner_.findMinVertexCover(*graph_);
                std::cout << "Минимальное вершинное покрытие (размер " << result.vertex_cover.size() << "): ";
                for (int v : result.vertex_cover) {
                    std::cout << v << " ";
                }
                std::cout << "\n";

                auto data = DrawDataConfig::getConfigs().at(44);
                std::vector<int> colors(graph_->vertexCount(), 0);
                for (int v : result.vertex_cover) {
                    colors[v] = 1;
                }
                data.colors = colors;
                Visualizer::drawColoredGraph(*graph_, data);
                std::cout << "[OK] Вершинное покрытие сохранено в assets/png/44_vertex_cover.png\n";
            },
            no_graph_msg_);
    };
    actions_[45] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto result = lab4_runner_.findMinEdgeCover(*graph_);
                std::cout << "Минимальное реберное покрытие (размер " << result.edge_cover.size() << "): ";
                for (auto const& [u, v] : result.edge_cover) {
                    std::cout << "(" << u << "," << v << ") ";
                }
                std::cout << "\n";

                auto data = DrawDataConfig::getConfigs().at(45);
                data.addedEdges = result.edge_cover;
                data.paths = {{}};
                Visualizer::drawGraphWithPath(*graph_, data);
                std::cout << "[OK] Реберное покрытие сохранено в assets/png/45_edge_cover.png\n";
            },
            no_graph_msg_);
    };
    actions_[46] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto result = lab4_runner_.findMinColoring(*graph_);
                std::cout << "Хроматическое число: " << result.chromatic_number << "\n";

                auto data = DrawDataConfig::getConfigs().at(46);
                data.colors = result.coloring;
                Visualizer::drawColoredGraph(*graph_, data);
            },
            no_graph_msg_);
    };
    actions_[47] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto const& result = lab4_runner_.getLastCombinatorics();
                if (result && !result->coloring.empty()) {
                    auto data = DrawDataConfig::getConfigs().at(47);
                    data.colors = result->coloring;
                    Visualizer::drawColoredGraph(*graph_, data);
                } else {
                    std::cout << "[FAIL] Сначала найдите раскраску (пункт 46)\n";
                }
            },
            no_graph_msg_);
    };
    actions_[51] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                lab5_runner_.checkEulerian(*graph_);

                auto euler = lab5_runner_.getLastEulerianCycle();
                auto added_edges = lab5_runner_.getLastEulerianAddedEdges();
                if (euler && !euler->empty()) {
                    auto data = DrawDataConfig::getConfigs().at(51);
                    data.path = *euler;
                    data.addedEdges = added_edges;
                    Visualizer::drawGraphWithPath(*graph_, data);
                } else {
                    std::cout << "[FAIL] " << no_euler_msg_ << "\n";
                }
            },
            no_graph_msg_);
    };
    actions_[52] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                lab5_runner_.checkHamiltonian(*graph_);

                auto hamilton = lab5_runner_.getLastHamiltonianCycle();
                auto added_edges = lab5_runner_.getLastHamiltonianAddedEdges();
                if (hamilton && !hamilton->empty()) {
                    auto data = DrawDataConfig::getConfigs().at(52);
                    data.path = *hamilton;
                    data.addedEdges = added_edges;
                    Visualizer::drawGraphWithPath(*graph_, data);
                } else {
                    std::cout << "[FAIL] " << no_hamilton_msg_ << "\n";
                }
            },
            no_graph_msg_);
    };
    actions_[53] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                lab5_runner_.solveTSP(*graph_);

                auto tsp = lab5_runner_.getLastTSPCycle();
                if (tsp && !tsp->empty()) {
                    auto data = DrawDataConfig::getConfigs().at(53);
                    data.path = *tsp;
                    Visualizer::drawGraphWithPath(*graph_, data);
                } else {
                    std::cout << "[FAIL] " << no_tsp_msg_ << "\n";
                }
            },
            no_graph_msg_);
    };

}

void Menu::show() const {
    std::cout << "\n=== Единое меню лабораторных работ ===\n";

    std::cout << "\n[Общее]\n";
    std::cout << "1 - Сгенерировать граф\n";
    std::cout << "2 - Визуализировать матрицу смежности\n";
    std::cout << "3 - Визуализировать матрицу весов\n";

    std::cout << "\n[Lab 1 - Метод Шимбелла и подсчёт путей]\n";
    std::cout << "11 - Метод Шимбелла\n";
    std::cout << "12 - Подсчёт количества маршрутов\n";
    std::cout << "13 - Визуализировать найденный путь\n";
    std::cout << "14 - Визуализировать матрицу Шимбелла\n";

    std::cout << "\n[Lab 3 - Потоки]\n";
    std::cout << "31 - Сгенерировать сеть потоков\n";
    std::cout << "32 - Поиск максимального потока\n";
    std::cout << "33 - Поиск потока минимальной стоимости\n";
    std::cout << "34 - Визуализировать путь минимальной стоимости\n";
    std::cout << "35 - Визуализировать матрицу пропускных способностей\n";
    std::cout << "36 - Визуализировать матрицу стоимостей\n";

    std::cout << "\n[Lab 4 - Остовы и комбинаторика]\n";
    std::cout << "41 - Число остовных деревьев (Кирхгоф)\n";
    std::cout << "42 - Макс. независимое множество вершин\n";
    std::cout << "43 - Макс. независимое множество рёбер\n";
    std::cout << "44 - Мин. вершинное покрытие\n";
    std::cout << "45 - Мин. рёберное покрытие\n";
    std::cout << "46 - Минимальная раскраска графа\n";

    std::cout << "\n[Lab 5 - Циклы]\n";
    std::cout << "51 - Проверка эйлеровости\n";
    std::cout << "52 - Проверка гамильтоновости\n";
    std::cout << "53 - Задача коммивояжера (TSP)\n";

    std::cout << "\n0 - Выход\n";
}

void Menu::handleChoice(int choice) {
    try {
        if (actions_.count(choice)) {
            actions_[choice]();
        } else if (choice == 0) {
            std::cout << "Выход из программы.\n";
        } else {
            std::cout << "Неверный выбор.\n";
        }
    } catch (std::exception const& e) {
        std::cerr << "[ERROR] " << e.what() << "\n";
    }
}

}  // namespace graph
