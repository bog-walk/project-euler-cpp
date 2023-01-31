#ifndef PROJECT_EULER_CPP_CARTESIAN_PRODUCT_H
#define PROJECT_EULER_CPP_CARTESIAN_PRODUCT_H

#include <algorithm>
#include <vector>

/*
 * Returns Cartesian product of elements.
 *
 * To return the product of an iterable with itself, specify the number of repetitions
 * using repeat.
 *
 * If elements is sorted, product lists will be returned in lexicographic order.
 *
 * e.g. elements = "ABCD", "xy" -> Ax Ay Bx By Cx Cy Dx Dy
 * e.g. elements = "AB", repeat = 2 -> AA AB BA BB
 */
template <typename T>
std::vector<std::vector<T>> product(std::vector<std::vector<T>>& elements,
                                    int repeat = 1)
{
    // cartesian product of any non-empty set with an empty set gives an empty set
    if (elements.empty() ||
        std::find_if(elements.cbegin(), elements.cend(),
                    [](auto& nested) { return nested.empty(); }) != elements.cend())
        return {};

    std::vector<std::vector<T>> inputs;
    for (int i {0}; i < repeat; ++i) {
        for (auto& e : elements) {
            inputs.push_back(e);
        }
    }

    std::vector<std::vector<T>> results;
    for (auto& input : inputs[0]) {
        results.push_back({ input });
    }
    for (int i {1}; i < inputs.size(); ++i) {
        std::vector<std::vector<T>> temp;
        for (auto& nested : results) {
            for (auto& next : inputs[i]) {
                auto t = nested;
                t.push_back(next);
                temp.push_back(t);
            }
        }
        results = temp;
    }

    return results;
}

#endif //PROJECT_EULER_CPP_CARTESIAN_PRODUCT_H