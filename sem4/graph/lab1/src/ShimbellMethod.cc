// ShimbellMethod.cc
#include "ShimbellMethod.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

namespace graph {

ShimbellMethod::ShimbellMethod(const Graph& graph)
    : m_graph(graph)
    , m_vertexIds(graph.vertexIds())
    , m_size(static_cast<int>(m_vertexIds.size())) {
}

ShimbellResult ShimbellMethod::compute(int pathLength) {
    if (pathLength <= 0) {
        throw std::invalid_argument("Длина пути должна быть положительной");
    }

    DistanceMatrix currentMin = createAdjacencyMatrix();
    DistanceMatrix currentMax = createAdjacencyMatrix();

    if (pathLength == 1) {
        return {currentMin, currentMax, 1};
    }

    DistanceMatrix baseMin = currentMin;
    DistanceMatrix baseMax = currentMax;

    for (int step = 2; step <= pathLength; ++step) {
        currentMin = multiplyMin(currentMin, baseMin);
        currentMax = multiplyMax(currentMax, baseMax);
    }

    return {currentMin, currentMax, pathLength};
}

DistanceMatrix ShimbellMethod::createAdjacencyMatrix() const {
    DistanceMatrix matrix(m_size, std::vector<std::optional<double>>(m_size, std::nullopt));

    for (int i = 0; i < m_size; ++i) {
        matrix[i][i] = 0.0;
    }

    for (const auto& edge : m_graph.edges()) {
        int fromIdx = getIndex(edge.from);
        int toIdx = getIndex(edge.to);
        
        matrix[fromIdx][toIdx] = edge.weight;
        matrix[toIdx][fromIdx] = edge.weight;  
    }

    return matrix;
}

DistanceMatrix ShimbellMethod::multiplyMin(
    const DistanceMatrix& a, 
    const DistanceMatrix& b
) const {
    DistanceMatrix result(m_size, std::vector<std::optional<double>>(m_size, std::nullopt));

    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            for (int k = 0; k < m_size; ++k) {
                if (a[i][k].has_value() && b[k][j].has_value()) {
                    double distance = a[i][k].value() + b[k][j].value();
                    
                    if (!result[i][j].has_value() || distance < result[i][j].value()) {
                        result[i][j] = distance;
                    }
                }
            }
        }
    }

    return result;
}

DistanceMatrix ShimbellMethod::multiplyMax(
    const DistanceMatrix& a, 
    const DistanceMatrix& b
) const {
    DistanceMatrix result(m_size, std::vector<std::optional<double>>(m_size, std::nullopt));

    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            for (int k = 0; k < m_size; ++k) {
                if (a[i][k].has_value() && b[k][j].has_value()) {
                    double distance = a[i][k].value() + b[k][j].value();
                    
                    if (!result[i][j].has_value() || distance > result[i][j].value()) {
                        result[i][j] = distance;
                    }
                }
            }
        }
    }

    return result;
}

int ShimbellMethod::getIndex(int vertexId) const {
    auto it = std::find(m_vertexIds.begin(), m_vertexIds.end(), vertexId);
    if (it == m_vertexIds.end()) {
        throw std::runtime_error("Вершина не найдена в графе");
    }
    return static_cast<int>(std::distance(m_vertexIds.begin(), it));
}

void ShimbellMethod::printMatrix(const DistanceMatrix& matrix, const char* title) {
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
                std::cout << std::setw(10) << std::fixed << std::setprecision(2) 
                          << matrix[i][j].value();
            } else {
                std::cout << std::setw(10) << "-";
            }
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

} // namespace graph
