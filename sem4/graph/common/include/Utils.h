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

inline void printHeader(const std::string& title) {
    std::cout << "╔════════════════════════════════════════════════╗\n";
    std::cout << "║  " << title;
    for (size_t i = title.length(); i < 46; ++i) std::cout << " ";
    std::cout << "║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n";
}

inline void printSequence(const std::vector<int>& seq) {
    if (seq.empty()) {
        std::cout << "Пусто\n";
        return;
    }
    for (size_t i = 0; i < seq.size(); ++i) {
        std::cout << seq[i];
        if (i < seq.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";
}

} // namespace graph
