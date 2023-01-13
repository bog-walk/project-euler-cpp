#ifndef PROJECT_EULER_CPP_EXTENSION_H
#define PROJECT_EULER_CPP_EXTENSION_H

#include <map>
#include <vector>

template <typename K, typename V>
std::vector<K> getMapKeys(const std::map<K, V>& inputMap)
{
    std::vector<K> keys;
    keys.reserve(inputMap.size());

    for (const auto& [k, _] : inputMap) {
        keys.push_back(k);
    }

    return keys;
}

template <typename K, typename V>
std::vector<V> getMapValues(const std::map<K, V>& inputMap)
{
    std::vector<V> values;
    values.reserve(inputMap.size());

    for (const auto& [_, v] : inputMap) {
        values.push_back(v);
    }

    return values;
}

#endif //PROJECT_EULER_CPP_EXTENSION_H