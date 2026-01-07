// ShimbellMethod.h
#pragma once

#include <Graph.h>
#include <limits>
#include <optional>
#include <vector>

namespace graph {

using DistanceMatrix = std::vector<std::vector<std::optional<double>>>;

struct ShimbellResult {
    DistanceMatrix minDistances;
    DistanceMatrix maxDistances;
    int pathLength;
};

class ShimbellMethod {
public:
    explicit ShimbellMethod(const Graph& graph);

    [[nodiscard]] const ShimbellResult& compute(int pathLength);

private:
    const Graph& m_graph;
    std::vector<int> m_vertexIds;
    int m_size;
    ShimbellResult result_;

    [[nodiscard]] DistanceMatrix createAdjacencyMatrix() const;
    [[nodiscard]] DistanceMatrix multiplyMin(const DistanceMatrix& a, const DistanceMatrix& b) const;
    [[nodiscard]] DistanceMatrix multiplyMax(const DistanceMatrix& a, const DistanceMatrix& b) const;
    [[nodiscard]] int getIndex(int vertexId) const;
};

} // namespace graph
