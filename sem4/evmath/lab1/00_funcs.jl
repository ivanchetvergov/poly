function lagrange_interpolate(xs::Vector{Float64}, ys::Vector{Float64}, x::Float64)
    n = length(xs)
    @assert n == length(ys)
    result = 0.0
    for i in 1:n
        term = ys[i]
        for j in 1:n
            if j != i
                term *= (x - xs[j]) / (xs[i] - xs[j])
            end
        end
        result += term
    end
    return result
end

function aitken(xs::Vector{Float64}, ys::Vector{Float64}, x::Float64)
    n = length(xs)
    @assert n == length(ys)
    P = copy(ys)
    for m in 1:n-1
        for i in 1:(n-m)
            P[i] = ((x - xs[i+m]) * P[i] + (xs[i] - x) * P[i+1]) / (xs[i] - xs[i+m])
        end
    end
    return P[1]
end

function divided_differences(xs::Vector{Float64}, ys::Vector{Float64})
    n = length(xs)
    @assert n == length(ys)
    @assert length(unique(xs)) == n "xs must be distinct"
    a = copy(ys)
    for j in 2:n
        for i in n:-1:j
            a[i] = (a[i] - a[i-1]) / (xs[i] - xs[i-j+1])
        end
    end
    return a
end

function newton_eval_from_coeffs(xs::Vector{Float64}, coeffs::Vector{Float64}, x::Float64)
    n = length(coeffs)
    result = coeffs[1]
    prod = 1.0
    for i in 2:n
        prod *= (x - xs[i-1])
        result += coeffs[i] * prod
    end
    return result
end

function newton_interpolate(xs::Vector{Float64}, ys::Vector{Float64}, x::Float64)
    coeffs = divided_differences(xs, ys)
    return newton_eval_from_coeffs(xs, coeffs, x)
end
