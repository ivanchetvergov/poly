#pragma once

#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace graph {

inline int readInt(const char* prompt) {
    int value;
    for (;;) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ошибка ввода\n";
    }
}

template<typename T>
bool checkAndRun(const std::unique_ptr<T>& ptr,
    std::function<void()> action, const char* errorMsg)
{
    if (ptr) {
        action();
        return true;
    }
    std::cout << "[FAIL] " << errorMsg << "\n";
    return false;
}

void showMenu() {
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

    std::cout << "\n[Lab 5 - Циклы]\n";
    std::cout << "51 - Проверка эйлеровости\n";
    std::cout << "52 - Проверка гамильтоновости\n";
    std::cout << "53 - Задача коммивояжера (TSP)\n";
    std::cout << "54 - Визуализировать TSP-цикл\n";
    std::cout << "55 - Визуализировать Гамильтонов цикл\n";
    std::cout << "56 - Визуализировать Эйлеров цикл\n";

    std::cout << "\n0 - Выход\n";
    std::cout << "\nВаш выбор: ";
}


} // namespace graph
