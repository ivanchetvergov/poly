#pragma once

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
    explicit Vertex(int id) : m_id(id) {}
    
    [[nodiscard]] int id() const noexcept { return m_id; }
    void addNeighbor(int neighborId, double weight);
    [[nodiscard]] const std::vector<std::pair<int, double>>& neighbors() const noexcept;

private:
    int m_id;
    std::vector<std::pair<int, double>> m_neighbors;
};

class Graph {
public:
    Graph() = default;
    
    bool addVertex(int id);
    bool addEdge(int from, int to, double weight);
    
    [[nodiscard]] size_t vertexCount() const noexcept;
    [[nodiscard]] size_t edgeCount() const noexcept;
    [[nodiscard]] std::optional<const Vertex*> getVertex(int id) const;
    [[nodiscard]] const std::vector<EdgeData>& edges() const noexcept;
    [[nodiscard]] std::vector<int> vertexIds() const;
    [[nodiscard]] std::vector<std::pair<int, double>> neighbors(int id) const;
    [[nodiscard]] bool hasVertex(int id) const noexcept;
    [[nodiscard]] bool hasEdge(int from, int to) const;
    [[nodiscard]] std::optional<double> getEdgeWeight(int from, int to) const;
    [[nodiscard]] int degree(int v) const;
    
    void generateWeights(bool allowNegative = false);
    void printGraph() const;
    void printGraphInfo() const;

protected:
    std::unordered_map<int, std::unique_ptr<Vertex>> m_vertices;
    std::vector<EdgeData> m_edges;
};

} // namespace graph
