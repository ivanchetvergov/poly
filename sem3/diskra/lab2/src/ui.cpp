#include "ui.h"

#include "bdd.h"
#include "help.h"
#include "zheg.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

ConsoleUi::ConsoleUi(std::vector<std::string> variables,
                     std::vector<int> truthVector,
                     ZhegalkinPolynomial& polynomial,
                     BDDGraph& bdd)
    : variables_(std::move(variables))
    , truthVector_(std::move(truthVector))
    , polynomial_(polynomial)
    , bdd_(bdd) {}

void ConsoleUi::run() {
    showInfo();
    while (true) {
        try {
            auto values = requestValues();
            if (values.empty()) {
                std::cout << "Выход." << std::endl;
                break;
            }
            const int direct = boolean_help::evaluateDirect(truthVector_, values);
            const int viaBdd = bdd_.evaluate(values);
            const int viaPoly = polynomial_.evaluatePolynomial(values);
            std::cout << "f = " << direct
                      << " | БДР = " << viaBdd
                      << " | Жегалкина = " << viaPoly << std::endl;
        } catch (const std::exception& ex) {
            std::cout << "Ошибка: " << ex.what() << std::endl;
        }
    }
}

void ConsoleUi::showInfo() {
    std::cout << "Переменные: ";
    for (size_t i = 0; i < variables_.size(); ++i) {
        if (i) {
            std::cout << ", ";
        }
        std::cout << variables_[i];
    }
    std::cout << '\n';

    std::cout << "Таблица истинности:" << std::endl;
    const size_t rows = truthVector_.size();
    for (const auto& var : variables_) {
        std::cout << std::setw(3) << var;
    }
    std::cout << std::setw(4) << 'f' << std::endl;
    for (size_t idx = 0; idx < rows; ++idx) {
        for (size_t var = 0; var < variables_.size(); ++var) {
            const size_t shift = variables_.size() - 1 - var;
            const bool bit = (idx >> shift) & 1U;
            std::cout << std::setw(3) << bit;
        }
        std::cout << std::setw(4) << truthVector_[idx] << std::endl;
    }
    std::cout << "Вектор: " << boolean_help::truthVectorString(truthVector_) << "\n\n";
    std::cout << "СДНФ: " << boolean_help::buildSDNF(variables_, truthVector_) << "\n\n";
    std::cout << "СКНФ: " << boolean_help::buildSKNF(variables_, truthVector_) << "\n\n";
    std::cout << "Полином Жегалкина: " << polynomial_.buildPolynomial() << "\n\n";
    std::cout << "Треугольник Паскаля для полинома Жегалкина" << std::endl;
    polynomial_.printTriangle();
    std::cout << std::endl << bdd_.describe() << std::endl;
}

std::vector<int> ConsoleUi::requestValues() const {
    std::cout << "Введите значения переменных через пробел (или q для выхода): ";
    std::string line;
    if (!std::getline(std::cin, line)) {
        return {};
    }
    if (line == "q" || line == "Q") {
        return {};
    }
    std::istringstream iss(line);
    std::vector<int> values;
    int bit;
    while (iss >> bit) {
        values.push_back(bit ? 1 : 0);
    }
    if (!values.empty() && values.size() != variables_.size()) {
        throw std::invalid_argument("Нужно ввести " + std::to_string(variables_.size()) + " бит");
    }
    return values;
}
