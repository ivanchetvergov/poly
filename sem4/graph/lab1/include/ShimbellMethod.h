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
    explicit ShimbellMethod(Graph const& graph);

    [[nodiscard]] ShimbellResult const& compute(int pathLength);

private:
    Graph const& m_graph_;
    std::vector<int> m_vertex_ids_;
    int m_size_;
    ShimbellResult result_;

    [[nodiscard]] DistanceMatrix createAdjacencyMatrix() const;
    [[nodiscard]] DistanceMatrix multiplyMin(DistanceMatrix const& a,
                                             DistanceMatrix const& b) const;
    [[nodiscard]] DistanceMatrix multiplyMax(DistanceMatrix const& a,
                                             DistanceMatrix const& b) const;
    [[nodiscard]] int getIndex(int vertexId) const;
};

}  // namespace graph
