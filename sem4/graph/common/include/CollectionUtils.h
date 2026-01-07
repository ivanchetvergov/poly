#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <optional>

namespace graph {

class CollectionUtils {
public:

    template<typename T>
    using Matrix = std::vector<std::vector<T>>;

    template<typename T, typename Getter>
    static Matrix<T> makeMatrix(const std::vector<int>& rows,
        const std::vector<int>& cols, Getter getter)
    {
        Matrix<T> matrix(rows.size(), std::vector<T>(cols.size()));

        for (size_t i = 0; i < rows.size(); ++i) {
            for (size_t j = 0; j < cols.size(); ++j) {
                matrix[i][j] = getter(rows[i], cols[j]);
            }
        }
        return matrix;
    }

    template<typename CompareFunc>
    static Matrix<std::optional<double>> multiplyOptionalMatrix(
        const Matrix<std::optional<double>>& a,
        const Matrix<std::optional<double>>& b,
        CompareFunc compare)
    {
        int size = static_cast<int>(a.size());
        Matrix<std::optional<double>> result(size, std::vector<std::optional<double>>(size, std::nullopt));

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                for (int k = 0; k < size; ++k) {
                    if (a[i][k].has_value() && b[k][j].has_value()) {
                        double distance = a[i][k].value() + b[k][j].value();

                        if (!result[i][j].has_value() || compare(distance, result[i][j].value())) {
                            result[i][j] = distance;
                        }
                    }
                }
            }
        }
        return result;
    }

    template<typename T>
    static void addUnique(std::vector<T>& vec, const T& value) {
        if (std::find(vec.begin(), vec.end(), value) == vec.end()) {
            vec.push_back(value);
        }
    }

    template<typename Map, typename Key, typename Value = double>
    static Value getNestedMapValue(
        const Map& map,
        const Key& key1,
        const Key& key2,
        const Value& defaultValue = Value{})
    {
        auto it1 = map.find(key1);
        if (it1 == map.end()) return defaultValue;
        auto it2 = it1->second.find(key2);
        return (it2 == it1->second.end()) ? defaultValue : it2->second;
    }

    template<typename Map, typename Key>
    static bool hasKey(const Map& map, const Key& key) {
        return map.find(key) != map.end();
    }
};

template<typename Map, typename Key>
static bool hasKey(const Map& map, const Key& key) {
    return map.find(key) != map.end();
}

} // namespace graph
