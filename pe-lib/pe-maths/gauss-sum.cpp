#include "gauss-sum.h"

#include "../../doctest/doctest.h"

TEST_SUITE("test gaussSum()") {
    TEST_CASE("with lower constraints") {
        unsigned long long nValues[] {1, 2, 3};
        unsigned long long expected[] {1, 3, 6};

        for (const auto& n : nValues) {
            auto i = &n - &nValues[0];
            CHECK_EQ(expected[i], gaussSum(n));
        }
    }

    TEST_CASE("with mid constraints") {
        unsigned long long nValues[] {50, 100, 2234};
        unsigned long long expected[] {1275, 5050, 2'496'495};

        for (const auto& n : nValues) {
            auto i = &n - &nValues[0];
            CHECK_EQ(expected[i], gaussSum(n));
        }
    }
}