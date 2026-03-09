#include "../include/Comparator.h"

#include <BFS.h>
#include <Dijkstra.h>

namespace graph {

Comparator::CompareResults Comparator::compare(Graph const& graph, int start) {
    CompareResults results;

    {
        BFS bfs(graph);
        auto r = bfs.traverse(start);
        results.push_back({"BFS", r.iterations});
    }
    {
        Dijkstra dijk(graph);
        auto r = dijk.compute(start);
        results.push_back({"Dijkstra", r.iterations});
    }

    return results;
}

}  // namespace graph
