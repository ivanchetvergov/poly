#include "FlowNetwork.h"
#include <Generator.h>

#include <algorithm>
#include <numeric>

namespace graph {

void FlowVertex::addNeighbor(int neighborId) {
    m_neighbors.push_back(neighborId);
}

std::vector<int> FlowVertex::neighbors() const {
    return m_neighbors;
}

bool FlowNetwork::addEdge(int from, int to, double capacity, double cost) {
    FlowEdge edge(from, to, capacity, cost, 0.0);
    bool added = GraphBase::addEdge(from, to, edge);

    if (added && hasVertex(to)) {
        m_vertices[to]->addNeighbor(from);
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

void FlowNetwork::generateFromTree(int numVertices) {
    Generator gen;

    for (int i = 0; i < numVertices; ++i) {
        addVertex(i);
    }

    std::vector<int> inTree = {0};
    std::vector<int> notInTree(numVertices - 1);
    std::iota(notInTree.begin(), notInTree.end(), 1);

    while (!notInTree.empty()) {
        int from = inTree[gen.randomInt(0, static_cast<int>(inTree.size()) - 1)];
        int toIdx = gen.randomInt(0, static_cast<int>(notInTree.size()) - 1);
        int to = notInTree[toIdx];

        addEdge(from, to, gen.randomReal(5.0, 20.0), gen.randomReal(1.0, 10.0));

        inTree.push_back(to);
        notInTree.erase(notInTree.begin() + toIdx);
    }
}


} // namespace graph
