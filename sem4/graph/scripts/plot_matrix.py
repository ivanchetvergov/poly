import sys
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

if len(sys.argv) >= 3:
    input_file = sys.argv[1]
    output_file = sys.argv[2]
else:
    input_file = 'assets/txt/matrix.txt'
    output_file = 'assets/png/matrix.png'

matrix = np.loadtxt(input_file)
plt.figure(figsize=(8, 8))
sns.heatmap(matrix, annot=True,
            fmt=".2f", cmap='viridis',
            cbar=True, square=True)
plt.title('Матрица')
plt.xlabel('Вершина')
plt.ylabel('Вершина')
plt.savefig(output_file)
plt.show()
