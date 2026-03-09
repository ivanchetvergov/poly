// Dijkstra.h
#pragma once

#include <Graph.h>
#include <unordered_map>
#include <vector>

namespace graph {

struct DijkstraResult {
    std::unordered_map<int, double> dist;           // shortest weighted distance; INF = unreachable
    std::unordered_map<int, int>    parent;         // parent in shortest-path tree; -1 for start
    size_t                          iterations{0};  // total edge relaxation attempts
};

class Dijkstra {
public:
    explicit Dijkstra(Graph const& graph);

    [[nodiscard]] DijkstraResult compute(int start) const;

private:
    Graph const& m_graph_;
};

}  // namespace graph
