#include "FlowNetwork.h"

#include <algorithm>

#include <Generator.h>

namespace graph {

void FlowVertex::addNeighbor(int neighborId) {
    m_neighbors_.push_back(neighborId);
}

void FlowVertex::removeNeighbor(int neighborId) {
    auto it = std::find(m_neighbors_.begin(), m_neighbors_.end(), neighborId);
    if (it != m_neighbors_.end()) {
        m_neighbors_.erase(it);
    }
}

std::vector<int> FlowVertex::neighbors() const {
    return m_neighbors_;
}

bool FlowNetwork::addEdge(int from, int to, double capacity, double cost) {
    FlowEdge forward_edge(from, to, capacity, cost, 0.0);
    FlowEdge backward_edge(to, from, 0.0, -cost, 0.0);

    bool added = GraphBase::addEdge(from, to, forward_edge);
    if (added) {
        GraphBase::addEdge(to, from, backward_edge);
    }

    return added;
}

double FlowNetwork::getCapacity(int from, int to) const {
    auto edge = getEdge(from, to);
    return edge ? edge->capacity : 0.0;
}

double FlowNetwork::getCost(int from, int to) const {
    auto edge = getEdge(from, to);
    return edge ? edge->cost : 0.0;
}

double FlowNetwork::getFlow(int from, int to) const {
    auto edge = getEdge(from, to);
    return edge ? edge->flow : 0.0;
}

double FlowNetwork::getResidualCapacity(int from, int to) const {
    return getCapacity(from, to) - getFlow(from, to);
}

void FlowNetwork::setFlow(int from, int to, double flow) {
    auto* edge = getEdgeMutable(from, to);
    if (edge) {
        edge->flow = flow;
    }
}

void FlowNetwork::addFlow(int from, int to, double flow) {
    auto* edge_forward = getEdgeMutable(from, to);
    auto* edge_backward = getEdgeMutable(to, from);

    if (edge_forward) {
        edge_forward->flow += flow;
    }
    if (edge_backward) {
        edge_backward->flow -= flow;
    }
}

}  // namespace graph
