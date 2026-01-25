// ShimbellMethod.h
#pragma once

#include <limits>
#include <optional>
#include <vector>

#include <Graph.h>

namespace graph {

using DistanceMatrix = std::vector<std::vector<std::optional<double>>>;

struct ShimbellResult {
    DistanceMatrix min_distances;
    DistanceMatrix max_distances;
    int path_length;
};

class ShimbellMethod {
public:
    explicit ShimbellMethod(const Graph& graph);

    [[nodiscard]] const ShimbellResult& compute(int pathLength);

private:
    const Graph& m_graph_;
    std::vector<int> m_vertex_ids_;
    int m_size_;
    ShimbellResult result_;

    [[nodiscard]] DistanceMatrix createAdjacencyMatrix() const;
    [[nodiscard]] DistanceMatrix multiplyMin(const DistanceMatrix& a,
                                             const DistanceMatrix& b) const;
    [[nodiscard]] DistanceMatrix multiplyMax(const DistanceMatrix& a,
                                             const DistanceMatrix& b) const;
    [[nodiscard]] int getIndex(int vertexId) const;
};

}  // namespace graph
