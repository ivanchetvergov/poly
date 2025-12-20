#include "zheg.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {

std::string join(const std::vector<std::string>& parts) {
    if (parts.empty()) {
        return "";
    }
    std::ostringstream oss;
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i) {
            oss << " + ";
        }
        oss << parts[i];
    }
    return oss.str();
}

std::vector<std::string> defaultNames(int count) {
    std::vector<std::string> names(count);
    for (int i = 0; i < count; ++i) {
        names[i] = "x" + std::to_string(i + 1);
    }
    return names;
}

}

ZhegalkinPolynomial::ZhegalkinPolynomial(int numVars)
    : numVars_(numVars)
    , truthTable_(static_cast<size_t>(1) << numVars, 0)
    , triangle_(static_cast<size_t>(1) << numVars)
    , variableNames_(defaultNames(numVars)) {
    const int size = static_cast<int>(truthTable_.size());
    for (int row = 0; row < size; ++row) {
        triangle_[row].assign(size - row, 0);
    }
    coefficients_.assign(size, 0);
}

void ZhegalkinPolynomial::setTruthTableFromVector(const std::vector<int>& values) {
    if (values.size() != truthTable_.size()) {
        throw std::invalid_argument("Некорректный размер таблицы истинности");
    }
    truthTable_ = values;
    triangleReady_ = false;
    polynomialReady_ = false;
}

void ZhegalkinPolynomial::setVariableNames(std::vector<std::string> names) {
    if (!names.empty() && static_cast<int>(names.size()) == numVars_) {
        variableNames_ = std::move(names);
    }
}

void ZhegalkinPolynomial::buildTriangle() {
    const int size = static_cast<int>(truthTable_.size());
    // инициализация первого ряда
    for (int i = 0; i < size; ++i) {
        triangle_[0][i] = truthTable_[i];
    }
    // построение треугольника
    for (int row = 1; row < size; ++row) {
        for (int col = 0; col < size - row; ++col) {
            // формула: a[i][j] = a[i-1][j] XOR a[i-1][j+1]
            triangle_[row][col] = triangle_[row - 1][col] ^ triangle_[row - 1][col + 1];
        }
    }
    // извлечение коэффициентов (первый симв строки)
    for (int row = 0; row < size; ++row) {
        coefficients_[row] = triangle_[row][0];
    }
    triangleReady_ = true;
    polynomialReady_ = false;
}

void ZhegalkinPolynomial::printTriangle() const {
    if (!triangleReady_) {
        std::cout << "Треугольник Жегалкина ещё не построен." << std::endl;
        return;
    }
    const int size = static_cast<int>(truthTable_.size());
    for (int row = 0; row < size; ++row) {
        std::cout << std::setw(2) << row << ": ";
        for (int col = 0; col < size - row; ++col) {
            std::cout << triangle_[row][col] << ' ';
        }
        std::cout << std::endl;
    }
}

std::string ZhegalkinPolynomial::buildPolynomial() {
    ensureTriangle();
    terms_.clear();
    const int size = static_cast<int>(truthTable_.size());
    // цикл по всем возможным комбинациям пар-ов
    for (int index = 0; index < size; ++index) {
        // если 1 то это наш терм
        if (coefficients_[index] == 1) {
            terms_.push_back(generateTerm(index));
        }
    }
    // пустой полином
    if (terms_.empty()) {
        cachedPolynomial_ = "0";
    } else {
        for (auto& term : terms_) {
            if (term.empty()) {
                term = "1";
            }
        }
        // строим итоговый полином
        cachedPolynomial_ = join(terms_);
    }
    polynomialReady_ = true;
    return cachedPolynomial_;
}

int ZhegalkinPolynomial::evaluatePolynomial(const std::vector<int>& values) {
    if (static_cast<int>(values.size()) != numVars_) {
        throw std::invalid_argument("Некорректное число переменных");
    }
    ensureTriangle();
    int result = 0;
    const int size = static_cast<int>(coefficients_.size());
    for (int mask = 0; mask < size; ++mask) {
        if (coefficients_[mask]) {
            // если коэффициент равен 1 учитываем значение терма
            result ^= termValue(mask, values);
        }
    }
    return result;
}

std::string ZhegalkinPolynomial::generateTerm(int index) const {
    if (index == 0) {
        return "";
    }
    std::string term;
    for (int bit = 0; bit < numVars_; ++bit) {
        const int shift = numVars_ - 1 - bit;
        if ((index >> shift) & 1) {
            term += variableNames_[bit];
        }
    }
    return term;
}

void ZhegalkinPolynomial::ensureTriangle() {
    if (!triangleReady_) {
        buildTriangle();
    }
}

int ZhegalkinPolynomial::termValue(int mask, const std::vector<int>& values) const {
    if (mask == 0) {
        return 1;
    }
    int result = 1;
    for (int bit = 0; bit < numVars_; ++bit) {
        const int shift = numVars_ - 1 - bit;
        // если в маске стоит 1, то учитываем значение переменной
        if ((mask >> shift) & 1) {
            // result =  
            result &= values[bit];
        }
    }
    return result;
}
