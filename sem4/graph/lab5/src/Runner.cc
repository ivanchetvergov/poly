#include "../include/Runner.h"
#include "../include/EulerianCycle.h"
#include "../include/HamiltonianCycle.h"
#include "../include/TSPSolver.h"
#include <Generator.h>
#include <iostream>

namespace lab5 {


void Runner::checkEulerian() {
    if (!graph_) {
        throw std::runtime_error("Graph not set");
    }

    EulerianCycle euler(*graph_);

    std::cout << "\n=== Проверка эйлеровости ===\n";

    if (euler.isEulerian()) {
        std::cout << "[OK] Граф является эйлеровым\n";
        auto cycle = euler.findCycle();
        if (cycle.has_value()) {
            lastEulerianCycle_ = cycle.value();
            std::cout << "Эйлеров цикл найден, длина: " << lastEulerianCycle_->size() << "\n";
        }
    } else if (euler.isSemiEulerian()) {
        std::cout << "[INFO] Граф является полуэйлеровым (есть эйлеров путь, но нет цикла)\n";
        auto oddVertices = euler.getOddDegreeVertices();
        std::cout << "Вершины с нечётной степенью: ";
        for (int v : oddVertices) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "[INFO] Граф не является эйлеровым\n";
        auto oddVertices = euler.getOddDegreeVertices();
        std::cout << "Вершин с нечётной степенью: " << oddVertices.size() << "\n";

        std::cout << "\nПопытка модификации графа...\n";
        euler.makeEulerian();
        lastEulerianAddedEdges_ = euler.getAddedEdges();
        if (!lastEulerianAddedEdges_.empty()) {
            std::cout << "[ИНФО] Добавлено рёбер: " << lastEulerianAddedEdges_.size() << "\n";
        }
        auto cycle = euler.findCycle();
        if (cycle.has_value()) {
            lastEulerianCycle_ = cycle.value();
            std::cout << "[OK] После модификации найден эйлеров цикл, длина: " << lastEulerianCycle_->size() << "\n";
        } else {
            std::cout << "[WARN] Не удалось найти эйлеров цикл после модификации\n";
        }
    }
}

void Runner::checkHamiltonian() {
    if (!graph_) {
        throw std::runtime_error("Graph not set");
    }

    HamiltonianCycle hamilton(*graph_);

    std::cout << "\n=== Проверка гамильтоновости ===\n";

    if (hamilton.isHamiltonian()) {
        std::cout << "[OK] Граф является гамильтоновым\n";
        auto cycle = hamilton.findCycle();
        if (cycle.has_value()) {
            lastHamiltonianCycle_ = cycle.value();
            std::cout << "Гамильтонов цикл найден\n";
        }
    } else {
        std::cout << "[INFO] Граф не является гамильтоновым\n";
        std::cout << "Попытка модификации графа...\n";
        hamilton.makeHamiltonian();
        lastHamiltonianAddedEdges_ = hamilton.getAddedEdges();
        if (!lastHamiltonianAddedEdges_.empty()) {
            std::cout << "[ИНФО] Добавлено рёбер: " << lastHamiltonianAddedEdges_.size() << "\n";
        }
        auto cycle = hamilton.findCycle();
        if (cycle.has_value()) {
            lastHamiltonianCycle_ = cycle.value();
            std::cout << "[OK] После модификации найден гамильтонов цикл\n";
        } else {
            std::cout << "[WARN] Не удалось найти гамильтонов цикл после модификации\n";
        }
    }
}

void Runner::solveTSP() {
    if (!graph_) {
        throw std::runtime_error("Graph not set");
    }

    std::cout << "\n=== Задача коммивояжёра (TSP) ===\n";

    TSPSolver solver(*graph_);
    auto result = solver.findAllCycles(100);

    if (result.empty()) {
        std::cout << "[INFO] Не найдено гамильтоновых циклов\n";
        return;
    }

    std::cout << "[OK] Найдено циклов: " << result.size() << "\n\n";

    size_t showCount = std::min(result.size(), size_t(5));
    std::cout << "Лучшие " << showCount << " циклов:\n";

    for (size_t i = 0; i < showCount; ++i) {
        const auto& cycle = result[i];
        std::cout << (i + 1) << ". Стоимость: " << cycle.cost << ", Путь: ";
        for (size_t j = 0; j < cycle.path.size(); ++j) {
            std::cout << cycle.path[j];
            if (j < cycle.path.size() - 1) std::cout << " -> ";
        }
        std::cout << "\n";
    }

    lastTSPCycle_ = result.front().path;
}

const std::optional<std::vector<int>>& Runner::getLastEulerianCycle() const {
    return lastEulerianCycle_;
}

const std::optional<std::vector<int>>& Runner::getLastHamiltonianCycle() const {
    return lastHamiltonianCycle_;
}

const std::optional<std::vector<int>>& Runner::getLastTSPCycle() const {
    return lastTSPCycle_;
}

} // namespace lab5
