include(joinpath(@__DIR__, "00_funcs.jl"))

xs = [11.153, 11.454, 11.673, 11.879, 12.009, 12.231, 12.549]
ys = [-3.234, 5.321, -1.123, 0.393, 8.939, 141.231, 15.001]
xinterp = 12.721

lag = lagrange_interpolate(xs, ys, xinterp)
ait = aitken(xs, ys, xinterp)
newton = newton_interpolate(xs, ys, xinterp)

println("interpolation point x = ", xinterp)
println("lagrange: ", lag)
println("aitken:   ", ait)
println("newton:   ", newton)
println("diff (L - A): ", lag - ait)
println("diff (L - N): ", lag - newton)
println("diff (A - N): ", ait - newton)

