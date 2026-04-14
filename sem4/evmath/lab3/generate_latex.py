import numpy as np

# Исходные данные
x = np.array([0.100, 0.210, 0.295, 0.348, 0.419, 0.475, 0.511, 0.555, 0.609])
y = np.array([0.0010, 0.0441, 0.0868, 0.1205, 0.1738, 0.2219, 0.2554, 0.2988, 0.0002])
n = len(x)

# Шаги интерполяции
h = np.diff(x)

# Граничные условия
m0 = (y[1] - y[0]) / h[0]
mn = (y[-1] - y[-2]) / h[-1]

# Составление системы для вычисления наклонов m_i во внутренних узлах
A = np.zeros((n, n))
b = np.zeros(n)

A[0, 0] = 1.0
b[0] = m0
A[-1, -1] = 1.0
b[-1] = mn

for i in range(1, n-1):
    a_i = h[i] / (h[i-1] + h[i])
    b_i = h[i-1] / (h[i-1] + h[i])
    c_i = 3 * a_i * ((y[i] - y[i-1]) / h[i-1]) + 3 * b_i * ((y[i+1] - y[i]) / h[i])

    A[i, i-1] = a_i
    A[i, i] = 2.0
    A[i, i+1] = b_i
    b[i] = c_i

m = np.linalg.solve(A, b)

# Вывод значений m_i
print("Значения наклонов $m_i$:\\n")
print("\\begin{align*}")
for i, mi in enumerate(m):
    print(f"m_{{{i}}} &= {mi:.4f} \\\\")
print("\\end{align*}\n")

# Вычисление коэффициентов и вывод уравнений S_i(x)
print("Уравнения кубических сплайнов $S_i(x)$ на каждом отрезке:\\n")
print("\\begin{align*}")
for i in range(n-1):
    dy = y[i+1] - y[i]

    a_coeff = y[i]
    b_coeff = m[i]
    c_coeff = (3 * dy / (h[i]**2)) - (2 * m[i] + m[i+1]) / h[i]
    d_coeff = (-2 * dy / (h[i]**3)) + (m[i] + m[i+1]) / (h[i]**2)

    # Форматирование знаков для красивого вывода
    sign_b = "+" if b_coeff >= 0 else "-"
    sign_c = "+" if c_coeff >= 0 else "-"
    sign_d = "+" if d_coeff >= 0 else "-"

    term_a = f"{a_coeff:.4f}"
    term_b = f"{sign_b} {abs(b_coeff):.4f}(x - {x[i]:.3f})"
    term_c = f"{sign_c} {abs(c_coeff):.4f}(x - {x[i]:.3f})^2"
    term_d = f"{sign_d} {abs(d_coeff):.4f}(x - {x[i]:.3f})^3"

    print(f"S_{{{i}}}(x) &= {term_a} {term_b} {term_c} {term_d} \\\\")
print("\\end{align*}")
