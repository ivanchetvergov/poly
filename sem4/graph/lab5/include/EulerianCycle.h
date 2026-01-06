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

private:
    Graph& m_graph;

    [[nodiscard]] std::optional<std::vector<int>> findEulerianCycle(int start);
};

} // namespace graph
