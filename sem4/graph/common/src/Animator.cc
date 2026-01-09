#include "Animator.h"

#include "Utils.h"

#include <iostream>

namespace graph {

void Animator::animateFlowGrowth(std::string const& snapshotsFile, std::string const& outputFile,
                                 std::string const& graphType) {
    std::vector<std::string> args = {"--input",      snapshotsFile, "--output", outputFile,
                                     "--graph_type", graphType,     "--show"};
    runPythonScript("plot_flow_animation.py", args);
}

}  // namespace graph
