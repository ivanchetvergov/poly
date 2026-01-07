// ShimbellMethod.cc
#include "ShimbellMethod.h"

#include <CollectionUtils.h>
#include <algorithm>

namespace graph {

ShimbellMethod::ShimbellMethod(const Graph& graph)
    : m_graph(graph)
    , m_vertexIds(graph.vertexIds())
    , m_size(static_cast<int>(m_vertexIds.size())) {
}

const ShimbellResult& ShimbellMethod::compute(int pathLength) {
    if (pathLength <= 0) {
        throw std::invalid_argument("Длина пути должна быть положительной");
    }

    DistanceMatrix currentMin = createAdjacencyMatrix();
    DistanceMatrix currentMax = createAdjacencyMatrix();

    if (pathLength == 1) {
        result_ = ShimbellResult{currentMin, currentMax, 1};
        return result_;
    }

    DistanceMatrix baseMin = currentMin;
    DistanceMatrix baseMax = currentMax;

    for (int step = 2; step <= pathLength; ++step) {
        currentMin = multiplyMin(currentMin, baseMin);
        currentMax = multiplyMax(currentMax, baseMax);
    }

    result_ = ShimbellResult{currentMin, currentMax, pathLength};
    return result_;
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
    return CollectionUtils::multiplyOptionalMatrix(a, b, std::less<double>());
}

DistanceMatrix ShimbellMethod::multiplyMax(
    const DistanceMatrix& a,
    const DistanceMatrix& b
) const {
    return CollectionUtils::multiplyOptionalMatrix(a, b, std::greater<double>());
}

int ShimbellMethod::getIndex(int vertexId) const {
    auto it = std::find(m_vertexIds.begin(), m_vertexIds.end(), vertexId);
    if (it == m_vertexIds.end()) {
        throw std::runtime_error("Вершина не найдена в графе");
    }
    return static_cast<int>(std::distance(m_vertexIds.begin(), it));
}


} // namespace graph
