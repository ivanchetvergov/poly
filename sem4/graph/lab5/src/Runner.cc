#include "../include/Runner.h"

#include "../include/EulerianCycle.h"
#include "../include/HamiltonianCycle.h"
#include "../include/TSPSolver.h"

#include <iostream>

#include <Generator.h>

namespace lab5 {

using graph::EulerianCycle;
using graph::HamiltonianCycle;
using graph::TSPSolver;

void Runner::checkEulerian(Graph& graph) {
    EulerianCycle euler(graph);

    std::cout << "\n=== Проверка эйлеровости ===\n";

    if (euler.isEulerian()) {
        std::cout << "[OK] Граф является эйлеровым\n";
        auto cycle = euler.findCycle();
        if (cycle.has_value()) {
            last_eulerian_cycle_ = cycle.value();
            std::cout << "Эйлеров цикл найден, длина: " << last_eulerian_cycle_->size() << "\n";
        }
    } else if (euler.isSemiEulerian()) {
        std::cout << "[INFO] Граф является полуэйлеровым (есть эйлеров путь, но нет цикла)\n";
        auto odd_vertices = euler.getOddDegreeVertices();
        std::cout << "Вершины с нечётной степенью: ";
        for (int v : odd_vertices) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "[INFO] Граф не является эйлеровым\n";
        auto odd_vertices = euler.getOddDegreeVertices();
        std::cout << "Вершин с нечётной степенью: " << odd_vertices.size() << "\n";

        std::cout << "\nПопытка модификации графа...\n";
        euler.makeEulerian();
        last_eulerian_added_edges_ = euler.getAddedEdges();
        if (!last_eulerian_added_edges_.empty()) {
            std::cout << "[ИНФО] Добавлено рёбер: " << last_eulerian_added_edges_.size() << "\n";
        }
        auto cycle = euler.findCycle();
        if (cycle.has_value()) {
            last_eulerian_cycle_ = cycle.value();
            std::cout << "[OK] После модификации найден эйлеров цикл, длина: "
                      << last_eulerian_cycle_->size() << "\n";
        } else {
            std::cout << "[WARN] Не удалось найти эйлеров цикл после модификации\n";
        }
    }
}

void Runner::checkHamiltonian(Graph& graph) {
    HamiltonianCycle hamilton(graph);

    std::cout << "\n=== Проверка гамильтоновости ===\n";

    if (hamilton.isHamiltonian()) {
        std::cout << "[OK] Граф является гамильтоновым\n";
        auto cycle = hamilton.findCycle();
        if (cycle.has_value()) {
            last_hamiltonian_cycle_ = cycle.value();
            std::cout << "Гамильтонов цикл найден\n";
        }
    } else {
        std::cout << "[INFO] Граф не является гамильтоновым\n";
        std::cout << "Попытка модификации графа...\n";
        hamilton.makeHamiltonian();
        last_hamiltonian_added_edges_ = hamilton.getAddedEdges();
        if (!last_hamiltonian_added_edges_.empty()) {
            std::cout << "[ИНФО] Добавлено рёбер: " << last_hamiltonian_added_edges_.size() << "\n";
        }
        auto cycle = hamilton.findCycle();
        if (cycle.has_value()) {
            last_hamiltonian_cycle_ = cycle.value();
            std::cout << "[OK] После модификации найден гамильтонов цикл\n";
        } else {
            std::cout << "[WARN] Не удалось найти гамильтонов цикл после модификации\n";
        }
    }
}

void Runner::solveTSP(Graph const& graph) {
    std::cout << "\n=== Задача коммивояжёра (TSP) ===\n";

    TSPSolver solver(graph);
    auto result = solver.findAllCycles(100);

    if (result.empty()) {
        std::cout << "[INFO] Не найдено гамильтоновых циклов\n";
        return;
    }

    std::cout << "[OK] Найдено циклов: " << result.size() << "\n\n";

    size_t show_count = std::min(result.size(), static_cast<size_t>(5));
    std::cout << "Лучшие " << show_count << " циклов:\n";

    for (size_t i = 0; i < show_count; ++i) {
        auto const& cycle = result[i];
        std::cout << (i + 1) << ". Стоимость: " << cycle.cost << ", Путь: ";
        for (size_t j = 0; j < cycle.path.size(); ++j) {
            std::cout << cycle.path[j];
            if (j < cycle.path.size() - 1)
                std::cout << " -> ";
        }
        std::cout << "\n";
    }

    last_tsp_cycle_ = result.front().path;
}

std::optional<std::vector<int>> const& Runner::getLastEulerianCycle() const {
    return last_eulerian_cycle_;
}

std::optional<std::vector<int>> const& Runner::getLastHamiltonianCycle() const {
    return last_hamiltonian_cycle_;
}

std::optional<std::vector<int>> const& Runner::getLastTSPCycle() const {
    return last_tsp_cycle_;
}

}  // namespace lab5
