#include "extension.h"

#include <algorithm>
#include <numeric>

#include "../../doctest/doctest.h"

TEST_SUITE("test getMap() functions") {
    const int exampleMax {10};
    std::map<int, double> example;

    TEST_CASE("setup example") {
        for (int i {0}; i < exampleMax; ++i) {
            example[i] = 1.1 * i;
        }
    }

    TEST_CASE("return empty list when map is empty") {
        std::map<int, int> emptyMap;

        CHECK(emptyMap.empty());
        CHECK(getMapKeys(emptyMap).empty());
        CHECK(getMapValues(emptyMap).empty());
    }

    TEST_CASE("getMapKeys() correct") {
        std::vector<int> expected(exampleMax);
        std::iota(expected.begin(), expected.end(), 0);

        auto actual = getMapKeys(example);

        CHECK_EQ(expected, actual);
    }

    TEST_CASE("getMapValues() correct") {
        std::vector<double> expected(exampleMax);
        int i {};
        std::generate_n(
                expected.begin(),
                exampleMax,
                [&i] {
                    return 1.1 * i++;
                });

        auto actual = getMapValues(example);

        CHECK_EQ(expected, actual);
    }
}