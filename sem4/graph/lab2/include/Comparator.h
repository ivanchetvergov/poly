#pragma once

#include <string>
#include <vector>

#include <Graph.h>

namespace graph {

class Comparator {
public:
    struct CompareResult {
        std::string algorithm_name;
        std::vector<int> path;
        double time_taken;
        size_t memory_used;
        size_t iterations;
    };

    using CompareResults = std::vector<CompareResult>;
    using AlgorithmNames = std::vector<std::string>;

    CompareResults compareAlgorithms(Graph const& graph,
                                     int start_node,
                                     int end_node,
                                     AlgorithmNames const& algorithm_names);
};

}  // namespace graph
