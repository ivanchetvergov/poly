#pragma once

#include <Graph.h>
#include <CollectionUtils.h>

namespace graph {

using Matrix = CollectionUtils::Matrix<double>;

class KirchhoffTheorem {
public:
    explicit KirchhoffTheorem() = default;

    [[nodiscard]] static size_t countSpanningTrees(Graph const& graph);

private:
    [[nodiscard]] static double determinant(Matrix const& matrix);
};

}  // namespace graph
