/*
 * Problem 20: Factorial Digit Sum
 *
 * https://projecteuler.net/problem=20
 *
 * Goal: Find the sum of the digits of N!
 *
 * Constraints: 0 <= N <= 1000
 *
 * e.g.: N = 10
 *       10! = 10 * 9 * ... * 2 * 1 = 3_628_800
 *       sum  = 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27
 */

#include "../../doctest/doctest.h"

#include "pe-maths/factorial.h"
#include "pe-strings/utility.h"

unsigned long factorialDigitSum(unsigned short n)
{
    auto result = factorial(n).toString();

    return characterSum(result);
}

TEST_CASE("test lower constraints") {
    unsigned long expected[] {1, 1, 2, 6};

    for (int n {0}; n < 4; ++n) {
        CHECK_EQ(expected[n], factorialDigitSum(n));
    }
}

TEST_CASE("test lower-mid constraints") {
    unsigned short nValues[] {4, 5, 6, 10};
    unsigned long expected[] {6, 3, 9, 27};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], factorialDigitSum(n));
    }
}

TEST_CASE("test upper-mid constraints") {
    unsigned short nValues[] {100, 333, 750};
    unsigned long expected[] {648, 2862, 7416};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], factorialDigitSum(n));
    }
}

TEST_CASE("test upper constraints") {
    unsigned short nValues[] {946, 1000};
    unsigned long expected[] {9675, 10539};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], factorialDigitSum(n));
    }
}