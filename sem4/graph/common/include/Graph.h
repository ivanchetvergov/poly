#pragma once

#include "GraphBase.h"
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

namespace graph {

struct EdgeData {
    int from{-1};
    int to{-1};
    double weight{1.0};

    EdgeData() = default;
    EdgeData(int f, int t, double w) : from(f), to(t), weight(w) {}
    bool operator<(const EdgeData& other) const { return weight < other.weight; }
};

class Vertex {
public:
    explicit Vertex(int id) : m_id_(id) {}

    [[nodiscard]] int id() const noexcept { return m_id_; }
    void addNeighbor(int neighborId);
    [[nodiscard]] std::vector<int> neighbors() const;

private:
    int m_id_;
    std::vector<int> m_neighbors_;
};

class Graph : public GraphBase<Vertex, EdgeData> {
public:
    Graph(bool isDirected = false) : GraphBase(isDirected) {}

    bool addEdge(int from, int to, double weight);

    [[nodiscard]] std::optional<double> getEdgeWeight(int from, int to) const;
    [[nodiscard]] std::vector<std::pair<int, double>> neighbors(int id) const;
    [[nodiscard]] std::vector<int> getNeighbors(int id) const;


    void printGraphInfo() const;
};

} // namespace graph
