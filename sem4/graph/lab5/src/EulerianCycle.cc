#include "EulerianCycle.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>

namespace graph {


bool EulerianCycle::isEulerian() const {
    return getOddDegreeVertices().empty() && hasSingleNonZeroComponent();
}

bool EulerianCycle::isSemiEulerian() const {
    return getOddDegreeVertices().size() == 2 && hasSingleNonZeroComponent();
}

std::vector<int> EulerianCycle::getOddDegreeVertices() const {
    std::vector<int> odd;
    for (int v : m_graph_.vertexIds()) {
        if (m_graph_.degree(v) % 2 != 0) {
            odd.push_back(v);
        }
    }
    return odd;
}

void EulerianCycle::makeEulerian() {
    m_added_edges_.clear();
    logDebug("makeEulerian: start");

    auto comps = nonZeroComponents();
    logDebug("non-zero components: " + std::to_string(comps.size()));
    for (size_t i = 1; i < comps.size(); ++i) {
        int u = comps[i - 1].front();
        int v = comps[i].front();
        bool added = addEdgeTracked(u, v);
        logDebug("connect components edge " + std::to_string(u) + "--" +
                 std::to_string(v) + (added ? " [added]" : " [exists, skipped]"));
    }

    const int MAX_ITERATIONS = 64;
    for (int iteration = 1; iteration <= MAX_ITERATIONS; ++iteration) {
        auto odd = getOddDegreeVertices();
        logDebug("iteration " + std::to_string(iteration) +
                 ", odd vertices: " + oddVerticesToString(odd));
        if (odd.empty()) {
            break;
        }

        if (odd.size() == 2) {
            bool closed = addEdgeTracked(odd[0], odd[1]);
            logDebug("attempt close semi-eulerian with edge " +
                     std::to_string(odd[0]) + "--" + std::to_string(odd[1]) +
                     (closed ? " [added, graph becomes eulerian]"
                             : " [exists, keep semi-eulerian]"));
            break;
        }

        auto pairs = buildPairsWithoutExistingEdges(odd);
        if (pairs.empty()) {
            logDebug("no valid non-existing pairs found; cannot reduce odd vertices further");
            break;
        }

        int added = addTrackedPairs(pairs);
        logDebug("pairs selected: " + std::to_string(pairs.size()) +
                 ", edges actually added: " + std::to_string(added));

        if (added == 0) {
            logDebug("no edges added on this iteration; stop to avoid infinite loop");
            break;
        }
    }

    auto finalOdd = getOddDegreeVertices();
    logDebug("makeEulerian: end, odd vertices: " + oddVerticesToString(finalOdd));
}

std::optional<std::vector<int>> EulerianCycle::findCycle() {
    if (!isEulerian() && !isSemiEulerian()) {
        return std::nullopt;
    }

    auto vertices = m_graph_.vertexIds();
    if (vertices.empty()) return std::nullopt;

    std::vector<int> candidates;
    if (isSemiEulerian()) {
        candidates = getOddDegreeVertices();
    } else {
        for (int v : vertices) {
            if (m_graph_.degree(v) > 0) {
                candidates.push_back(v);
                break;
            }
        }
    }
    if (candidates.empty()) candidates = vertices;

    const int totalEdges = static_cast<int>(m_graph_.edgeCount());

    for (int s : candidates) {
        auto result = hierholzer(s, totalEdges);
        if (result.has_value()) return result;
    }

    return std::nullopt;
}


void EulerianCycle::logDebug(std::string const& message) const {
    std::cout << "[EULER][DEBUG] " << message << "\n";
}

std::string EulerianCycle::oddVerticesToString(std::vector<int> const& odd) const {
    std::ostringstream out;
    out << "count=" << odd.size() << " [";
    for (size_t i = 0; i < odd.size(); ++i) {
        out << odd[i];
        if (i + 1 < odd.size()) out << ", ";
    }
    out << "]";
    return out.str();
}

std::vector<std::pair<int, int>> EulerianCycle::buildPairsWithoutExistingEdges(
    std::vector<int> const& odd) const {
    std::vector<std::pair<int, int>> pairs;
    std::vector<bool> used(odd.size(), false);

    for (size_t i = 0; i < odd.size(); ++i) {
        if (used[i]) continue;

        int bestJ = -1;
        int bestScore = -1;
        for (size_t j = i + 1; j < odd.size(); ++j) {
            if (used[j]) continue;
            if (m_graph_.hasEdge(odd[i], odd[j])) continue;

            int score = 0;
            for (size_t k = i + 1; k < odd.size(); ++k) {
                if (k == j || used[k]) continue;
                if (!m_graph_.hasEdge(odd[i], odd[k])) ++score;
                if (!m_graph_.hasEdge(odd[j], odd[k])) ++score;
            }
            if (score > bestScore) {
                bestScore = score;
                bestJ = static_cast<int>(j);
            }
        }

        if (bestJ >= 0) {
            used[i] = true;
            used[bestJ] = true;
            pairs.emplace_back(odd[i], odd[bestJ]);
        }
    }

    return pairs;
}

int EulerianCycle::addTrackedPairs(std::vector<std::pair<int, int>> const& pairs) {
    int added = 0;
    for (auto const& [u, v] : pairs) {
        bool ok = addEdgeTracked(u, v);
        logDebug("add edge " + std::to_string(u) + "--" + std::to_string(v) +
                 (ok ? " [added]" : " [exists, skipped]"));
        if (ok) ++added;
    }
    return added;
}

bool EulerianCycle::addEdgeTracked(int u, int v) {
    bool added = m_graph_.addEdge(u, v, 1.0);
    if (added) {
        m_added_edges_.emplace_back(std::min(u, v), std::max(u, v));
    }
    return added;
}

std::optional<std::vector<int>> EulerianCycle::hierholzer(int start, int totalEdges) const {
    auto vertices = m_graph_.vertexIds();
    if (vertices.empty()) return std::nullopt;

    std::unordered_map<int,int> id2idx;
    id2idx.reserve(vertices.size());
    for (int i = 0; i < static_cast<int>(vertices.size()); ++i) {
        id2idx[vertices[i]] = i;
    }

    if (!id2idx.count(start)) return std::nullopt;

    int n = static_cast<int>(vertices.size());
    auto edges = m_graph_.edges();
    int m = static_cast<int>(edges.size());

    std::vector<std::vector<std::pair<int,int>>> g(n);
    for (int eid = 0; eid < m; ++eid) {
        const auto& e = edges[eid];
        auto itU = id2idx.find(e.from);
        auto itV = id2idx.find(e.to);
        if (itU == id2idx.end() || itV == id2idx.end()) continue;
        int ui = itU->second;
        int vi = itV->second;
        g[ui].emplace_back(vi, eid);
        if (!m_graph_.isDirected()) {
            g[vi].emplace_back(ui, eid);
        }
    }

    std::vector<int>  ptr(n, 0);
    std::vector<bool> usedEdge(m, false);

    std::stack<int>  stk;
    std::vector<int> circuit;
    circuit.reserve(totalEdges + 1);

    stk.push(id2idx[start]);

    while (!stk.empty()) {
        int u = stk.top();
        bool found = false;

        while (ptr[u] < static_cast<int>(g[u].size())) {
            auto [v, eid] = g[u][ptr[u]++];
            if (usedEdge[eid]) continue;
            usedEdge[eid] = true;
            stk.push(v);
            found = true;
            break;
        }

        if (!found) {
            circuit.push_back(vertices[u]);
            stk.pop();
        }
    }

    std::reverse(circuit.begin(), circuit.end());

    if (static_cast<int>(circuit.size()) != totalEdges + 1) {
        return std::nullopt;
    }

    if (isEulerian() && circuit.front() != circuit.back()) {
        return std::nullopt;
    }

    return circuit;
}

std::vector<std::vector<int>> EulerianCycle::nonZeroComponents() const {
    std::vector<std::vector<int>> comps;
    std::set<int> visited;

    for (int v : m_graph_.vertexIds()) {
        if (m_graph_.degree(v) == 0 || visited.count(v)) continue;

        std::vector<int> comp;
        std::queue<int> q;
        q.push(v);
        visited.insert(v);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            comp.push_back(u);
            for (auto const& [to, _] : m_graph_.neighbors(u)) {
                if (!visited.count(to)) {
                    visited.insert(to);
                    q.push(to);
                }
            }
        }

        comps.push_back(std::move(comp));
    }

    return comps;
}

bool EulerianCycle::hasSingleNonZeroComponent() const {
    return nonZeroComponents().size() <= 1;
}

}  // namespace graph
