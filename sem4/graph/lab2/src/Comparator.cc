#include "Comparator.h"

namespace graph {

using CompareResults = std::vector<Comparator::CompareResult>;

CompareResults Comparator::compareAlgorithms(Graph const& graph,
                                         int start_node,
                                         int end_node,
                                         AlgorithmNames const& algorithm_names) {
    CompareResults results;

    for (const auto& algorithm_name : algorithm_names) {
        CompareResult result;


        results.push_back(result);
    }

    return results;
}


} // namespace graph
