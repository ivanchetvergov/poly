#include "Graph.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>

namespace graph {

void Vertex::addNeighbor(int neighborId, double weight) {
    m_neighbors.emplace_back(neighborId, weight);
}

const std::vector<std::pair<int, double>>& Vertex::neighbors() const noexcept {
    return m_neighbors;
}

bool Graph::addVertex(int id) {
    if (m_vertices.find(id) != m_vertices.end()) {
        return false;
    }
    m_vertices[id] = std::make_unique<Vertex>(id);
    return true;
}

bool Graph::addEdge(int from, int to, double weight) {
    if (!hasVertex(from) || !hasVertex(to)) {
        return false;
    }
    
    m_edges.emplace_back(from, to, weight);
    m_vertices[from]->addNeighbor(to, weight);
    m_vertices[to]->addNeighbor(from, weight);
    return true;
}

size_t Graph::vertexCount() const noexcept {
    return m_vertices.size();
}

size_t Graph::edgeCount() const noexcept {
    return m_edges.size();
}

std::optional<const Vertex*> Graph::getVertex(int id) const {
    auto it = m_vertices.find(id);
    if (it == m_vertices.end()) {
        return std::nullopt;
    }
    return it->second.get();
}

const std::vector<EdgeData>& Graph::edges() const noexcept {
    return m_edges;
}

std::vector<int> Graph::vertexIds() const {
    std::vector<int> ids;
    ids.reserve(m_vertices.size());
    for (const auto& [id, _] : m_vertices) {
        ids.push_back(id);
    }
    std::sort(ids.begin(), ids.end());
    return ids;
}

std::vector<std::pair<int, double>> Graph::neighbors(int id) const {
    auto vertex = getVertex(id);
    if (!vertex) {
        return {};
    }
    return (*vertex)->neighbors();
}

bool Graph::hasVertex(int id) const noexcept {
    return m_vertices.find(id) != m_vertices.end();
}

bool Graph::hasEdge(int from, int to) const {
    auto vertex = getVertex(from);
    if (!vertex) return false;
    
    for (const auto& [neighborId, _] : (*vertex)->neighbors()) {
        if (neighborId == to) return true;
    }
    return false;
}

std::optional<double> Graph::getEdgeWeight(int from, int to) const {
    auto vertex = getVertex(from);
    if (!vertex) {
        return std::nullopt;
    }
    
    for (const auto& [neighborId, weight] : (*vertex)->neighbors()) {
        if (neighborId == to) {
            return weight;
        }
    }
    return std::nullopt;
}

int Graph::degree(int v) const {
    return static_cast<int>(neighbors(v).size());
}

void Graph::generateWeights(bool allowNegative) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(1.0, 10.0);
    std::uniform_real_distribution<> negDist(-5.0, 10.0);
    
    std::vector<std::pair<int, int>> edgePairs;
    for (const auto& edge : m_edges) {
        edgePairs.emplace_back(edge.from, edge.to);
    }
    
    m_edges.clear();
    for (auto& [_, vertex] : m_vertices) {
        vertex = std::make_unique<Vertex>(vertex->id());
    }
    
    for (const auto& [from, to] : edgePairs) {
        double weight = allowNegative ? negDist(gen) : dist(gen);
        addEdge(from, to, weight);
    }
}

void Graph::printGraph() const {
    std::cout << "\nРебра графа:\n";
    std::cout << std::setw(8) << "От" << std::setw(8) << "До" << std::setw(12) << "Вес\n";
    std::cout << std::string(28, '-') << "\n";
    
    for (const auto& edge : m_edges) {
        std::cout << std::setw(8) << edge.from
                  << std::setw(8) << edge.to
                  << std::setw(12) << std::fixed << std::setprecision(2) << edge.weight << "\n";
    }
}

} // namespace graph
