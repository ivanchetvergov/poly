#include "CutSystem.h"

#include <algorithm>
#include <queue>
#include <unordered_set>

namespace graph {

CutSystem::CutSystem(Graph const& originalGraph, Graph const& mst)
    : originalGraph_(originalGraph), mst_(mst) {}

std::pair<int, int> CutSystem::normalizeEdge(int u, int v) {
    if (u > v) {
        std::swap(u, v);
    }
    return {u, v};
}

std::set<int> CutSystem::componentWithoutTreeEdge(int start,
                                                  std::pair<int, int> removedEdge) const {
    std::set<int> component;
    std::queue<int> q;
    q.push(start);
    component.insert(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto const& [v, _] : mst_.neighbors(u)) {
            auto e = normalizeEdge(u, v);
            if (e == removedEdge) {
                continue;
            }
            if (!component.contains(v)) {
                component.insert(v);
                q.push(v);
            }
        }
    }

    return component;
}

std::vector<int> CutSystem::complementComponent(std::set<int> const& left) const {
    std::vector<int> right;
    for (int v : mst_.vertexIds()) {
        if (!left.contains(v)) {
            right.push_back(v);
        }
    }
    return right;
}

std::vector<CutSystem::FundamentalCut> CutSystem::buildFundamentalCuts() const {
    std::vector<FundamentalCut> result;

    for (auto const& edge : mst_.edges()) {
        auto treeEdge = normalizeEdge(edge.from, edge.to);
        auto left = componentWithoutTreeEdge(treeEdge.first, treeEdge);

        FundamentalCut cut;
        cut.treeEdge = treeEdge;
        cut.leftComponent.assign(left.begin(), left.end());
        cut.rightComponent = complementComponent(left);

        for (auto const& e : originalGraph_.edges()) {
            auto ge = normalizeEdge(e.from, e.to);
            bool inLeftU = left.contains(ge.first);
            bool inLeftV = left.contains(ge.second);
            if (inLeftU != inLeftV) {
                cut.cutEdges.push_back(ge);
            }
        }

        std::sort(cut.cutEdges.begin(), cut.cutEdges.end());
        cut.cutEdges.erase(std::unique(cut.cutEdges.begin(), cut.cutEdges.end()), cut.cutEdges.end());
        result.push_back(std::move(cut));
    }

    return result;
}

std::vector<std::pair<int, int>> CutSystem::symmetricDifference(
    std::vector<FundamentalCut> const& cuts,
    std::vector<int> const& selectedIndices) const {

    std::unordered_set<unsigned long long> active;
    auto encode = [](std::pair<int, int> e) {
        return (static_cast<unsigned long long>(e.first) << 32U) |
               static_cast<unsigned int>(e.second);
    };

    for (int idx : selectedIndices) {
        if (idx < 0 || idx >= static_cast<int>(cuts.size())) {
            continue;
        }
        for (auto const& edge : cuts[idx].cutEdges) {
            auto code = encode(edge);
            if (active.contains(code)) {
                active.erase(code);
            } else {
                active.insert(code);
            }
        }
    }

    std::vector<std::pair<int, int>> out;
    out.reserve(active.size());
    for (auto code : active) {
        int u = static_cast<int>(code >> 32U);
        int v = static_cast<int>(code & 0xFFFFFFFFU);
        out.emplace_back(u, v);
    }

    std::sort(out.begin(), out.end());
    return out;
}

}  // namespace graph
