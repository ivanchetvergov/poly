// BFS.cc
#include "../include/BFS.h"

#include <queue>

namespace graph {

BFS::BFS(Graph const& graph) : m_graph_(graph) {}

BFSResult BFS::traverse(int start) const {
    BFSResult result;

    for (int v : m_graph_.vertexIds()) {
        result.dist_steps[v] = -1;
        result.parent[v]     = -1;
    }

    result.dist_steps[start] = 0;
    result.visited_order.push_back(start);

    std::queue<int> q;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ++result.iterations;  // dequeue

        for (auto const& [v, w] : m_graph_.neighbors(u)) {
            ++result.iterations;  // edge
            if (result.dist_steps[v] == -1) {
                result.dist_steps[v] = result.dist_steps[u] + 1;
                result.parent[v]     = u;
                result.edges_traversed.emplace_back(u, v);
                result.visited_order.push_back(v);
                q.push(v);
            }
        }
    }

    return result;
}

}  // namespace graph
