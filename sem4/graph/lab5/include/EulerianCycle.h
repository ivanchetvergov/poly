#pragma once

#include <memory>
#include <optional>
#include <vector>

#include <Graph.h>

namespace graph {

class EulerianCycle {
public:
    explicit EulerianCycle(Graph& graph) : m_graph_(graph) {}

    [[nodiscard]] bool isEulerian() const;
    [[nodiscard]] bool isSemiEulerian() const;
    [[nodiscard]] std::vector<int> getOddDegreeVertices() const;

    void makeEulerian();
    [[nodiscard]] std::optional<std::vector<int>> findCycle();

    [[nodiscard]] std::vector<std::pair<int, int>> const& getAddedEdges() const {
        return m_added_edges_;
    }

private:
    Graph& m_graph_;
    std::vector<std::pair<int, int>> m_added_edges_;

    [[nodiscard]] std::optional<std::vector<int>> findEulerianCycle(int start);
};

}  // namespace graph
