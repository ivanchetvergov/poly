#pragma once

#include <string>

namespace graph {

class Animator {
public:
    static void animateFlowGrowth(
        std::string const& snapshotsFile = "assets/txt/32_flow_snapshots.txt",
        std::string const& outputFile = "assets/gif/37_flow_animation.gif",
        std::string const& graphType = "directed");
};

}  // namespace graph
