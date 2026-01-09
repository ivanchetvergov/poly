#include "HamiltonianCycle.h"

namespace graph {

bool HamiltonianCycle::isHamiltonian() {
    auto vertices = m_graph_.vertexIds();
    if (vertices.empty())
        return false;

    std::vector<int> path = {vertices[0]};
    std::vector<bool> visited(m_graph_.vertexCount(), false);
    visited[0] = true;

    return hasHamiltonianCycle(path, visited, 1);
}

std::optional<std::vector<int>> HamiltonianCycle::findCycle() {
    auto vertices = m_graph_.vertexIds();
    if (vertices.empty())
        return std::nullopt;

    std::vector<int> path = {vertices[0]};
    std::vector<bool> visited(m_graph_.vertexCount(), false);
    visited[0] = true;

    if (hasHamiltonianCycle(path, visited, 1)) {
        path.push_back(path[0]);
        return path;
    }

    return std::nullopt;
}

void HamiltonianCycle::makeHamiltonian() {
    auto vertices = m_graph_.vertexIds();
    int n = static_cast<int>(vertices.size());

    m_added_edges_.clear();

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int v1 = vertices[i];
            int v2 = vertices[j];

            if (m_graph_.degree(v1) < n / 2 || m_graph_.degree(v2) < n / 2) {
                if (!m_graph_.hasEdge(v1, v2)) {
                    m_graph_.addEdge(v1, v2, 1.0);
                    m_added_edges_.emplace_back(v1, v2);
                }
            }
        }
    }
}

bool HamiltonianCycle::hasHamiltonianCycle(std::vector<int>& path, std::vector<bool>& visited,
                                           int const pos) {
    auto const& vertices = m_graph_.vertexIds();
    int const n = static_cast<int>(m_graph_.vertexCount());

    if (pos == n) {
        return m_graph_.hasEdge(path.back(), path[0]);
    }

    for (int i = 0; i < n; ++i) {
        int const v = vertices[i];
        if (!visited[i] && m_graph_.hasEdge(path.back(), v)) {
            path.push_back(v);
            visited[i] = true;

            if (hasHamiltonianCycle(path, visited, pos + 1)) {
                return true;
            }

            path.pop_back();
            visited[i] = false;
        }
    }

    return false;
}

}  // namespace graph
