#include "Menu.h"

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
        std::cout << "[OK] Граф сгенерирован\n";
        graph_->printGraphInfo();
    };
    actions_[2] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                Visualizer::drawGraph(*graph_, "assets/png/graph.png", "Сгенерированный граф");
            },
            no_graph_msg_);
    };
    actions_[3] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                Visualizer::drawAdjacencyMatrix(*graph_, "assets/png/adjacency.png",
                                                "Матрица смежности");
            },
            no_graph_msg_);
    };
    actions_[4] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                Visualizer::drawWeightMatrix(*graph_, "assets/png/weights.png", "Матрица весов");
            },
            no_graph_msg_);
    };
    actions_[11] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                int path_length = readInt("Длина пути: ");
                lab1_runner_.setGraph(graph_.get());
                lab1_runner_.runShimbellMethod(path_length);
            },
            no_graph_msg_);
    };
    actions_[12] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                int from = readInt("Начальная вершина: ");
                int to = readInt("Конечная вершина: ");
                lab1_runner_.setGraph(graph_.get());
                int result = lab1_runner_.countPaths(from, to);
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
                    Visualizer::drawGraphWithPaths(*graph_, all_paths, "assets/png/paths.png",
                                                   "Найденные пути");
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
                    Visualizer::drawShimbellMatrix(
                        shimbell->min_distances, "assets/png/shimbell.png", "Мин матрица Шимбелла");
                    Visualizer::drawShimbellMatrix(shimbell->max_distances,
                                                   "assets/png/shimbell_max.png",
                                                   "Макс матрица Шимбелла");

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
        lab3_runner_.setNetwork(flow_net_.get());
        last_max_flow_ = 0.0;
        std::cout << "[OK] Сеть потоков сгенерирована: Вершин: " << flow_net_->vertexCount()
                  << ", Рёбер: " << flow_net_->edgeCount() / 2 << "\n";
    };
    actions_[32] = [this]() {
        checkAndRun(
            flow_net_,
            [&]() {
                int source = readInt("Исток: ");
                int sink = readInt("Сток: ");
                last_max_flow_ = lab3_runner_.findMaxFlow(source, sink);
                std::cout << "Максимальный поток: " << last_max_flow_ << "\n";
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
            auto result = lab3_runner_.findMinCostFlow(source, sink, target_flow);
            std::cout << "\n=== Результат ===\n";
            std::cout << "Минимальная стоимость: " << result.cost << "\n";
            std::cout << "Достигнутый поток: " << result.flow << "\n";
            if (!result.path.empty()) {
                std::cout << "Длина пути: " << result.path.size() << " узлов\n";
                std::cout << "[OK] Путь найден\n";
            } else {
                std::cout << "[WARN] Путь не найден\n";
            }
        } else if (!flow_net_) {
            std::cout << "[FAIL] " << no_flow_msg_ << "\n";
        } else {
            std::cout << "[FAIL] " << no_max_flow_msg_ << "\n";
        }
    };
    actions_[34] = [this]() {
        checkAndRun(
            flow_net_,
            [&]() {
                Visualizer::drawFlowNetwork(*flow_net_, "assets/png/flow.png", "Сеть потоков");
            },
            no_flow_msg_);
    };
    actions_[35] = [this]() {
        checkAndRun(
            flow_net_,
            [&]() {
                auto result = lab3_runner_.getLastMinCostResult();
                if (!result.path.empty()) {
                    Visualizer::drawFlowNetworkWithPath(*flow_net_, result.path,
                                                        "assets/png/min_cost_flow_path.png",
                                                        "Путь минимальной стоимости");
                } else {
                    std::cout << "[FAIL] " << no_min_cost_msg_ << "\n";
                }
            },
            no_flow_msg_);
    };
    actions_[36] = [this]() {
        checkAndRun(
            flow_net_,
            [&]() {
                Visualizer::drawCapacityMatrix(*flow_net_, "assets/png/capacity_matrix.png",
                                               "Матрица пропускных способностей");
            },
            no_flow_msg_);
    };
    actions_[37] = [this]() {
        checkAndRun(
            flow_net_,
            [&]() {
                Visualizer::drawCostMatrix(*flow_net_, "assets/png/cost_matrix.png",
                                           "Матрица стоимостей");
            },
            no_flow_msg_);
    };
    actions_[38] = [this]() {
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
                lab4_runner_.setGraph(graph_.get());
                int count = lab4_runner_.countSpanningTrees();
                std::cout << "Количество остовных деревьев: " << count << "\n";
            },
            no_graph_msg_);
    };
    actions_[42] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                std::vector<int> set;
                int n = readInt("Размер множества: ");
                for (int i = 0; i < n; ++i) {
                    set.push_back(readInt(("Вершина " + std::to_string(i + 1) + ": ").c_str()));
                }
                lab4_runner_.setGraph(graph_.get());
                auto result = lab4_runner_.checkIndependentSet(set);
                std::cout << "Независимое множество: " << (result.is_independent ? "Да" : "Нет") << "\n";
            },
            no_graph_msg_);
    };
    actions_[43] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                lab4_runner_.setGraph(graph_.get());
                auto result = lab4_runner_.findChromaticNumber();
                std::cout << "Хроматическое число: " << result.chromatic_number << "\n";
                if (!result.coloring.empty()) {
                    std::cout << "Раскраска: ";
                    for (size_t i = 0; i < result.coloring.size(); ++i) {
                        std::cout << "v" << i << ":" << result.coloring[i] << " ";
                    }
                    std::cout << "\n";
                }
            },
            no_graph_msg_);
    };
    actions_[44] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                lab4_runner_.setGraph(graph_.get());
                auto result = lab4_runner_.findMaxIndependentSet();
                std::cout << "Максимальное независимое множество: ";
                for (int v : result.independent_set) {
                    std::cout << v << " ";
                }
                std::cout << "\n";
            },
            no_graph_msg_);
    };
    actions_[45] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto const& result = lab4_runner_.getLastCombinatorics();
                if (result && !result->coloring.empty()) {
                    Visualizer::drawColoredGraph(*graph_, result->coloring, "assets/png/coloring.png",
                                                 "Раскраска графа");
                } else {
                    std::cout << "[FAIL] Сначала найдите хроматическое число (пункт 43)\n";
                }
            },
            no_graph_msg_);
    };
    actions_[51] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                lab5_runner_.setGraph(graph_.get());
                lab5_runner_.checkEulerian();
            },
            no_graph_msg_);
    };
    actions_[52] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                lab5_runner_.setGraph(graph_.get());
                lab5_runner_.checkHamiltonian();
            },
            no_graph_msg_);
    };
    actions_[53] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                lab5_runner_.setGraph(graph_.get());
                lab5_runner_.solveTSP();
            },
            no_graph_msg_);
    };
    actions_[54] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto tsp = lab5_runner_.getLastTSPCycle();
                if (tsp && !tsp->empty()) {
                    Visualizer::drawGraphWithPath(*graph_, *tsp, "assets/png/tsp_cycle.png",
                                                  "TSP-цикл");
                } else {
                    std::cout << "[FAIL] " << no_tsp_msg_ << "\n";
                }
            },
            no_graph_msg_);
    };
    actions_[55] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto hamilton = lab5_runner_.getLastHamiltonianCycle();
                auto added_edges = lab5_runner_.getLastHamiltonianAddedEdges();
                if (hamilton && !hamilton->empty()) {
                    Visualizer::drawGraphWithPath(*graph_, *hamilton, added_edges,
                                                  "assets/png/hamilton_cycle.png",
                                                  "Гамильтонов цикл");
                } else {
                    std::cout << "[FAIL] " << no_hamilton_msg_ << "\n";
                }
            },
            no_graph_msg_);
    };
    actions_[56] = [this]() {
        checkAndRun(
            graph_,
            [&]() {
                auto euler = lab5_runner_.getLastEulerianCycle();
                auto added_edges = lab5_runner_.getLastEulerianAddedEdges();
                if (euler && !euler->empty()) {
                    Visualizer::drawGraphWithPath(*graph_, *euler, added_edges,
                                                  "assets/png/euler_cycle.png", "Эйлеров цикл");
                } else {
                    std::cout << "[FAIL] " << no_euler_msg_ << "\n";
                }
            },
            no_graph_msg_);
    };
}

