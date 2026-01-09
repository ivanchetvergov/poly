#pragma once

#include <string>

namespace graph {

class Animator {
public:
    static void animateFlowGrowth(
        std::string const& snapshotsFile = "assets/txt/flow_snapshots.txt",
        std::string const& outputFile = "assets/gif/flow_animation.gif",
        std::string const& graphType = "directed");
};

}  // namespace graph
