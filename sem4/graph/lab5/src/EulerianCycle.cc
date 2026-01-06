#include "EulerianCycle.h"
#include <CollectionUtils.h>
#include <algorithm>
#include <set>
#include <stack>
#include <unordered_set>

namespace graph {

bool EulerianCycle::isEulerian() const {
    auto oddVertices = getOddDegreeVertices();
    return oddVertices.empty();
}

bool EulerianCycle::isSemiEulerian() const {
    auto oddVertices = getOddDegreeVertices();
    return oddVertices.size() == 2;
}

std::vector<int> EulerianCycle::getOddDegreeVertices() const {
    std::vector<int> oddVertices;
    for (int v : m_graph.vertexIds()) {
        if (m_graph.degree(v) % 2 != 0) {
            oddVertices.push_back(v);
        }
    }
    return oddVertices;
}

void EulerianCycle::makeEulerian() {
    auto oddVertices = getOddDegreeVertices();

    if (oddVertices.empty()) return;

    for (size_t i = 0; i + 1 < oddVertices.size(); i += 2) {
        int v1 = oddVertices[i];
        int v2 = oddVertices[i + 1];

        if (!m_graph.hasEdge(v1, v2)) {
            m_graph.addEdge(v1, v2, 1.0);
        }
    }
}

std::optional<std::vector<int>> EulerianCycle::findCycle() {
    if (!isEulerian() && !isSemiEulerian()) {
        return std::nullopt;
    }

    auto vertices = m_graph.vertexIds();
    if (vertices.empty()) return std::nullopt;

    int start = vertices[0];
    if (isSemiEulerian()) {
        auto oddVertices = getOddDegreeVertices();
        if (!oddVertices.empty()) {
            start = oddVertices[0];
        }
    }

    return findEulerianCycle(start);
}

std::optional<std::vector<int>> EulerianCycle::findEulerianCycle(int start) {
    std::unordered_map<int, std::multiset<int>> adjCopy;

    for (int v : m_graph.vertexIds()) {
        for (const auto& [u, weight] : m_graph.neighbors(v)) {
            adjCopy[v].insert(u);
        }
    }

    std::stack<int> stack;
    std::vector<int> path;

    stack.push(start);
    int current = start;

    while (!stack.empty()) {
        if (!adjCopy[current].empty()) {
            stack.push(current);
            int next = *adjCopy[current].begin();
            adjCopy[current].erase(adjCopy[current].begin());
            adjCopy[next].erase(adjCopy[next].find(current));
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

} // namespace graph
