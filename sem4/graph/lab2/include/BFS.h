// BFS.h
#pragma once

#include <Graph.h>
#include <unordered_map>
#include <utility>
#include <vector>

namespace graph {

struct BFSResult {
    std::vector<int>                 visited_order;    // vertices in visit order
    std::vector<std::pair<int, int>> edges_traversed;  // tree edges (from, to) in order
    std::unordered_map<int, int>     parent;           // parent in BFS tree; -1 for start
    std::unordered_map<int, int>     dist_steps;       // hop distance from start; -1 = unreachable
    size_t                           iterations{0};    // total edge examinations
};

class BFS {
public:
    explicit BFS(Graph const& graph);

    [[nodiscard]] BFSResult traverse(int start) const;

private:
    Graph const& m_graph_;
};

}  // namespace graph
