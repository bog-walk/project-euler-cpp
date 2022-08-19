/*
 * Problem 1: Multiples of 3 or 5
 *
 * https://projecteuler.net/problem=1
 *
 * Goal: Find the sum of all natural numbers less than N that are multiples of either
 * of the provided factors K1 or K2.
 *
 * Constraints: 1 <= N <= 1e9, 1 <= K < N
 *
 * e.g.: N = 10, K1 = 3, K2 = 5
 *       multiples of K1 || K2 < N = {3, 5, 6, 9}
 *       sum = 23
 */

#include <algorithm>
#include <numeric>

#include "../../doctest/doctest.h"

#include "pe-maths/gauss-sum.h"

/**
 * Brute iteration through all numbers < [n] that checks for selector predicate.
 */
unsigned long long sumOfMultiplesBrute(unsigned long n, unsigned long factor1,
                                       unsigned long factor2)
{
    unsigned long long sum {};

    for (auto num = std::min(factor1, factor2); num < n; ++num) {
        sum += !(num % factor1) || !(num % factor2) ? num : 0;
    }

    return sum;
}

/**
 * Calculates the sum of an arithmetic progression sequence.
 *
 * Solution based on the formula:
 *
 *      S_n = {n}Sigma{k=1} a + (k-1)d = n(2a + (n - 1)d)/2,
 *
 * where a is the 1st term, d is the delta, and n is the amount of terms to add.
 *
 * a and d are the same in this case, so the formula becomes:
 *
 *      S_n = (n(n + 1)d)/2
 *
 * Note that this is an adapted Gaussian sum (triangular number) formula, where n is
 * replaced with the amount of terms that are evenly divisible by d, then the original
 * formula is multiplied by d.
 */
unsigned long long sumOfArithProgression(unsigned long maxTerm, unsigned long delta)
{
    return gaussSum(maxTerm / delta) * delta;
}

/**
 * Calculates the sum of multiples of both factors minus the sum of duplicates found
 * via the least common multiple of the given factors.
 */
unsigned long long sumOfMultiples(unsigned long n, unsigned long factor1,
                                  unsigned long factor2)
{
    const auto maxTerm = n - 1;

    if (factor1 == factor2)
        return sumOfArithProgression(maxTerm, factor1);

   return sumOfArithProgression(maxTerm, factor1) +
       sumOfArithProgression(maxTerm, factor2) -
       sumOfArithProgression(maxTerm, std::lcm(factor1,factor2));
}

TEST_CASE("test lower constraints for N") {
    unsigned long nValues[] {1, 2, 2, 3, 4};
    unsigned long k1Values[] {1, 1, 1, 1, 2};
    unsigned long k2Values[] {1, 2, 1, 2, 3};
    unsigned long long expected[] {0, 1, 1, 3, 5};

    for (const auto& n : nValues) {
        // use pointer arithmetic to get index
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumOfMultiplesBrute(n, k1Values[i],
                                                  k2Values[i]));
        CHECK_EQ(expected[i], sumOfMultiples(n, k1Values[i],k2Values[i]));
    }
}

TEST_CASE("test lower constraints for K") {
    unsigned long n {20}, k1 {1}, k2 {2};
    unsigned long long expected {190};

    CHECK_EQ(expected, sumOfMultiplesBrute(n, k1, k2));
    CHECK_EQ(expected, sumOfMultiples(n, k1, k2));
}

TEST_CASE("test when K1 == K2") {
    unsigned long n {10}, k {3};
    unsigned long long expected {18};

    CHECK_EQ(expected, sumOfMultiplesBrute(n, k, k));
    CHECK_EQ(expected, sumOfMultiples(n, k, k));
}

TEST_CASE("test mid constraints") {
    unsigned long nValues[] {10, 100, 1000};
    unsigned long k1 {3}, k2 {5};
    unsigned long long expected[] {23, 2318, 233'168};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumOfMultiplesBrute(n, k1,k2));
        CHECK_EQ(expected[i], sumOfMultiples(n, k1,k2));
    }
}

TEST_CASE("test upper constraints") {
    unsigned long nValues[] {23000, 10'000'000, 10'000'000, 1'000'000'000};
    unsigned long k1Values[] {8, 20, 3, 3};
    unsigned long k2Values[] {21, 32, 5, 5};
    unsigned long long expected[] {44'087'172, 3'749'995'000'000,
                                   23'333'331'666'668,
                                   233'333'333'166'666'668};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumOfMultiplesBrute(n, k1Values[i],k2Values[i]));
        CHECK_EQ(expected[i], sumOfMultiples(n, k1Values[i],k2Values[i]));
    }
}