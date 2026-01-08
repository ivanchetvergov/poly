#pragma once

#include "GraphBase.h"
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace graph {

struct FlowEdge {
    int from{-1};
    int to{-1};
    double capacity{1.0};
    double cost{0.0};
    double flow{0.0};

    FlowEdge() = default;
    FlowEdge(int f, int t, double cap, double c = 0.0, double fl = 0.0)
        : from(f), to(t), capacity(cap), cost(c), flow(fl) {}
    bool operator<(const FlowEdge& other) const { return capacity < other.capacity; }
};

class FlowVertex {
public:
    explicit FlowVertex(int id) : m_id_(id) {}

    [[nodiscard]] int id() const noexcept { return m_id_; }
    void addNeighbor(int neighborId);
    [[nodiscard]] std::vector<int> neighbors() const;

private:
    int m_id_;
    std::vector<int> m_neighbors_;
};

class FlowNetwork : public GraphBase<FlowVertex, FlowEdge> {
public:
    FlowNetwork(bool isDirected = true) : GraphBase(isDirected) {}

    bool addEdge(int from, int to, double capacity, double cost = 0.0);

    [[nodiscard]] double getCapacity(int from, int to) const;
    [[nodiscard]] double getCost(int from, int to) const;
    [[nodiscard]] double getFlow(int from, int to) const;
    [[nodiscard]] double getResidualCapacity(int from, int to) const;

    void setFlow(int from, int to, double flow);
    void addFlow(int from, int to, double flow);

    void printCapacities() const;
    void printCosts() const;
};

} // namespace graph

