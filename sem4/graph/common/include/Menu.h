#pragma once

#include "../../lab1/include/Runner.h"
#include "../../lab3/include/Runner.h"
#include "../../lab4/include/Runner.h"
#include "../../lab5/include/Runner.h"

#include <functional>
#include <iostream>
#include <map>
#include <memory>

#include <Generator.h>
#include <Graph.h>
#include <Utils.h>
#include <Visualizer.h>

namespace graph {

class Menu {
public:
    Menu();
    void show() const;
    void handleChoice(int choice);

private:
    void initializeActions();
    std::unique_ptr<Graph> graph_;
    std::unique_ptr<FlowNetwork> flow_net_;
    Generator gen_;
    lab1::Runner lab1_runner_;
    lab3::Runner lab3_runner_;
    lab4::Runner lab4_runner_;
    lab5::Runner lab5_runner_;
    double last_max_flow_ = 0.0;

    char const* no_graph_msg_ = "Сначала сгенерируйте граф (пункт 1)";
    char const* no_flow_msg_ = "Сначала сгенерируйте сеть потоков (пункт 31)";
    char const* no_max_flow_msg_ = "Сначала найдите максимальный поток (пункт 32)";
    char const* no_tsp_msg_ = "Сначала найдите TSP-цикл (пункт 53)";
    char const* no_hamilton_msg_ = "Сначала найдите Гамильтонов цикл (пункт 52)";
    char const* no_euler_msg_ = "Сначала найдите Эйлеров цикл (пункт 51)";

    std::map<int, std::function<void()>> actions_;
};

}  // namespace graph
