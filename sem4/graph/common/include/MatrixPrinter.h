#pragma once

#include <iomanip>
#include <iostream>
#include <optional>
#include <vector>

namespace graph {

template<typename T>
using SparseMatrix = std::vector<std::vector<std::optional<T>>>;

class MatrixPrinter {
public:
    template<typename T>
    static void printOptionalMatrix(
        const SparseMatrix<T>& matrix,
        const char* title,
        int precision = 2)
    {
        std::cout << "\n" << title << ":\n";

        if (matrix.empty()) {
            std::cout << "Матрица пуста\n";
            return;
        }

        int size = static_cast<int>(matrix.size());

        std::cout << std::setw(8) << " ";
        for (int j = 0; j < size; ++j) {
            std::cout << std::setw(10) << j;
        }
        std::cout << "\n";

        for (int i = 0; i < size; ++i) {
            std::cout << std::setw(8) << i;
            for (int j = 0; j < size; ++j) {
                if (matrix[i][j].has_value()) {
                    std::cout << std::setw(10) << std::fixed
                              << std::setprecision(precision)
                              << matrix[i][j].value();
                } else {
                    std::cout << std::setw(10) << "-";
                }
            }
            std::cout << "\n";
        }
        std::cout << std::endl;
    }

    template<typename T, typename GetValueFunc>
    static void printMatrix(
        const char* title,
        const std::vector<int>& indices,
        GetValueFunc getValue,
        int precision = 2)
    {
        std::cout << "\n" << title << ":\n" << std::setw(6) << " ";
        for (int v : indices) {
            std::cout << std::setw(8) << v;
        }
        std::cout << "\n";

        for (int from : indices) {
            std::cout << std::setw(6) << from;
            for (int to : indices) {
                T value = getValue(from, to);
                if (value > 0) {
                    std::cout << std::setw(8) << std::fixed
                              << std::setprecision(precision) << value;
                } else {
                    std::cout << std::setw(8) << "-";
                }
            }
            std::cout << "\n";
        }
    }
};

} // namespace graph
