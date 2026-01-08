#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include <map>
#include <Graph.h>
#include <Generator.h>
#include <Visualizer.h>
#include <Animator.h>
#include <Utils.h>
#include "../../lab1/include/Runner.h"
#include "../../lab3/include/Runner.h"
#include "../../lab5/include/Runner.h"

namespace graph {

class Menu {
public:
    Menu();
    void show() const;
    void handleChoice(int choice);

private:
    std::unique_ptr<Graph> graph_;
    std::unique_ptr<FlowNetwork> flowNet_;
    Generator gen_;
    lab1::Runner lab1Runner_;
    lab3::Runner lab3Runner_;
    lab5::Runner lab5Runner_;
    double lastMaxFlow_ = 0.0;

    const char* noGraphMsg_ = "Сначала сгенерируйте граф (пункт 1)";
    const char* noFlowMsg_ = "Сначала сгенерируйте сеть потоков (пункт 31)";
    const char* noMaxFlowMsg_ = "Сначала найдите максимальный поток (пункт 32)";
    const char* noMinCostMsg_ = "Сначала найдите путь минимальной стоимости (пункт 33)";
    const char* noTSPMsg_ = "Сначала найдите TSP-цикл (пункт 53)";
    const char* noHamiltonMsg_ = "Сначала найдите Гамильтонов цикл (пункт 52)";
    const char* noEulerMsg_ = "Сначала найдите Эйлеров цикл (пункт 51)";

    std::map<int, std::function<void()>> actions_;
};

} // namespace graph
