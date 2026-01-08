#include "FlowNetwork.h"
#include <Generator.h>

#include <algorithm>
#include <numeric>

namespace graph {

void FlowVertex::addNeighbor(int neighborId) {
    m_neighbors_.push_back(neighborId);
}

std::vector<int> FlowVertex::neighbors() const {
    return m_neighbors_;
}

bool FlowNetwork::addEdge(int from, int to, double capacity, double cost) {
    FlowEdge forwardEdge(from, to, capacity, cost, 0.0);
    FlowEdge backwardEdge(to, from, 0.0, -cost, 0.0);

    bool added = GraphBase::addEdge(from, to, forwardEdge);
    if (added) {
        GraphBase::addEdge(to, from, backwardEdge);
        if (hasVertex(to)) {
            m_vertices[to]->addNeighbor(from);
        }
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
    auto edge = getEdgeMutable(from, to);
    if (edge) {
        edge->flow = flow;
    }
}

void FlowNetwork::addFlow(int from, int to, double flow) {
    auto edgeForward = getEdgeMutable(from, to);
    auto edgeBackward = getEdgeMutable(to, from);

    if (edgeForward) {
        edgeForward->flow += flow;
    }
    if (edgeBackward) {
        edgeBackward->flow -= flow;
    }
}

} // namespace graph
