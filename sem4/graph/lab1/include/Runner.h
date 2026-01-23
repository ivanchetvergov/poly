#pragma once

#include "Graph.h"
#include "ShimbellMethod.h"
#include "PathCounter.h"

#include <memory>
#include <optional>
#include <vector>

namespace lab1 {

using graph::Graph;

class Runner {
public:
    Runner() = default;

    void runShimbellMethod(Graph const& graph);
    void runPathsMethod(Graph const& graph);

private:
};

}  // namespace lab1
