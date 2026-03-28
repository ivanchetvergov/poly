#pragma once

#include "../../lab1/include/Runner.h"
#include "../../lab2/include/Runner.h"
#include "../../lab3/include/Runner.h"
#include "../../lab4/include/Runner.h"
#include "../../lab5/include/Runner.h"
#include "../../lab6/include/Runner.h"

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
    Generator gen_;
    lab1::Runner lab1_runner_;
    lab2::Runner lab2_runner_;
    lab3::Runner lab3_runner_;
    lab4::Runner lab4_runner_;
    lab5::Runner lab5_runner_;
    dict::lab6::Runner lab6_runner_;

    std::map<int, std::function<void()>> actions_;
};

}  // namespace graph
