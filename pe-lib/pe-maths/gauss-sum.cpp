#include "gauss-sum.h"

#include "../../doctest/doctest.h"

/**
 * Calculates the sum of the first n natural numbers, based on the formula:
 *
 *      {n}Sigma{k=1} k = n * (n + 1) / 2
 *
 * Conversion of large long double types to long long types in this formula can lead to
 * large rounding losses, so integer division by 2 is replaced with a single bitwise
 * right shift, as n >> 1 = n / (2^1).
 */
unsigned long long gaussSum(unsigned long long n)
{
    return n * (n + 1) >> 1;
}

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