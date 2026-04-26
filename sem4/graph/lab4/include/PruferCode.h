#pragma once

#include <Graph.h>
#include <vector>
#include <memory>
#include <unordered_map>

namespace graph {

// Hash function for pair<int, int>
struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        auto h1 = std::hash<int>{}(p.first);
        auto h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

class PruferCode {
public:
    explicit PruferCode() = default;

    /// Encode tree to Prufer sequence preserving weights
    /// Returns vector of vertex IDs
    struct PruferEncoding {
        std::vector<int> vertices;
        std::vector<int> sequence;
        std::unordered_map<std::pair<int, int>, double, PairHash> edgeWeights;
    };

    [[nodiscard]] PruferEncoding encode(Graph const& tree);

    /// Decode Prufer sequence back to tree with original weights
    [[nodiscard]] std::unique_ptr<Graph> decode(PruferEncoding const& encoding);
};

}  // namespace graph
