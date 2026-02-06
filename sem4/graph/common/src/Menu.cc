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
    actions_[2] = [this]() { lab1_runner_.runVisualizeAdjacencyMatrix(*graph_); };
    actions_[3] = [this]() { lab1_runner_.runVisualizeWeightMatrix(*graph_); };

    actions_[11] = [this]() { lab1_runner_.runShimbellMethod(*graph_); };
    actions_[12] = [this]() { lab1_runner_.runPathsMethod(*graph_); };

    actions_[31] = [this]() { lab3_runner_.runGenerateFlowNetwork(); };
    actions_[32] = [this]() { lab3_runner_.runMaxFlow(); };
    actions_[33] = [this]() { lab3_runner_.runMinCostFlow(); };
    actions_[34] = [this]() { lab3_runner_.runVisualizeCapacityMatrix(); };
    actions_[35] = [this]() { lab3_runner_.runVisualizeCostMatrix(); };

    actions_[41] = [this]() { lab4_runner_.runCountSpanningTrees(*graph_); };
    actions_[42] = [this]() { lab4_runner_.runFindMaxIndependentSet(*graph_); };
    actions_[43] = [this]() { lab4_runner_.runFindMaxIndependentSetEdges(*graph_); };
    actions_[44] = [this]() { lab4_runner_.runFindMinVertexCover(*graph_); };
    actions_[45] = [this]() { lab4_runner_.runFindMinEdgeCover(*graph_); };
    actions_[46] = [this]() { lab4_runner_.runFindMinColoring(*graph_); };

    actions_[51] = [this]() { lab5_runner_.runCheckEulerian(*graph_); };
    actions_[52] = [this]() { lab5_runner_.runCheckHamiltonian(*graph_); };
    actions_[53] = [this]() { lab5_runner_.runSolveTSP(*graph_); };

    actions_[61] = [this]() { lab6_runner_.runHashTableDemo(); };
    actions_[62] = [this]() { lab6_runner_.runRBTreeDemo(); };
    actions_[63] = [this]() { lab6_runner_.runHashTableInteractive(); };
    actions_[64] = [this]() { lab6_runner_.runRBTreeInteractive(); };
}

void Menu::show() const {
    std::cout << "\n=== Единое меню лабораторных работ ===\n";

    std::cout << "\n[Общее]\n";
    std::cout << "1 - Сгенерировать граф\n";
    std::cout << "2 - Визуализировать матрицу смежности\n";
    std::cout << "3 - Визуализировать матрицу весов\n";

    std::cout << "\n[Lab 1 - Метод Шимбелла и подсчёт путей]\n";
    std::cout << "11 - Метод Шимбелла\n";
    std::cout << "12 - Подсчёт путей\n";

    std::cout << "\n[Lab 3 - Потоки]\n";
    std::cout << "31 - Сгенерировать сеть потоков\n";
    std::cout << "32 - Поиск максимального потока\n";
    std::cout << "33 - Визуализировать путь минимальной стоимости\n";
    std::cout << "34 - Визуализировать матрицу пропускных способностей\n";
    std::cout << "35 - Визуализировать матрицу стоимостей\n";

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

    std::cout << "\n[Lab 6 - HashTable and RBTree]\n";
    std::cout << "61 - HashTable демо\n";
    std::cout << "62 - RBTree демо\n";
    std::cout << "63 - HashTable интерактивно\n";
    std::cout << "64 - RBTree интерактивно\n";

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
