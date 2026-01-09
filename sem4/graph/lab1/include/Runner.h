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

    void runShimbellMethod(Graph const& graph, int pathLength);
    ShimbellResult const* getLastShimbell() const;

    int countPaths(Graph const& graph, int from, int to);

    std::optional<PathResult> const& getLastPath() const;
    std::vector<std::vector<int>> const& getAllPaths() const;

private:
    std::optional<PathResult> last_path_;
    std::optional<ShimbellResult> last_shimbell_;
    std::vector<std::vector<int>> all_paths_;
};

}  // namespace lab1
