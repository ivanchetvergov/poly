#pragma once

#include <string>
#include <vector>

namespace config {

inline constexpr unsigned int kDescriptor = 37045;

inline const std::vector<std::string>& variables() {
    static const std::vector<std::string> vars{"x", "y", "z", "w"};
    return vars;
}

inline std::vector<int> truthVector() {
    const auto varCount = variables().size();
    const size_t size = static_cast<size_t>(1) << varCount;
    std::vector<int> result(size, 0);
    for (size_t i = 0; i < size; ++i) {
        const size_t shift = size - 1 - i;
        result[i] = (kDescriptor >> shift) & 1U;
    }
    return result;
}

}  // namespace config
