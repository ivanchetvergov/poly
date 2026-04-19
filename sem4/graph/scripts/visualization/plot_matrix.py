import sys
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

from ..core.config import matrix_cfg


def main():
    if len(sys.argv) >= 4:
        input_file = sys.argv[1]
        output_file = sys.argv[2]
        title = sys.argv[3]
        hide_offdiag_zeros = (len(sys.argv) >= 5 and sys.argv[4] == '1')
    else:
        input_file = 'assets/txt/matrix.txt'
        output_file = 'assets/png/matrix.png'
        title = 'Матрица'
        hide_offdiag_zeros = False

    matrix = np.loadtxt(input_file)
    matrix = np.atleast_2d(matrix)

    # Optionally hide only text labels for off-diagonal zeros.
    annot = np.empty(matrix.shape, dtype=object)
    diag_mask = np.eye(matrix.shape[0], dtype=bool)
    for i in range(matrix.shape[0]):
        for j in range(matrix.shape[1]):
            if hide_offdiag_zeros and matrix[i, j] == 0 and not diag_mask[i, j]:
                annot[i, j] = ''
            else:
                annot[i, j] = f"{matrix[i, j]:.2f}"

    plt.figure(figsize=matrix_cfg.matrix_figsize)

    sns.heatmap(
        matrix,
        annot=annot,
        fmt="",
        cmap=matrix_cfg.matrix_colormap,
        cbar=True,
        square=True,
        annot_kws={'size': matrix_cfg.matrix_annot_size},
        linewidths=0.5,
        linecolor='gray'
    )

    plt.title(title, fontsize=matrix_cfg.matrix_title_size, fontweight='bold', pad=20)
    plt.xlabel('Вершина', fontsize=12, fontweight='bold')
    plt.ylabel('Вершина', fontsize=12, fontweight='bold')
    plt.tight_layout()
    plt.savefig(output_file, bbox_inches='tight')
    plt.show()
    plt.close()


if __name__ == '__main__':
    main()
