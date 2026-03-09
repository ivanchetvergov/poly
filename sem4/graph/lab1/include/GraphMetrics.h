// GraphMetrics.h
#pragma once

#include <Graph.h>
#include <limits>
#include <unordered_map>
#include <vector>

namespace graph {

struct MetricsResult {
    std::unordered_map<int, double> eccentricities;
    std::vector<int> center;
    std::vector<int> diametralVerts;
    double radius{0.0};
    double diameter{0.0};
};

class GraphMetrics {
public:
    explicit GraphMetrics(Graph const& graph);

    [[nodiscard]] MetricsResult compute() const;

private:
    Graph const& m_graph_;
};

}  // namespace graph
