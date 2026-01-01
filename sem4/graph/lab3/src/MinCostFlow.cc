#include "MinCostFlow.h"
#include <algorithm>
#include <limits>

namespace graph {

bool MinCostFlow::bellmanFord(int source, int sink,
                                std::unordered_map<int, double>& dist,
                                std::unordered_map<int, int>& parent) {
    dist.clear();
    parent.clear();
    
    for (int v : m_network.vertexIds()) {
        dist[v] = std::numeric_limits<double>::infinity();
    }
    dist[source] = 0.0;
    
    int n = static_cast<int>(m_network.vertexCount());
    
    // Релаксация рёбер
    for (int i = 0; i < n - 1; ++i) {
        bool updated = false;
        
        for (int u : m_network.vertexIds()) {
            if (dist[u] == std::numeric_limits<double>::infinity()) continue;
            
            for (int v : m_network.neighbors(u)) {
                double residualCap = m_network.getCapacity(u, v) - m_network.getFlow(u, v);
                
                if (residualCap > 0) {
                    double newDist = dist[u] + m_network.getCost(u, v);
                    
                    if (newDist < dist[v]) {
                        dist[v] = newDist;
                        parent[v] = u;
                        updated = true;
                    }
                }
            }
        }
        
        if (!updated) break;
    }
    
    return dist[sink] != std::numeric_limits<double>::infinity();
}

MinCostFlow::Result MinCostFlow::findMinCostFlow(int source, int sink, double targetFlow) {
    Result result;
    
    // Сброс потоков
    for (int u : m_network.vertexIds()) {
        for (int v : m_network.neighbors(u)) {
            m_network.setFlow(u, v, 0.0);
        }
    }
    
    double currentFlow = 0.0;
    double totalCost = 0.0;
    
    std::unordered_map<int, double> dist;
    std::unordered_map<int, int> parent;
    
    // Пока не достигнут целевой поток
    while (currentFlow < targetFlow && bellmanFord(source, sink, dist, parent)) {
        // Находим минимальную пропускную способность вдоль пути
        double pathFlow = targetFlow - currentFlow;
        
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            double residualCap = m_network.getCapacity(u, v) - m_network.getFlow(u, v);
            pathFlow = std::min(pathFlow, residualCap);
        }
        
        // Обновляем поток и считаем стоимость
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            m_network.addFlow(u, v, pathFlow);
            totalCost += pathFlow * m_network.getCost(u, v);
        }
        
        currentFlow += pathFlow;
    }
    
    result.flow = currentFlow;
    result.cost = totalCost;
    result.success = (currentFlow == targetFlow);
    
    return result;
}

} // namespace graph
