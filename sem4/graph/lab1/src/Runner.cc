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

PathResult Runner::countPaths(int from, int to) {
    if (!graph_) {
        throw std::runtime_error("Graph not set");
    }

    if (!graph_->hasVertex(from) || !graph_->hasVertex(to)) {
        throw std::invalid_argument("Vertex does not exist");
    }

    PathCounter counter(*graph_);
    int count = counter.countPaths(from, to).value_or(0);

    std::vector<int> path;
    if (count > 0) {
        std::unordered_map<int, int> parent{{from, -1}};
        std::queue<int> q;
        q.push(from);

        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            if (curr == to) {
                for (int v = to; v != -1; v = parent[v]) {
                    path.push_back(v);
                }
                std::reverse(path.begin(), path.end());
                break;
            }
            for (const auto& [nb, _] : graph_->neighbors(curr)) {
                if (parent.find(nb) == parent.end()) {
                    parent[nb] = curr;
                    q.push(nb);
                }
            }
        }
    }

    lastPath_ = PathResult{from, to, path, count};
    return *lastPath_;
}

const std::optional<PathResult>& Runner::getLastPath() const {
    return lastPath_;
}

const graph::ShimbellResult* Runner::getLastShimbell() const {
    return lastShimbell_ ? &(*lastShimbell_) : nullptr;
}

} // namespace lab1
