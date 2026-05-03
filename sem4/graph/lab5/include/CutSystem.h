#pragma once

#include <Graph.h>

#include <set>
#include <utility>
#include <vector>

namespace graph {

class CutSystem {
public:
    struct FundamentalCut {
        std::pair<int, int> treeEdge;
        std::vector<int> leftComponent;
        std::vector<int> rightComponent;
        std::vector<std::pair<int, int>> cutEdges;
    };

    CutSystem(Graph const& originalGraph, Graph const& mst);

    [[nodiscard]] std::vector<FundamentalCut> buildFundamentalCuts() const;

    [[nodiscard]] std::vector<std::pair<int, int>> symmetricDifference(
        std::vector<FundamentalCut> const& cuts,
        std::vector<int> const& selectedIndices) const;

private:
    Graph const& originalGraph_;
    Graph const& mst_;

    [[nodiscard]] static std::pair<int, int> normalizeEdge(int u, int v);
    [[nodiscard]] std::set<int> componentWithoutTreeEdge(int start,
                                                         std::pair<int, int> removedEdge) const;
    [[nodiscard]] std::vector<int> complementComponent(std::set<int> const& left) const;
};

}  // namespace graph