void Menu::show() const {
    std::cout << "\n=== Единое меню лабораторных работ ===\n";

    std::cout << "\n[Общее]\n";
    std::cout << "1 - Сгенерировать граф\n";
    std::cout << "2 - Визуализировать граф\n";
    std::cout << "3 - Визуализировать матрицу смежности\n";
    std::cout << "4 - Визуализировать матрицу весов\n";

    std::cout << "\n[Lab 1 - Метод Шимбелла и подсчёт путей]\n";
    std::cout << "11 - Метод Шимбелла\n";
    std::cout << "12 - Подсчёт количества маршрутов\n";
    std::cout << "13 - Визуализировать найденный путь\n";
    std::cout << "14 - Визуализировать матрицу Шимбелла\n";

    std::cout << "\n[Lab 3 - Потоки]\n";
    std::cout << "31 - Сгенерировать сеть потоков\n";
    std::cout << "32 - Поиск максимального потока\n";
    std::cout << "33 - Поиск потока минимальной стоимости\n";
    std::cout << "34 - Визуализировать сеть\n";
    std::cout << "35 - Визуализировать путь минимальной стоимости\n";
    std::cout << "36 - Визуализировать матрицу пропускных способностей\n";
    std::cout << "37 - Визуализировать матрицу стоимостей\n";
    std::cout << "38 - Анимация роста потока (создать и показать)\n";

    std::cout << "\n[Lab 5 - Циклы]\n";
    std::cout << "51 - Проверка эйлеровости\n";
    std::cout << "52 - Проверка гамильтоновости\n";
    std::cout << "53 - Задача коммивояжера (TSP)\n";
    std::cout << "54 - Визуализировать TSP-цикл\n";
    std::cout << "55 - Визуализировать Гамильтонов цикл\n";
    std::cout << "56 - Визуализировать Эйлеров цикл\n";
    std::cout << "\n[Lab 4 - Остовы и комбинаторика]\n";
    std::cout << "40 - Число остовных деревьев (Кирхгоф)\n";
    std::cout << "41 - Максимальное независимое множество вершин\n";
    std::cout << "42 - Максимальное независимое множество ребер\n";
    std::cout << "43 - Минимальное вершинное покрытие\n";
    std::cout << "44 - Минимальное реберное покрытие\n";
    std::cout << "45 - Минимальная раскраска графа\n";
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
