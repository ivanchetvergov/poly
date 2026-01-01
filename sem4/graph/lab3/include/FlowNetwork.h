#pragma once

#include <memory>
#include <optional>
#include <random>
#include <unordered_map>
#include <vector>

namespace graph {

class FlowNetwork {
public:
    FlowNetwork() = default;
    
    void addVertex(int id);
    void addEdge(int from, int to, double capacity, double cost = 0.0);
    
    [[nodiscard]] size_t vertexCount() const noexcept { return m_vertices.size(); }
    [[nodiscard]] std::vector<int> vertexIds() const;
    [[nodiscard]] std::vector<int> neighbors(int v) const;
    
    [[nodiscard]] double getCapacity(int from, int to) const;
    [[nodiscard]] double getCost(int from, int to) const;
    [[nodiscard]] double getFlow(int from, int to) const;
    
    void setFlow(int from, int to, double flow);
    void addFlow(int from, int to, double flow);
    
    void generateFromTree(int numVertices);
    
    void printCapacities() const;
    void printCosts() const;

private:
    std::unordered_map<int, std::vector<int>> m_adj;
    std::unordered_map<int, std::unordered_map<int, double>> m_capacity;
    std::unordered_map<int, std::unordered_map<int, double>> m_cost;
    std::unordered_map<int, std::unordered_map<int, double>> m_flow;
    std::vector<int> m_vertices;
};

} // namespace graph
