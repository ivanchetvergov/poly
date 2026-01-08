#pragma once

#include "Graph.h"
#include "ShimbellMethod.h"
#include <memory>
#include <optional>
#include <vector>

namespace lab1 {

using namespace graph;

struct PathResult {
    int from;
    int to;
    std::vector<int> path;
    int pathCount;
};

class Runner {
public:
    Runner() = default;
    explicit Runner(Graph* graph) : graph_(graph) {}

    void setGraph(Graph* graph) { graph_ = graph; }
    const Graph* getGraph() const;

    void runShimbellMethod(int pathLength);
    const ShimbellResult* getLastShimbell() const;

    int countPaths(int from, int to);

    const std::optional<PathResult>& getLastPath() const;
    const std::vector<std::vector<int>>& getAllPaths() const;

private:
    Graph* graph_ = nullptr;
    std::optional<PathResult> lastPath_;
    std::optional<ShimbellResult> lastShimbell_;
    std::vector<std::vector<int>> allPaths_;
};

} // namespace lab1
