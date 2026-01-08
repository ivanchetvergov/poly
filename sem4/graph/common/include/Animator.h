#pragma once

#include <string>

namespace graph {

class Animator {
public:
    static void animateFlowGrowth(
        const std::string& snapshotsFile = "assets/txt/flow_snapshots.txt",
        const std::string& outputFile = "assets/gif/flow_animation.gif",
        const std::string& graphType = "directed");
};

} // namespace graph
