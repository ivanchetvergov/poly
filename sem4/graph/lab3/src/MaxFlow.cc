#include "MaxFlow.h"
#include <algorithm>
#include <limits>
#include <unordered_set>

namespace graph {

bool MaxFlow::bfs(int source, int sink, std::unordered_map<int, int>& parent) {
    std::unordered_set<int> visited;
    std::queue<int> q;
    
    q.push(source);
    visited.insert(source);
    parent.clear();
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : m_network.neighbors(u)) {
            if (visited.find(v) == visited.end()) {
                double residualCap = m_network.getCapacity(u, v) - m_network.getFlow(u, v);
                
                if (residualCap > 0) {
                    visited.insert(v);
                    parent[v] = u;
                    
                    if (v == sink) {
                        return true;
                    }
                    
                    q.push(v);
                }
            }
        }
    }
    
    return false;
}

double MaxFlow::fordFulkerson(int source, int sink) {
    double maxFlow = 0.0;
    std::unordered_map<int, int> parent;
    
    // Сброс потоков
    for (int u : m_network.vertexIds()) {
        for (int v : m_network.neighbors(u)) {
            m_network.setFlow(u, v, 0.0);
        }
    }
    
    // Пока есть увеличивающий путь
    while (bfs(source, sink, parent)) {
        // Находим минимальную пропускную способность вдоль пути
        double pathFlow = std::numeric_limits<double>::infinity();
        
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            double residualCap = m_network.getCapacity(u, v) - m_network.getFlow(u, v);
            pathFlow = std::min(pathFlow, residualCap);
        }
        
        // Обновляем поток вдоль пути
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            m_network.addFlow(u, v, pathFlow);
        }
        
        maxFlow += pathFlow;
    }
    
    return maxFlow;
}

} // namespace graph
