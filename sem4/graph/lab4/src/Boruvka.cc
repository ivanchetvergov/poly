#include "Boruvka.h"

#include <unordered_map>
#include <vector>
#include <algorithm>

namespace graph {

Boruvka::UnionFind::UnionFind(std::vector<int> const& vertices) {
    for (int v : vertices) {
        parent_[v] = v;
        rank_[v] = 0;
    }
}

int Boruvka::UnionFind::find(int v) {
    if (parent_[v] != v) {
        parent_[v] = find(parent_[v]);  // Path compression
    }
    return parent_[v];
}

void Boruvka::UnionFind::unite(int u, int v) {
    int root_u = find(u);
    int root_v = find(v);

    if (root_u == root_v)
        return;

    // Union by rank
    if (rank_[root_u] < rank_[root_v]) {
        parent_[root_u] = root_v;
    } else if (rank_[root_u] > rank_[root_v]) {
        parent_[root_v] = root_u;
    } else {
        parent_[root_v] = root_u;
        rank_[root_u]++;
    }
}

std::unique_ptr<Graph> Boruvka::buildMST(Graph const& graph) {
    if (graph.isDirected() || graph.vertexCount() == 0) {
        return nullptr;
    }

    std::vector<int> vertices = graph.vertexIds();
    std::sort(vertices.begin(), vertices.end());

    auto mst = std::make_unique<Graph>(false);

    // Add all vertices to MST
    for (int v : vertices) {
        mst->addVertex(v);
    }

    UnionFind uf(vertices);

    // Collect all edges
    std::vector<Edge> edges;
    for (int u : vertices) {
        for (auto const& [v, weight] : graph.neighbors(u)) {
            if (u < v) {  // Avoid duplicates in undirected graph
                edges.push_back({u, v, weight});
            }
        }
    }

    if (vertices.size() == 1) {
        return mst;
    }

    size_t components = vertices.size();

    while (components > 1) {
        std::unordered_map<int, Edge> cheapest;

        for (auto const& edge : edges) {
            int set_u = uf.find(edge.from);
            int set_v = uf.find(edge.to);
            if (set_u == set_v) {
                continue;
            }

            auto update_cheapest = [&cheapest](int component, Edge const& candidate) {
                auto it = cheapest.find(component);
                if (it == cheapest.end() || candidate.weight < it->second.weight) {
                    cheapest[component] = candidate;
                }
            };

            update_cheapest(set_u, edge);
            update_cheapest(set_v, edge);
        }

        if (cheapest.empty()) {
            return nullptr;
        }

        bool merged_any = false;
        for (auto const& [component, edge] : cheapest) {
            (void)component;
            if (uf.find(edge.from) == uf.find(edge.to)) {
                continue;
            }
            uf.unite(edge.from, edge.to);
            mst->addEdge(edge.from, edge.to, edge.weight);
            --components;
            merged_any = true;
        }

        if (!merged_any) {
            return nullptr;
        }
    }

    return mst;
}

}  // namespace graph
