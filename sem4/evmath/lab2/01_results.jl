include("00_funcs.jl")

xs = [11.153, 11.454, 11.673, 11.879, 12.009, 12.231, 12.549]
ys = [-3.234, 5.321, -1.123, 0.393, 8.939, 141.231, 15.001]

a_lin, b_lin = linear_reg(xs, ys)
y_pred_lin = a_lin .* xs .+ b_lin
err_lin = rmse(ys, y_pred_lin)

a_quad, b_quad, c_quad = quadratic_reg(xs, ys)
y_pred_quad = a_quad .* xs.^2 .+ b_quad .* xs .+ c_quad
err_quad = rmse(ys, y_pred_quad)

println("--- Linear ---")
println("y = $(round(a_lin, digits=4))x + $(round(b_lin, digits=4))")
println("RMSE: $(round(err_lin, digits=4))")

println("\n--- Quadratic ---")
println("y = $(round(a_quad, digits=4))x^2 + $(round(b_quad, digits=4))x + $(round(c_quad, digits=4))")
println("RMSE: $(round(err_quad, digits=4))")