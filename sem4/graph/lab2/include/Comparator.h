#pragma once

#include <string>
#include <vector>

#include <Graph.h>

namespace graph {

class Comparator {
public:
    struct CompareResult {
        std::string algorithm_name;
        size_t      iterations{0};
    };

    using CompareResults = std::vector<CompareResult>;

    [[nodiscard]] static CompareResults compare(Graph const& graph, int start);
};

}  // namespace graph
