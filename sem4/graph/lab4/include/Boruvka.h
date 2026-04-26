#pragma once

#include <Graph.h>
#include <memory>

namespace graph {

class Boruvka {
public:
    explicit Boruvka() = default;

    /// Builds minimum spanning tree using Boruvka's algorithm
    /// Returns a new Graph representing the MST (undirected, with weights)
    [[nodiscard]] std::unique_ptr<Graph> buildMST(Graph const& graph);

private:
    struct Edge {
        int from;
        int to;
        double weight;
    };

    struct Component {
        int id;
        std::vector<int> vertices;
    };

    /// Union-Find data structure
    class UnionFind {
    public:
        explicit UnionFind(std::vector<int> const& vertices);
        [[nodiscard]] int find(int v);
        void unite(int u, int v);

    private:
        std::unordered_map<int, int> parent_;
        std::unordered_map<int, int> rank_;
    };
};

}  // namespace graph
