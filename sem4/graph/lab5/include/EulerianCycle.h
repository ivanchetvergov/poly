#pragma once

#include <optional>
#include <vector>
#include <utility>
#include <string>

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

    [[nodiscard]] std::vector<std::pair<int,int>> const& getAddedEdges() const {
        return m_added_edges_;
    }

    [[nodiscard]] std::vector<std::pair<int,int>> const& getRemovedEdges() const {
        return m_removed_edges_;
    }

private:
    Graph& m_graph_;
    std::vector<std::pair<int,int>> m_added_edges_;
    std::vector<std::pair<int,int>> m_removed_edges_;

    void logDebug(std::string const& message) const;
    [[nodiscard]] std::string oddVerticesToString(std::vector<int> const& odd) const;
    [[nodiscard]] std::vector<std::pair<int, int>> buildPairsWithoutExistingEdges(
        std::vector<int> const& odd) const;
    [[nodiscard]] int addTrackedPairs(std::vector<std::pair<int, int>> const& pairs);

    [[nodiscard]] std::optional<std::vector<int>> hierholzer(int start, int totalEdges) const;

    [[nodiscard]] std::vector<std::vector<int>> nonZeroComponents() const;
    [[nodiscard]] bool hasSingleNonZeroComponent() const;

    [[nodiscard]] bool addEdgeTracked(int u, int v);
};

}  // namespace graph
