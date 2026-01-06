#include "FlowNetwork.h"
#include <RandomGenerator.h>
#include <CollectionUtils.h>
#include <MatrixPrinter.h>
#include <algorithm>
#include <numeric>

namespace graph {

void FlowNetwork::addVertex(int id) {
    if (!std::binary_search(m_vertices.begin(), m_vertices.end(), id)) {
        auto it = std::lower_bound(m_vertices.begin(), m_vertices.end(), id);
        m_vertices.insert(it, id);
    }
}

void FlowNetwork::addEdge(int from, int to, double capacity, double cost) {
    addVertex(from);
    addVertex(to);

    CollectionUtils::addUnique(m_adj[from], to);
    CollectionUtils::addUnique(m_adj[to], from);

    m_capacity[from][to] = m_capacity[to][from] = capacity;
    m_cost[from][to] = m_cost[to][from] = cost;
    m_flow[from][to] = m_flow[to][from] = 0.0;
}

std::vector<int> FlowNetwork::vertexIds() const {
    return m_vertices;
}

std::vector<int> FlowNetwork::neighbors(int v) const {
    auto it = m_adj.find(v);
    if (it == m_adj.end()) {
        return {};
    }
    return it->second;
}

double FlowNetwork::getCapacity(int from, int to) const {
    return CollectionUtils::getNestedMapValue(m_capacity, from, to);
}

double FlowNetwork::getCost(int from, int to) const {
    return CollectionUtils::getNestedMapValue(m_cost, from, to);
}

double FlowNetwork::getFlow(int from, int to) const {
    return CollectionUtils::getNestedMapValue(m_flow, from, to);
}

void FlowNetwork::setFlow(int from, int to, double flow) {
    m_flow[from][to] = flow;
}

void FlowNetwork::addFlow(int from, int to, double flow) {
    m_flow[from][to] += flow;
    m_flow[to][from] -= flow;
}

void FlowNetwork::generateFromTree(int numVertices) {
    RandomGenerator rng;

    for (int i = 0; i < numVertices; ++i) {
        addVertex(i);
    }

    //*  алгоритм Прима: строим дерево, добавляя по одной вершине
    std::vector<int> inTree = {0};
    std::vector<int> notInTree(numVertices - 1);
    std::iota(notInTree.begin(), notInTree.end(), 1);

    while (!notInTree.empty()) {
        int from = inTree[rng.randomInt(0, inTree.size() - 1)];
        int toIdx = rng.randomInt(0, notInTree.size() - 1);
        int to = notInTree[toIdx];

        addEdge(from, to, rng.randomDouble(5.0, 20.0), rng.randomDouble(1.0, 10.0));

        inTree.push_back(to);
        notInTree.erase(notInTree.begin() + toIdx);
    }
}

void FlowNetwork::printCapacities() const {
    MatrixPrinter::printMatrix<double>("Матрица пропускных способностей", m_vertices,
                [this](int from, int to) { return getCapacity(from, to); }, 1);
}

void FlowNetwork::printCosts() const {
    MatrixPrinter::printMatrix<double>("Матрица стоимостей", m_vertices,
                [this](int from, int to) { return getCost(from, to); }, 2);
}

} // namespace graph
