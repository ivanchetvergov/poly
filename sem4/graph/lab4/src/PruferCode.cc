#include "PruferCode.h"

#include <unordered_map>
#include <vector>
#include <algorithm>

namespace graph {

PruferCode::PruferEncoding PruferCode::encode(Graph const& tree) {
    PruferEncoding result;

    std::vector<int> vertices = tree.vertexIds();
    std::sort(vertices.begin(), vertices.end());
    result.vertices = vertices;
    std::unordered_map<int, int> degree;

    // Calculate degrees
    for (int v : vertices) {
        degree[v] = tree.degree(v);
    }

    // Store edge weights
    for (int u : vertices) {
        for (auto const& [v, weight] : tree.neighbors(u)) {
            if (u < v) {
                result.edgeWeights[{u, v}] = weight;
            }
        }
    }

    if (vertices.size() <= 2) {
        return result;
    }

    // Build Prufer sequence
    for (size_t i = 0; i < vertices.size() - 2; ++i) {
        // Find leaf with minimum label
        int leaf = -1;
        for (int v : vertices) {
            if (degree[v] == 1) {
                leaf = v;
                break;
            }
        }

        if (leaf == -1)
            break;

        // Find neighbor of leaf
        for (auto const& [u, weight] : tree.neighbors(leaf)) {
            if (degree[u] > 0) {
                result.sequence.push_back(u);
                degree[leaf]--;
                degree[u]--;
                break;
            }
        }
    }

    return result;
}

std::unique_ptr<Graph> PruferCode::decode(PruferEncoding const& encoding) {
    auto tree = std::make_unique<Graph>(false);

    auto vertices = encoding.vertices;
    std::sort(vertices.begin(), vertices.end());

    // Add all vertices
    for (int v : vertices) {
        tree->addVertex(v);
    }

    if (vertices.empty()) {
        return tree;
    }

    auto lookupWeight = [&encoding](int u, int v) {
        int from = std::min(u, v);
        int to = std::max(u, v);
        auto it = encoding.edgeWeights.find({from, to});
        return it != encoding.edgeWeights.end() ? it->second : 1.0;
    };

    if (vertices.size() == 1) {
        return tree;
    }

    if (vertices.size() == 2) {
        tree->addEdge(vertices[0], vertices[1], lookupWeight(vertices[0], vertices[1]));
        return tree;
    }

    // Calculate degrees from Prufer sequence
    std::unordered_map<int, int> degree;
    for (int v : vertices) {
        degree[v] = 1;
    }
    for (int v : encoding.sequence) {
        degree[v]++;
    }

    // Reconstruct edges
    for (int v : encoding.sequence) {
        // Find leaf with minimum label
        int leaf = -1;
        for (int candidate : vertices) {
            if (degree[candidate] == 1) {
                leaf = candidate;
                break;
            }
        }

        if (leaf == -1)
            break;

        tree->addEdge(leaf, v, lookupWeight(leaf, v));
        degree[leaf]--;
        degree[v]--;
    }

    // Add final edge between two remaining degree-1 vertices
    int v1 = -1, v2 = -1;
    for (int candidate : vertices) {
        if (degree[candidate] == 1) {
            if (v1 == -1) {
                v1 = candidate;
            } else {
                v2 = candidate;
                break;
            }
        }
    }

    if (v1 != -1 && v2 != -1) {
        tree->addEdge(v1, v2, lookupWeight(v1, v2));
    }

    return tree;
}

}  // namespace graph
