#include "../include/Runner.h"
#include "../include/ShimbellMethod.h"
#include "../include/PathCounter.h"
#include <Generator.h>
#include <PathUtils.h>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <iostream>

namespace lab1 {

const Graph* Runner::getGraph() const {
    return graph_;
}

void Runner::runShimbellMethod(int pathLength) {
    if (!graph_) {
        throw std::runtime_error("Graph not set");
    }

    ShimbellMethod shimbell(*graph_);
    lastShimbell_ = shimbell.compute(pathLength);
}

int Runner::countPaths(int from, int to) {
    if (!graph_) {
        throw std::runtime_error("Graph not set");
    }

    if (!graph_->hasVertex(from) || !graph_->hasVertex(to)) {
        throw std::invalid_argument("Vertex does not exist");
    }

    PathCounter counter(*graph_);
    allPaths_ = counter.getAllPaths(from, to);
    int count = counter.getPathCount(from, to);

    std::vector<int> path;
    if (!allPaths_.empty()) {
        path = allPaths_[0];
    }

    lastPath_ = PathResult{from, to, path, count};
    return count;
}

const std::optional<PathResult>& Runner::getLastPath() const {
    return lastPath_;
}

const std::vector<std::vector<int>>& Runner::getAllPaths() const {
    return allPaths_;
}

const graph::ShimbellResult* Runner::getLastShimbell() const {
    return lastShimbell_ ? &(*lastShimbell_) : nullptr;
}

} // namespace lab1
