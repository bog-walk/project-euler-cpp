/*
 * Problem 16: Power Digit Sum
 *
 * https://projecteuler.net/problem=16
 *
 * Goal: Calculate the sum of the digits of the number 2^N.
 *
 * Constraints: 1 <= N <= 1e4
 *
 * e.g.: N = 9
 *       2^9 = 512
 *       sum = 5+1+2 = 8
 */

#include "../../doctest/doctest.h"

#include "pe-custom/big-int.h"
#include "pe-strings/utility.h"

/*
 * This solution does not handle N > 1e3 well.
 */
unsigned long expDigSumIterative(unsigned long n)
{
    BigInt power = BigInt {2uLL}.pow(BigInt {n});
    const BigInt ten = BigInt::ten(), zero = BigInt::zero();
    unsigned long total {};

    while (power != zero) {
        total += (power % ten).toULong();
        power /= ten;
    }

    return total;
}

unsigned long expDigSum(unsigned long n)
{
    const auto number = BigInt {2uLL}.pow(BigInt {n}).toString();

    return characterSum(number);
}

TEST_CASE("test lower constraints") {
    unsigned long nValues[] {1, 2, 3, 4, 7, 9};
    unsigned long expected[] {2, 4, 8, 7, 11, 8};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], expDigSumIterative(n));
        CHECK_EQ(expected[i], expDigSum(n));
    }
}

TEST_CASE("test mid constraints") {
    unsigned long nValues[] {15, 99};
    unsigned long expected[] {26, 107};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], expDigSumIterative(n));
        CHECK_EQ(expected[i], expDigSum(n));
    }
}

TEST_CASE("test upper constraints") {
    unsigned long nValues[] {1000, 10'000};
    unsigned long expected[] {1366, 13561};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], expDigSum(n));
    }
}