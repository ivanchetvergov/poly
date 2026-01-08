#pragma once

#include <Graph.h>
#include <memory>
#include <optional>
#include <vector>

namespace graph {

class EulerianCycle {
public:
    explicit EulerianCycle(Graph& graph) : m_graph(graph) {}

    [[nodiscard]] bool isEulerian() const;
    [[nodiscard]] bool isSemiEulerian() const;
    [[nodiscard]] std::vector<int> getOddDegreeVertices() const;

    void makeEulerian();
    [[nodiscard]] std::optional<std::vector<int>> findCycle();

    [[nodiscard]] const std::vector<std::pair<int, int>>& getAddedEdges() const { return m_addedEdges; }

private:
    Graph& m_graph;
    std::vector<std::pair<int, int>> m_addedEdges;

    [[nodiscard]] std::optional<std::vector<int>> findEulerianCycle(int start);
};

} // namespace graph
