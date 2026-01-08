#include "Menu.h"

namespace graph {

Menu::Menu() {
    actions_[1] = [this]() {
        bool isDirected = bool(readInt("Ориентированный граф? (1 - да, 0 - нет): "));
        int numVertices = readInt("Количество вершин: ");
        int numEdges = readInt("Количество рёбер: ");
        graph_ = gen_.generateAcyclicGraph(numVertices, numEdges, isDirected);
        std::cout << "[OK] Граф сгенерирован\n";
        graph_->printGraphInfo();
    };
    actions_[2] = [this]() {
        checkAndRun(graph_, [&]() {
            Visualizer::drawGraph(*graph_, "assets/png/graph.png", "Сгенерированный граф");
        }, noGraphMsg_);
    };
    actions_[3] = [this]() {
        checkAndRun(graph_, [&]() {
            Visualizer::drawAdjacencyMatrix(*graph_, "assets/png/adjacency.png", "Матрица смежности");
        }, noGraphMsg_);
    };
    actions_[4] = [this]() {
        checkAndRun(graph_, [&]() {
            Visualizer::drawWeightMatrix(*graph_, "assets/png/weights.png", "Матрица весов");
        }, noGraphMsg_);
    };
    actions_[11] = [this]() {
        checkAndRun(graph_, [&]() {
            int pathLength = readInt("Длина пути: ");
            lab1Runner_.setGraph(graph_.get());
            lab1Runner_.runShimbellMethod(pathLength);
        }, noGraphMsg_);
    };
    actions_[12] = [this]() {
        checkAndRun(graph_, [&]() {
            int from = readInt("Начальная вершина: ");
            int to = readInt("Конечная вершина: ");
            lab1Runner_.setGraph(graph_.get());
            int result = lab1Runner_.countPaths(from, to);
            std::cout << "Количество путей: " << result << "\n";
        }, noGraphMsg_);
    };
    actions_[13] = [this]() {
        checkAndRun(graph_, [&]() {
            const auto& allPaths = lab1Runner_.getAllPaths();
            if (!allPaths.empty()) {
                Visualizer::drawGraphWithPaths(*graph_, allPaths, "assets/png/paths.png", "Найденные пути");
            } else {
                std::cout << "[FAIL] Сначала найдите пути (пункт 12)\n";
            }
        }, noGraphMsg_);
    };
    actions_[14] = [this]() {
        checkAndRun(graph_, [&]() {
            auto shimbell = lab1Runner_.getLastShimbell();
            if (shimbell) {
                Visualizer::drawShimbellMatrix(shimbell->minDistances, "assets/png/shimbell.png", "Мин матрица Шимбелла");
                Visualizer::drawShimbellMatrix(shimbell->maxDistances, "assets/png/shimbell_max.png", "Макс матрица Шимбелла");

            } else {
                std::cout << "[FAIL] Сначала вычислите матрицу Шимбелла (пункт 11)\n";
            }
        }, noGraphMsg_);
    };
    actions_[31] = [this]() {
        int numVertices = readInt("Количество вершин: ");
        int numEdges = readInt("Количество рёбер: ");
        flowNet_ = gen_.generateFlowNetwork(numVertices, numEdges);
        lab3Runner_.setNetwork(flowNet_.get());
        lastMaxFlow_ = 0.0;
        std::cout << "[OK] Сеть потоков сгенерирована: Вершин: "
                << flowNet_->vertexCount() << ", Рёбер: " << flowNet_->edgeCount() / 2 << "\n";
    };
    actions_[32] = [this]() {
        checkAndRun(flowNet_, [&]() {
            int source = readInt("Исток: ");
            int sink = readInt("Сток: ");
            lastMaxFlow_ = lab3Runner_.findMaxFlow(source, sink);
            std::cout << "Максимальный поток: " << lastMaxFlow_ << "\n";
        }, noFlowMsg_);
    };
    actions_[33] = [this]() {
        if (flowNet_ && lastMaxFlow_ > 0.0) {
            int source = readInt("Исток: ");
            int sink = readInt("Сток: ");
            double defaultTarget = (2.0 / 3.0) * lastMaxFlow_;
            std::cout << "Целевой поток [2/3 максимального = " << defaultTarget << "], использовать его? (1 - да, 0 - нет): ";
            bool useDefault = bool(readInt(""));
            double targetFlow = 0.0;
            if (useDefault == 1) {
                targetFlow = defaultTarget;
            } else {
                targetFlow = static_cast<double>(readInt("Введите целевой поток: "));
            }
            auto result = lab3Runner_.findMinCostFlow(source, sink, targetFlow);
            std::cout << "\n=== Результат ===\n";
            std::cout << "Минимальная стоимость: " << result.cost << "\n";
            std::cout << "Достигнутый поток: " << result.flow << "\n";
            if (!result.path.empty()) {
                std::cout << "Длина пути: " << result.path.size() << " узлов\n";
                std::cout << "[OK] Путь найден\n";
            } else {
                std::cout << "[WARN] Путь не найден\n";
            }
        } else if (!flowNet_) {
            std::cout << "[FAIL] " << noFlowMsg_ << "\n";
        } else {
            std::cout << "[FAIL] " << noMaxFlowMsg_ << "\n";
        }
    };
    actions_[34] = [this]() {
        checkAndRun(flowNet_, [&]() {
            Visualizer::drawFlowNetwork(*flowNet_, "assets/png/flow.png", "Сеть потоков");
        }, noFlowMsg_);
    };
    actions_[35] = [this]() {
        checkAndRun(flowNet_, [&]() {
            auto result = lab3Runner_.getLastMinCostResult();
            if (!result.path.empty()) {
                Visualizer::drawFlowNetworkWithPath(*flowNet_, result.path,
                    "assets/png/min_cost_flow_path.png", "Путь минимальной стоимости");
            } else {
                std::cout << "[FAIL] " << noMinCostMsg_ << "\n";
            }
        }, noFlowMsg_);
    };
    actions_[36] = [this]() {
        checkAndRun(flowNet_, [&]() {
            Visualizer::drawCapacityMatrix(*flowNet_,
                "assets/png/capacity_matrix.png", "Матрица пропускных способностей");
        }, noFlowMsg_);
    };
    actions_[37] = [this]() {
        checkAndRun(flowNet_, [&]() {
            Visualizer::drawCostMatrix(*flowNet_,
                "assets/png/cost_matrix.png", "Матрица стоимостей");
        }, noFlowMsg_);
    };
    actions_[38] = [this]() {
        if (flowNet_) {
            try {
                Animator::animateFlowGrowth();
            } catch (const std::exception& e) {
                std::cout << "[FAIL] " << e.what() << "\n";
                std::cout << "[INFO] " << noMaxFlowMsg_ << "\n";
            }
        } else {
            std::cout << "[FAIL] " << noFlowMsg_ << "\n";
        }
    };
    actions_[51] = [this]() {
        checkAndRun(graph_, [&]() {
            lab5Runner_.setGraph(graph_.get());
            lab5Runner_.checkEulerian();
        }, noGraphMsg_);
    };
    actions_[52] = [this]() {
        checkAndRun(graph_, [&]() {
            lab5Runner_.setGraph(graph_.get());
            lab5Runner_.checkHamiltonian();
        }, noGraphMsg_);
    };
    actions_[53] = [this]() {
        checkAndRun(graph_, [&]() {
            lab5Runner_.setGraph(graph_.get());
            lab5Runner_.solveTSP();
        }, noGraphMsg_);
    };
    actions_[54] = [this]() {
        checkAndRun(graph_, [&]() {
            auto tsp = lab5Runner_.getLastTSPCycle();
            if (tsp && !tsp->empty()) {
                Visualizer::drawGraphWithPath(*graph_, *tsp,
                    "assets/png/tsp_cycle.png", "TSP-цикл");
            } else {
                std::cout << "[FAIL] " << noTSPMsg_ << "\n";
            }
        }, noGraphMsg_);
    };
    actions_[55] = [this]() {
        checkAndRun(graph_, [&]() {
            auto hamilton = lab5Runner_.getLastHamiltonianCycle();
            auto addedEdges = lab5Runner_.getLastHamiltonianAddedEdges();
            if (hamilton && !hamilton->empty()) {
                Visualizer::drawGraphWithPath(*graph_, *hamilton, addedEdges,
                    "assets/png/hamilton_cycle.png", "Гамильтонов цикл");
            } else {
                std::cout << "[FAIL] " << noHamiltonMsg_ << "\n";
            }
        }, noGraphMsg_);
    };
    actions_[56] = [this]() {
        checkAndRun(graph_, [&]() {
            auto euler = lab5Runner_.getLastEulerianCycle();
            auto addedEdges = lab5Runner_.getLastEulerianAddedEdges();
            if (euler && !euler->empty()) {
                Visualizer::drawGraphWithPath(*graph_, *euler, addedEdges,
                    "assets/png/euler_cycle.png", "Эйлеров цикл");
            } else {
                std::cout << "[FAIL] " << noEulerMsg_ << "\n";
            }
        }, noGraphMsg_);
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
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << "\n";
    }
}

} // namespace graph
