/*
 * Problem 6: Sum Square Difference
 *
 * https://projecteuler.net/problem=6
 *
 * Goal: Find the absolute difference between the sum of the squares & the
 * square of the sum of the first N natural numbers.
 *
 * Constraints: 1 <= N <= 1e4
 *
 * e.g.: N = 3 -> {1,2,3}
 *       sum of squares = {1,4,9} = 14
 *       square of sum = 6^2 = 36
 *       diff = |14 - 36| = 22
 */

#include <numeric>
#include <vector>

#include "../doctest/doctest.h"

#include "pe-maths/gauss-sum.h"

unsigned long long sumSquareDiffBrute(unsigned short n)
{
    // start range at 2, as 1 will be used as the accumulator's initial value
    std::vector<unsigned short> range(n - 1);
    std::iota(range.begin(), range.end(), 2);

    auto [sumOfRange, sumOfSquares] = std::accumulate(
            range.cbegin(),
            range.cend(),
            std::pair<unsigned long long, unsigned long long> {1, 1},
            [] (std::pair<unsigned long long, unsigned long long> acc,
                    unsigned short num){
                return std::pair {acc.first + num, acc.second + num * num};
            });

    return sumOfRange * sumOfRange - sumOfSquares;
}

/**
 * The sum of the 1st [n] natural numbers (triangular numbers) is found using the gauss
 * summation method.
 *
 * The sum of the sequence's squares is based on the assumption that:
 *
 *      f(n) = an^3 + bn^2 + cn + d,
 *      with f(0) = 0, f(1) = 1, f(2) = 5, f(3) = 14
 *
 * The formula (square pyramidal numbers) can then be solved as:
 *
 *      f(n) = (2n^3 + 3n^2 + n)/6
 *      f(n) = (n(2n + 1)(n + 1))/6
 */
unsigned long long sumSquareDiff(unsigned short n) {
    auto sumOfRange = gaussSum(n);

    auto sumOfSquares = n / 6.0 * (2 * n + 1) * (n + 1);

    return sumOfRange * sumOfRange - static_cast<unsigned long long>(sumOfSquares);
}

TEST_CASE("test lower constraints") {
    unsigned short nValues[] {1, 2, 3};
    unsigned long long expected[] {0, 4, 22};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumSquareDiffBrute(n));
        CHECK_EQ(expected[i], sumSquareDiff(n));
    }
}

TEST_CASE("test mid constraints") {
    unsigned short nValues[] {10, 51, 100};
    unsigned long long expected[] {2640, 1'712'750, 25'164'150};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumSquareDiffBrute(n));
        CHECK_EQ(expected[i], sumSquareDiff(n));
    }
}

TEST_CASE("test upper constraints") {
    unsigned short nValues[] {2256, 7000, 10'000};
    unsigned long long expected[] {6'477'756'566'600, 600'307'154'415'500,
                                               2'500'166'641'665'000};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumSquareDiffBrute(n));
        CHECK_EQ(expected[i], sumSquareDiff(n));
    }
}