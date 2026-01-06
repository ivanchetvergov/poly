#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace graph {

class CollectionUtils {
public:
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

} // namespace graph
