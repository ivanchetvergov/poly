#include "Menu.h"
#include "DrawDataConfig.h"
#include "FileHandler.h"
#include "Visualizer.h"
#include "../../lab6/include/Runner.h"
#include <iomanip>
#include <sstream>

namespace graph {

Menu::Menu() {
    initializeActions();
}

void Menu::initializeActions() {
    actions_[1] = [this]() { graph_ = lab1_runner_.generateGraph(); };
    actions_[4] = [this]() { graph_ = lab1_runner_.generateRayleighGraph(); };

    actions_[2] = [this]() {
        checkAndRun(graph_, [this]() { lab1_runner_.runVisualizeAdjacencyMatrix(*graph_); },
                    "Граф не инициализирован");
    };
    actions_[3] = [this]() {
        checkAndRun(graph_, [this]() { lab1_runner_.runVisualizeWeightMatrix(*graph_); },
                    "Граф не инициализирован");
    };

    actions_[11] = [this]() {
        checkAndRun(graph_, [this]() { lab1_runner_.runShimbellMethod(*graph_); },
                    "Граф не инициализирован");
    };
    actions_[12] = [this]() {
        checkAndRun(graph_, [this]() { lab1_runner_.runPathsMethod(*graph_); },
                    "Граф не инициализирован");
    };
    actions_[13] = [this]() {
        checkAndRun(graph_, [this]() { lab1_runner_.runGraphMetrics(*graph_); },
                    "Граф не инициализирован");
    };

    actions_[21] = [this]() {
        checkAndRun(graph_, [this]() { lab2_runner_.runBFS(*graph_); },
                    "Граф не инициализирован");
    };
    actions_[22] = [this]() {
        checkAndRun(graph_, [this]() { lab2_runner_.runDijkstra(*graph_); },
                    "Граф не инициализирован");
    };
    actions_[23] = [this]() {
        checkAndRun(graph_, [this]() { lab2_runner_.runCompare(*graph_); },
                    "Граф не инициализирован");
    };

    actions_[31] = [this]() { lab3_runner_.runGenerateFlowNetwork(); };
    actions_[36] = [this]() { lab3_runner_.runGenerateFlowNetworkByDegrees(); };
    actions_[37] = [this]() {
        checkAndRun(graph_, [this]() { lab3_runner_.runBuildFlowNetworkFromGraph(*graph_); },
                    "Сначала сгенерируйте граф в Lab 1");
    };
    actions_[32] = [this]() { lab3_runner_.runMaxFlow(); };
    actions_[33] = [this]() { lab3_runner_.runMinCostFlow(); };
    actions_[34] = [this]() { lab3_runner_.runVisualizeCapacityMatrix(); };
    actions_[35] = [this]() { lab3_runner_.runVisualizeCostMatrix(); };

    actions_[41] = [this]() { lab4_runner_.runCountSpanningTrees(*graph_); };
    actions_[42] = [this]() { lab4_runner_.runBuildMinimalSpanningTree(*graph_); };
    actions_[43] = [this]() { lab4_runner_.runPruferEncode(*graph_); };
    actions_[44] = [this]() { lab4_runner_.runPruferDecode(*graph_); };
    actions_[46] = [this]() { lab4_runner_.runFindMinColoring(*graph_); };

    actions_[51] = [this]() {
        checkAndRun(graph_, [this]() { lab5_runner_.runCheckEulerian(*graph_); },
                    "Граф не инициализирован");
    };
    actions_[52] = [this]() {
        checkAndRun(graph_, [this]() { lab5_runner_.runFundamentalCuts(*graph_); },
                    "Граф не инициализирован");
    };

    actions_[61] = [this]() { lab6_runner_.runHashTableDemo(); };
    actions_[62] = [this]() { lab6_runner_.runRBTreeDemo(); };
    actions_[63] = [this]() { lab6_runner_.runHashTableInteractive(); };
    actions_[64] = [this]() { lab6_runner_.runRBTreeInteractive(); };
}

void Menu::show() const {
    std::cout << "\n=== Единое меню лабораторных работ ===\n";

    std::cout << "\n[Общее]\n";
    std::cout << "1 - Сгенерировать граф (ациклический)\n";
    std::cout << "4 - Сгенерировать граф (Райс)\n";
    std::cout << "2 - Визуализировать матрицу смежности\n";
    std::cout << "3 - Визуализировать матрицу весов\n";

    std::cout << "\n[Lab 1 - Метод Шимбелла и подсчёт путей]\n";
    std::cout << "11 - Метод Шимбелла\n";
    std::cout << "12 - Подсчёт путей\n";
    std::cout << "13 - Метрики графа\n";

    std::cout << "\n[Lab 2 - Обход и кратчайшие пути]\n";
    std::cout << "21 - BFS-обход\n";
    std::cout << "22 - Путь Дейкстры\n";
    std::cout << "23 - Сравнение алгоритмов\n";

    std::cout << "\n[Lab 3 - Потоки]\n";
    std::cout << "31 - Сгенерировать сеть потоков\n";
    std::cout << "36 - Сгенерировать сеть потоков (Райс)\n";
    std::cout << "37 - fromNetwork\n";
    std::cout << "32 - Поиск максимального потока\n";
    std::cout << "33 - Визуализировать путь минимальной стоимости\n";
    std::cout << "34 - Визуализировать матрицу пропускных способностей\n";
    std::cout << "35 - Визуализировать матрицу стоимостей\n";

    std::cout << "\n[Lab 4 - Остовы и раскраска]\n";
    std::cout << "41 - Число остовных деревьев (матричная теорема Кирхгофа)\n";
    std::cout << "42 - Минимальный остов (алгоритм Борувки)\n";
    std::cout << "43 - Кодирование дерева (код Прюфера)\n";
    std::cout << "44 - Декодирование дерева (код Прюфера)\n";
    std::cout << "46 - Минимальная раскраска графа\n";

    std::cout << "\n[Lab 5 - Эйлеров цикл и разрезы]\n";
    std::cout << "51 - Проверка эйлеровости, модификация и построение цикла\n";
    std::cout << "52 - Фундаментальная система разрезов и симметрическая разность\n";

    // std::cout << "\n[Lab 6 - HashTable and RBTree]\n";
    // std::cout << "61 - HashTable демо\n";
    // std::cout << "62 - RBTree демо\n";
    // std::cout << "63 - HashTable интерактивно\n";
    // std::cout << "64 - RBTree интерактивно\n";

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
