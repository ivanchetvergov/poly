import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
from sklearn.metrics import mean_squared_error
import os

xs = np.array([11.153, 11.454, 11.673, 11.879, 12.009, 12.231, 12.549]).reshape(-1, 1)
ys = np.array([-3.234, 5.321, -1.123, 0.393, 8.939, 141.231, 15.001])

model_lin = LinearRegression().fit(xs, ys)
y_pred_lin = model_lin.predict(xs)
rmse_lin = np.sqrt(mean_squared_error(ys, y_pred_lin))

poly = PolynomialFeatures(degree=2)
xs_poly = poly.fit_transform(xs)
model_quad = LinearRegression().fit(xs_poly, ys)
y_pred_quad = model_quad.predict(xs_poly)
rmse_quad = np.sqrt(mean_squared_error(ys, y_pred_quad))

print("--- Linear ---")
print(f"y = {model_lin.coef_[0]:.4f}x + {model_lin.intercept_:.4f}")
print(f"RMSE: {rmse_lin:.4f}")

print("\n--- Quadratic ---")
print(f"y = {model_quad.coef_[2]:.4f}x^2 + {model_quad.coef_[1]:.4f}x + {model_quad.intercept_:.4f}")
print(f"RMSE: {rmse_quad:.4f}")

plt.figure(figsize=(10, 6))

plt.scatter(xs, ys, color='black', label='Original Data', zorder=5)

x_range = np.linspace(xs.min() - 0.1, xs.max() + 0.1, 200).reshape(-1, 1)
y_range_lin = model_lin.predict(x_range)
plt.plot(x_range, y_range_lin, color='blue', linestyle='--', label=f'Linear (RMSE: {rmse_lin:.2f})')

x_range_poly = poly.transform(x_range)
y_range_quad = model_quad.predict(x_range_poly)
plt.plot(x_range, y_range_quad, color='red', label=f'Quadratic (RMSE: {rmse_quad:.2f})')

plt.title('Linear vs Quadratic Regression')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid(True, alpha=0.3)

output_path = os.path.join(os.path.dirname(__file__), 'regression_plot.png')
plt.savefig(output_path)
print(f"\nsaved in: {output_path}")

