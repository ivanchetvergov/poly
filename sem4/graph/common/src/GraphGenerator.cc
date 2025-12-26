#include "GraphGenerator.h"
#include <algorithm>
#include <random>
#include <stdexcept>

namespace graph {

std::unique_ptr<Graph> generateRandomTree(int numVertices, int shape, double rate) {
    if (numVertices <= 0) {
        throw std::invalid_argument("Количество вершин должно быть положительным");
    }
    
    auto graph = std::make_unique<Graph>();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::gamma_distribution<double> erlang(static_cast<double>(shape), 1.0 / rate);
    
    for (int i = 0; i < numVertices; ++i) {
        graph->addVertex(i);
    }
    
    if (numVertices == 1) {
        return graph;
    }
    
    std::vector<int> inTree = {0};
    std::vector<int> notInTree;
    for (int i = 1; i < numVertices; ++i) {
        notInTree.push_back(i);
    }
    
    while (!notInTree.empty()) {
        std::uniform_int_distribution<> treeDist(0, static_cast<int>(inTree.size()) - 1);
        int fromVertex = inTree[treeDist(gen)];
        
        std::uniform_int_distribution<> notTreeDist(0, static_cast<int>(notInTree.size()) - 1);
        int toIndex = notTreeDist(gen);
        int toVertex = notInTree[toIndex];
        
        double weight = erlang(gen);
        graph->addEdge(fromVertex, toVertex, weight);
        
        inTree.push_back(toVertex);
        notInTree.erase(notInTree.begin() + toIndex);
    }
    
    return graph;
}

std::unique_ptr<Graph> generateRandomGraph(int numVertices, int numEdges) {
    auto graph = std::make_unique<Graph>();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> weightDist(1.0, 10.0);
    
    for (int i = 0; i < numVertices; ++i) {
        graph->addVertex(i);
    }
    
    for (int i = 1; i < numVertices; ++i) {
        std::uniform_int_distribution<> dist(0, i - 1);
        int from = dist(gen);
        graph->addEdge(from, i, weightDist(gen));
    }
    
    int currentEdges = numVertices - 1;
    int maxEdges = numVertices * (numVertices - 1) / 2;
    numEdges = std::min(numEdges, maxEdges);
    
    while (currentEdges < numEdges) {
        std::uniform_int_distribution<> vDist(0, numVertices - 1);
        int from = vDist(gen);
        int to = vDist(gen);
        
        if (from != to && !graph->hasEdge(from, to)) {
            graph->addEdge(from, to, weightDist(gen));
            currentEdges++;
        }
    }
    
    return graph;
}

} // namespace graph
