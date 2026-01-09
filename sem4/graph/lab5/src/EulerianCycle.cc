#include "EulerianCycle.h"

#include <set>
#include <stack>

#include <CollectionUtils.h>

namespace graph {

bool EulerianCycle::isEulerian() const {
    auto odd_vertices = getOddDegreeVertices();
    return odd_vertices.empty();
}

bool EulerianCycle::isSemiEulerian() const {
    auto odd_vertices = getOddDegreeVertices();
    return odd_vertices.size() == 2;
}

std::vector<int> EulerianCycle::getOddDegreeVertices() const {
    std::vector<int> odd_vertices;
    for (int v : m_graph_.vertexIds()) {
        if (m_graph_.degree(v) % 2 != 0) {
            odd_vertices.push_back(v);
        }
    }
    return odd_vertices;
}

void EulerianCycle::makeEulerian() {
    auto odd_vertices = getOddDegreeVertices();

    if (odd_vertices.empty())
        return;

    m_added_edges_.clear();

    for (size_t i = 0; i + 1 < odd_vertices.size(); i += 2) {
        int v1 = odd_vertices[i];
        int v2 = odd_vertices[i + 1];

        if (!m_graph_.hasEdge(v1, v2)) {
            m_graph_.addEdge(v1, v2, 1.0);
            m_added_edges_.emplace_back(v1, v2);
        }
    }
}

std::optional<std::vector<int>> EulerianCycle::findCycle() {
    if (!isEulerian() && !isSemiEulerian()) {
        return std::nullopt;
    }

    auto vertices = m_graph_.vertexIds();
    if (vertices.empty())
        return std::nullopt;

    int start = vertices[0];
    if (isSemiEulerian()) {
        auto odd_vertices = getOddDegreeVertices();
        if (!odd_vertices.empty()) {
            start = odd_vertices[0];
        }
    }

    auto cycle = findEulerianCycle(start);

    if (!cycle.has_value() || cycle->size() < 3) {
        return std::nullopt;
    }

    return cycle;
}

std::optional<std::vector<int>> EulerianCycle::findEulerianCycle(int start) {
    std::unordered_map<int, std::multiset<int>> adj_copy;

    for (int v : m_graph_.vertexIds()) {
        for (auto const& [u, weight] : m_graph_.neighbors(v)) {
            adj_copy[v].insert(u);
        }
    }

    std::stack<int> stack;
    std::vector<int> path;

    stack.push(start);
    int current = start;

    while (!stack.empty()) {
        if (!adj_copy[current].empty()) {
            stack.push(current);
            int next = *adj_copy[current].begin();
            adj_copy[current].erase(adj_copy[current].begin());
            adj_copy[next].erase(adj_copy[next].find(current));
            current = next;
        } else {
            path.push_back(current);
            current = stack.top();
            stack.pop();
        }
    }

    std::reverse(path.begin(), path.end());
    return path;
}

}  // namespace graph
