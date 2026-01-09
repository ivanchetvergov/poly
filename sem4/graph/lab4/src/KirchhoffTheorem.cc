#include "KirchhoffTheorem.h"

#include <CollectionUtils.h>

namespace graph {

using Matrix = CollectionUtils::Matrix<double>;

size_t KirchhoffTheorem::countSpanningTrees(Graph const& graph) {
    size_t n = graph.vertexCount();
    if (n <= 1) {
        return 0;
    }

    Matrix laplacian = CollectionUtils::makeMatrix<double>(
        graph.vertexIds(), graph.vertexIds(),
        [&graph](int i, int j){
            if (i == j){
                return static_cast<double>(graph.degree(i));
            } else {
                return graph.hasEdge(i, j) ? -1.0 : 0.0;
            }
        });

    Matrix minor(n - 1, std::vector<double>(n - 1));
    for (size_t i = 1; i < n; ++i) {
        for (size_t j = 1; j < n; ++j) {
            minor[i - 1][j - 1] = laplacian[i][j];
        }
    }

    return static_cast<size_t>(KirchhoffTheorem::determinant(minor));
}

double KirchhoffTheorem::determinant(Matrix const& matrix) {
    size_t n = matrix.size();
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    double det = 0.0;
    for (size_t p = 0; p < n; ++p) {
        Matrix submatrix(n - 1, std::vector<double>(n - 1));
        for (size_t i = 1; i < n; ++i) {
            size_t col = 0;
            for (size_t j = 0; j < n; ++j) {
                if (j != p) {
                    submatrix[i - 1][col++] = matrix[i][j];
                }
            }
        }
        double cofactor = (p % 2 == 0 ? 1 : -1) * matrix[0][p];
        det += cofactor * determinant(submatrix);
    }
    return det;
}

}  // namespace lab4
