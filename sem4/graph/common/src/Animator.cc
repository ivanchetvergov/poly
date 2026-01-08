#include "Animator.h"
#include "Utils.h"
#include <iostream>

namespace graph {

void Animator::animateFlowGrowth(const std::string& snapshotsFile,
    const std::string& outputFile, const std::string& graphType)
{
    std::vector<std::string> args = {"--input", snapshotsFile, "--output", outputFile, "--graph_type", graphType, "--show"};
    runPythonScript("plot_flow_animation.py", args);
}

} // namespace graph
