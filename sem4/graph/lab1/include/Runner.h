#pragma once

#include "Graph.h"
#include "ShimbellMethod.h"

#include <memory>
#include <optional>
#include <vector>

namespace lab1 {

using graph::Graph;
using graph::ShimbellResult;

struct PathResult {
    int from;
    int to;
    std::vector<int> path;
    int path_count;
};

class Runner {
public:
    Runner() = default;
    explicit Runner(Graph* graph) : graph_(graph) {}

    void setGraph(Graph* graph) { graph_ = graph; }
    Graph const* getGraph() const;

    void runShimbellMethod(int pathLength);
    ShimbellResult const* getLastShimbell() const;

    int countPaths(int from, int to);

    std::optional<PathResult> const& getLastPath() const;
    std::vector<std::vector<int>> const& getAllPaths() const;

private:
    Graph* graph_ = nullptr;
    std::optional<PathResult> last_path_;
    std::optional<ShimbellResult> last_shimbell_;
    std::vector<std::vector<int>> all_paths_;
};

}  // namespace lab1
