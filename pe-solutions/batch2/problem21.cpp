/*
 * Problem 21: Amicable Numbers
 *
 * https://projecteuler.net/problem=21
 *
 * Goal: Return the sum of all amicable numbers less than N.
 *
 * Constraints: 1 <= N <= 1e5
 *
 * Proper Divisor: A number x that evenly divides N, where x != N.
 *
 * Amicable Number: A number X that has a pair Y, where X != Y but
 * d(X) = Y && d(Y) = X, with d(N) = sum{proper divisors of N}.
 * First amicable pairs = {(220, 284), (1184, 1210), (2620, 2924)}
 *
 * e.g.: N = 300
 *       amicable pairs = [{220, 284}]; since
 *       d(220) = sum{1,2,4,5,10,11,20,22,44,55,110} = 284
 *       d(284) = sum{1,2,4,71,142} = 220
 *       sum = 220 + 284 = 504
 */

#include <numeric>
#include <vector>

#include "../../doctest/doctest.h"

#include "pe-maths/sum-proper-divisors.h"

unsigned long sumAmicablePairs(unsigned long n)
{
    std::vector<unsigned long> amicableNums;

    for (int x {2}; x < n; ++x) {
        auto y = sumProperDivisors(x);
        // the partner of a newly explored amicable number must be larger
        if (y > x && sumProperDivisors(y) == x) {
            amicableNums.push_back(x);
            // account for possibility that only 1 of the amicable pair may be under N
            if (y < n)
                amicableNums.push_back(y);
            else
                break;  // future pairs will be > N
        }
    }

    return std::accumulate(amicableNums.begin(),amicableNums.end(),0uL);
}

TEST_CASE("test lower constraints") {
    unsigned long nValues[] {1, 100};
    unsigned long expected {0};

    for (const auto& n: nValues) {
        CHECK_EQ(expected, sumAmicablePairs(n));
    }
}

TEST_CASE("test constraints based on pairs") {
    // 1st pair (220, 284), 2nd (1194, 1210), 3rd (2620, 2924)
    unsigned long nValues[] {300, 2000, 5000};
    unsigned long expected[] {504, 2898, 8442};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumAmicablePairs(n));
    }
}

TEST_CASE("test upper constraints") {
    unsigned long nValues[] {10'000, 40'000, 100'000};
    unsigned long expected[] {31626, 115'818, 852'810};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumAmicablePairs(n));
    }
}