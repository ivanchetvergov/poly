// ShimbellMethod.cc
#include "ShimbellMethod.h"

#include <algorithm>

#include <CollectionUtils.h>

namespace graph {

ShimbellMethod::ShimbellMethod(Graph const& graph)
    : m_graph_(graph),
      m_vertex_ids_(graph.vertexIds()),
      m_size_(static_cast<int>(m_vertex_ids_.size())) {}

ShimbellResult const& ShimbellMethod::compute(int pathLength) {
    if (pathLength < 0) {
        throw std::invalid_argument("Длина пути не может быть отрицательной");
    }

    if (pathLength == 0) {

        DistanceMatrix identity(m_size_,
                                std::vector<std::optional<double>>(m_size_, std::nullopt));
        for (int i = 0; i < m_size_; ++i) identity[i][i] = 1;
        result_ = ShimbellResult{identity, identity, 0};
        return result_;
    }

    DistanceMatrix current_min = createAdjacencyMatrix();
    DistanceMatrix current_max = createAdjacencyMatrix();

    DistanceMatrix base_min = current_min;
    DistanceMatrix base_max = current_max;

    for (int step = 2; step <= pathLength; ++step) {
        current_min = multiplyMin(current_min, base_min);
        current_max = multiplyMax(current_max, base_max);
    }

    result_ = ShimbellResult{
        .min_distances = current_min,
        .max_distances = current_max,
        .path_length = pathLength
    };
    return result_;
}

DistanceMatrix ShimbellMethod::createAdjacencyMatrix() const
{
    return CollectionUtils::makeSquareMatrix<std::optional<double>>(
        m_vertex_ids_,
        [&](int u, int v) -> std::optional<double> {
            if (u == v) {
                return 0.0;
            }
            auto w = m_graph_.getEdgeWeight(u, v);
            if (w.has_value()) {
                return w;
            }
            return std::nullopt;
        }
    );
}

DistanceMatrix ShimbellMethod::multiplyMin(DistanceMatrix const& a, DistanceMatrix const& b) const {
    return CollectionUtils::multiplyOptionalMatrix(a, b, std::less<>());
}

DistanceMatrix ShimbellMethod::multiplyMax(DistanceMatrix const& a, DistanceMatrix const& b) const {
    return CollectionUtils::multiplyOptionalMatrix(a, b, std::greater<>());
}

int ShimbellMethod::getIndex(int vertexId) const {
    auto it = std::find(m_vertex_ids_.begin(), m_vertex_ids_.end(), vertexId);
    if (it == m_vertex_ids_.end()) {
        throw std::runtime_error("Вершина не найдена в графе");
    }
    return static_cast<int>(std::distance(m_vertex_ids_.begin(), it));
}

}  // namespace graph
