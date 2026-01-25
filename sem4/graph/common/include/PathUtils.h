#pragma once

#include <algorithm>
#include <limits>
#include <unordered_map>
#include <vector>

namespace graph {

template <typename T = double>
class PathUtils {
public:
    static constexpr T infinity() { return std::numeric_limits<T>::infinity(); }

    template <typename GetValueFunc, typename AggFunc>
    [[nodiscard]] static T calculatePathValue(int source, int sink,
                                              std::unordered_map<int, int> const& parent,
                                              GetValueFunc getValue, AggFunc agg, T initial)
    {
        T result = initial;
        for (int v = sink; v != source; v = parent.at(v)) {
            int u = parent.at(v);
            result = agg(result, getValue(u, v));
        }
        return result;
    }

    template <typename GetValueFunc>
    [[nodiscard]] static T getMinPathValue(int source, int sink,
                                           std::unordered_map<int, int> const& parent,
                                           GetValueFunc getValue) {
        return calculatePathValue(
            source, sink, parent, getValue, [](T a, T b) { return std::min(a, b); }, infinity());
    }

    template <typename Func>
    static void forEachEdgeInPath(int source, int sink, std::unordered_map<int, int> const& parent,
                                  Func func) {
        for (int v = sink; v != source; v = parent.at(v)) {
            int u = parent.at(v);
            func(u, v);
        }
    }

    [[nodiscard]] static std::vector<int> reconstructPath(
        int source, int sink, std::unordered_map<int, int> const& parent) {
        std::vector<int> path;
        for (int v = sink; v != source; v = parent.at(v)) {
            path.push_back(v);
        }
        path.push_back(source);
        std::reverse(path.begin(), path.end());
        return path;
    }

};

}  // namespace graph
