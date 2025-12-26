import LinearAlgebra

function linear_reg(xs::Vector{Float64}, ys::Vector{Float64})
    n = length(xs)
    @assert n == length(ys)

    X = hcat(xs, ones(n))
    beta = X \ ys
    return beta 
end

function quadratic_reg(xs::Vector{Float64}, ys::Vector{Float64})
    n = length(xs)
    X = hcat(xs.^2, xs, ones(n))
    beta = X \ ys
    return beta 
end

function rmse(actual::Vector{Float64}, predicted::Vector{Float64})
    return sqrt(sum((actual .- predicted).^2) / length(actual))
end