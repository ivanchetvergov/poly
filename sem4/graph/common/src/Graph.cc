#include "Graph.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

namespace graph {

void Vertex::addNeighbor(int neighborId) {
    m_neighbors_.push_back(neighborId);
}

std::vector<int> Vertex::neighbors() const {
    return m_neighbors_;
}

bool Graph::addEdge(int from, int to, double weight) {
    EdgeData edge(from, to, weight);
    bool added = GraphBase::addEdge(from, to, edge);

    if (added && !isDirected_ && hasVertex(to)) {
        this->m_vertices[to]->addNeighbor(from);
    }

    return added;
}

std::optional<double> Graph::getEdgeWeight(int from, int to) const {
    auto edge = getEdge(from, to);
    if (!edge) return std::nullopt;
    return edge->weight;
}


std::vector<std::pair<int, double>> Graph::neighbors(int id) const {
    auto neighs = GraphBase::neighbors(id);
    std::vector<std::pair<int, double>> result;
    for (int nb : neighs) {
        auto weight = getEdgeWeight(id, nb);
        if (weight) {
            result.emplace_back(nb, *weight);
        }
    }
    return result;
}

std::vector<int> Graph::getNeighbors(int id) const {
    return GraphBase::neighbors(id);
}

void Graph::printGraphInfo() const {
    std::cout << "Информация о графе:\n";
    std::cout << "Количество вершин: " << vertexCount() << "\n";
    std::cout << "Количество рёбер: " << edgeCount() << "\n";
}

} // namespace graph
