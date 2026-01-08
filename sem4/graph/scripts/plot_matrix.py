import sys
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

from config import *

def main():
    if len(sys.argv) >= 4:
        input_file = sys.argv[1]
        output_file = sys.argv[2]
        title = sys.argv[3]
    else:
        input_file = 'assets/txt/matrix.txt'
        output_file = 'assets/png/matrix.png'
        title = 'Матрица'

    matrix = np.loadtxt(input_file)

    plt.figure(figsize=MATRIX_FIGSIZE)

    sns.heatmap(
        matrix,
        annot=True,
        fmt=".2f",
        cmap=MATRIX_COLORMAP,
        cbar=True,
        square=True,
        annot_kws={'size': MATRIX_ANNOT_SIZE},
        linewidths=0.5,
        linecolor='gray'
    )

    plt.title(title, fontsize=MATRIX_TITLE_SIZE, fontweight='bold', pad=20)
    plt.xlabel('Вершина', fontsize=12, fontweight='bold')
    plt.ylabel('Вершина', fontsize=12, fontweight='bold')
    plt.tight_layout()
    plt.savefig(output_file, bbox_inches='tight')
    plt.show()
    plt.close()


if __name__ == '__main__':
    main()
