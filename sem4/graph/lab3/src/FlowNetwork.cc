#include "FlowNetwork.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

namespace graph {

void FlowNetwork::addVertex(int id) {
    if (std::find(m_vertices.begin(), m_vertices.end(), id) == m_vertices.end()) {
        m_vertices.push_back(id);
        std::sort(m_vertices.begin(), m_vertices.end());
    }
}

void FlowNetwork::addEdge(int from, int to, double capacity, double cost) {
    addVertex(from);
    addVertex(to);
    
    if (std::find(m_adj[from].begin(), m_adj[from].end(), to) == m_adj[from].end()) {
        m_adj[from].push_back(to);
    }
    if (std::find(m_adj[to].begin(), m_adj[to].end(), from) == m_adj[to].end()) {
        m_adj[to].push_back(from);
    }
    
    m_capacity[from][to] = capacity;
    m_capacity[to][from] = capacity;
    m_cost[from][to] = cost;
    m_cost[to][from] = cost;
    m_flow[from][to] = 0.0;
    m_flow[to][from] = 0.0;
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
    auto it1 = m_capacity.find(from);
    if (it1 == m_capacity.end()) return 0.0;
    auto it2 = it1->second.find(to);
    if (it2 == it1->second.end()) return 0.0;
    return it2->second;
}

double FlowNetwork::getCost(int from, int to) const {
    auto it1 = m_cost.find(from);
    if (it1 == m_cost.end()) return 0.0;
    auto it2 = it1->second.find(to);
    if (it2 == it1->second.end()) return 0.0;
    return it2->second;
}

double FlowNetwork::getFlow(int from, int to) const {
    auto it1 = m_flow.find(from);
    if (it1 == m_flow.end()) return 0.0;
    auto it2 = it1->second.find(to);
    if (it2 == it1->second.end()) return 0.0;
    return it2->second;
}

void FlowNetwork::setFlow(int from, int to, double flow) {
    m_flow[from][to] = flow;
}

void FlowNetwork::addFlow(int from, int to, double flow) {
    m_flow[from][to] += flow;
    m_flow[to][from] -= flow;
}

void FlowNetwork::generateFromTree(int numVertices) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> capDist(5.0, 20.0);
    std::uniform_real_distribution<> costDist(1.0, 10.0);
    
    for (int i = 0; i < numVertices; ++i) {
        addVertex(i);
    }
    
    std::vector<int> inTree = {0};
    std::vector<int> notInTree;
    for (int i = 1; i < numVertices; ++i) {
        notInTree.push_back(i);
    }
    
    while (!notInTree.empty()) {
        std::uniform_int_distribution<> treeDist(0, static_cast<int>(inTree.size()) - 1);
        int from = inTree[treeDist(gen)];
        
        std::uniform_int_distribution<> notTreeDist(0, static_cast<int>(notInTree.size()) - 1);
        int toIdx = notTreeDist(gen);
        int to = notInTree[toIdx];
        
        double capacity = capDist(gen);
        double cost = costDist(gen);
        
        addEdge(from, to, capacity, cost);
        
        inTree.push_back(to);
        notInTree.erase(notInTree.begin() + toIdx);
    }
}

void FlowNetwork::printCapacities() const {
    std::cout << "\nМатрица пропускных способностей:\n";
    std::cout << std::setw(6) << " ";
    for (int v : m_vertices) {
        std::cout << std::setw(8) << v;
    }
    std::cout << "\n";
    
    for (int from : m_vertices) {
        std::cout << std::setw(6) << from;
        for (int to : m_vertices) {
            double cap = getCapacity(from, to);
            if (cap > 0) {
                std::cout << std::setw(8) << std::fixed << std::setprecision(1) << cap;
            } else {
                std::cout << std::setw(8) << "-";
            }
        }
        std::cout << "\n";
    }
}

void FlowNetwork::printCosts() const {
    std::cout << "\nМатрица стоимостей:\n";
    std::cout << std::setw(6) << " ";
    for (int v : m_vertices) {
        std::cout << std::setw(8) << v;
    }
    std::cout << "\n";
    
    for (int from : m_vertices) {
        std::cout << std::setw(6) << from;
        for (int to : m_vertices) {
            double cost = getCost(from, to);
            if (cost > 0) {
                std::cout << std::setw(8) << std::fixed << std::setprecision(2) << cost;
            } else {
                std::cout << std::setw(8) << "-";
            }
        }
        std::cout << "\n";
    }
}

} // namespace graph
