#include "../include/Runner.h"

#include "../include/PathCounter.h"
#include "../include/ShimbellMethod.h"

#include <iostream>

#include <Generator.h>
#include <PathUtils.h>

namespace lab1 {

using graph::PathCounter;
using graph::ShimbellMethod;

Graph const* Runner::getGraph() const {
    return graph_;
}

void Runner::runShimbellMethod(int pathLength) {
    if (!graph_) {
        throw std::runtime_error("Graph not set");
    }

    ShimbellMethod shimbell(*graph_);
    last_shimbell_ = shimbell.compute(pathLength);
}

int Runner::countPaths(int from, int to) {
    if (!graph_) {
        throw std::runtime_error("Graph not set");
    }

    if (!graph_->hasVertex(from) || !graph_->hasVertex(to)) {
        throw std::invalid_argument("Vertex does not exist");
    }

    PathCounter counter(*graph_);
    all_paths_ = counter.getAllPaths(from, to);
    int count = counter.getPathCount(from, to);

    std::vector<int> path;
    if (!all_paths_.empty()) {
        path = all_paths_[0];
    }

    last_path_ = PathResult{.from=from, .to=to, .path=path, .path_count=count};
    return count;
}

std::optional<PathResult> const& Runner::getLastPath() const {
    return last_path_;
}

std::vector<std::vector<int>> const& Runner::getAllPaths() const {
    return all_paths_;
}

graph::ShimbellResult const* Runner::getLastShimbell() const {
    return last_shimbell_ ? &(*last_shimbell_) : nullptr;
}

}  // namespace lab1
