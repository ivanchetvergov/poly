#pragma once

#include "Visualizer.h"
#include <map>

namespace graph {

class DrawDataConfig {
public:
    static const std::map<int, DrawData>& getConfigs();

private:
    static std::map<int, DrawData> configs_;
};

}  // namespace graph
