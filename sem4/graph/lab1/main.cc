#include <Graph.h>
#include <GraphGenerator.h>
#include <Utils.h>
#include "PathCounter.h"
#include "ShimbellMethod.h"

#include <iomanip>
#include <iostream>

using namespace graph;


void runShimbellMethod(const Graph& graph) {
    std::cout << "\n=== Метод Шимбелла ===\n";
    
    int pathLength = readInt("Введите длину пути (количество ребер): ");
    
    if (pathLength <= 0) {
        std::cout << "Длина пути должна быть положительной.\n";
        return;
    }

    try {
        ShimbellMethod shimbellMethod(graph);
        auto result = shimbellMethod.compute(pathLength);
        
        ShimbellMethod::printMatrix(result.minDistances, 
            "Матрица минимальных расстояний");
        ShimbellMethod::printMatrix(result.maxDistances, 
            "Матрица максимальных расстояний");
            
    } catch (const std::exception& e) {
        std::cout << "Ошибка при выполнении метода Шимбелла: " << e.what() << "\n";
    }
}

void countPathsBetweenVertices(const Graph& graph) {
    std::cout << "\n=== Подсчёт количества маршрутов ===\n";
    
    int from = readInt("Введите начальную вершину: ");
    int to = readInt("Введите конечную вершину: ");
    
    PathCounter counter(graph);
    
    if (!graph.hasVertex(from)) {
        std::cout << "Вершина " << from << " не существует в графе.\n";
        return;
    }
    
    if (!graph.hasVertex(to)) {
        std::cout << "Вершина " << to << " не существует в графе.\n";
        return;
    }
    
    auto pathCount = counter.countPaths(from, to);
    
    if (!pathCount.has_value()) {
        std::cout << "Не удалось подсчитать пути.\n";
        return;
    }
    
    if (pathCount.value() == 0) {
        std::cout << "Маршрут от вершины " << from << " до вершины " << to 
                  << " не существует.\n";
    } else {
        std::cout << "Количество маршрутов от вершины " << from << " до вершины " << to 
                  << ": " << pathCount.value() << "\n";
    }
}

int main() {
    try {
        
        int numVertices = readInt("\nВведите количество вершин: ");
        int numEdges = readInt("\nВведите количество ребер: ");
        
        if (numVertices <= 0) {
            std::cout << "Количество вершин должно быть положительным.\n";
            return 1;
        }
        
        std::cout << "\nГенерация связного ациклического графа...\n";
        std::cout << "Параметры распределения Эрланга: shape=3, rate=1.5\n";
        
        auto graph = generateRandomGraph(numVertices, numEdges);
        
        graph->printGraphInfo();
        graph->printGraph();
        
        runShimbellMethod(*graph);
        
        bool continueCountingPaths = true;
        while (continueCountingPaths) {
            countPathsBetweenVertices(*graph);
            
            std::cout << "\nПодсчитать пути для других вершин? (1 - да, 0 - нет): ";
            int choice;
            std::cin >> choice;
            continueCountingPaths = (choice == 1);
        }
        
        std::cout << "\nРабота программы завершена.\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
