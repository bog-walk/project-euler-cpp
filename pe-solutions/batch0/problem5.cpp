/*
 * Problem 5: Smallest Multiple
 *
 * https://projecteuler.net/problem=5
 *
 * Goal: Find the smallest positive number that can be evenly divided by each
 * number in the range [1, N].
 *
 * Constraints: 1 <= N <= 40
 *
 * e.g.: N = 3
 *       {1, 2, 3} evenly divides 6 to give quotient {6, 3, 2}
 */

#include <cmath>
#include <numeric>

#include "../../doctest/doctest.h"

#include "pe-maths/primes.h"

/**
 * Repeatedly calculates the lcm of 2 values (via reduce()), starting from the largest
 * and stepping backwards until the middle of the range, as the smaller half of a range
 * will already be factors of the larger half.
 */
unsigned long long lcmOfRange(unsigned short n)
{
    auto limit = !(n & 1) ? n / 2 : n / 2 + 1;
    std::vector<unsigned short> range(limit);
    // initialise a range of decrementing values
    std::iota(range.rbegin(), range.rend(), n / 2 + 1);

    return std::accumulate(
            range.rbegin(),
            range.rend(),
            static_cast<unsigned long long>(1),
            [] (unsigned long long acc, unsigned short num) {
                return std::lcm(acc, num);
            });
}

/**
 * Uses prime numbers to calculate the lcm of a range, based on the formula:
 *
 *      p_i^a_i <= n
 *      a_i * log(p_i) <= log(n)
 *      a_i = floor(log(n) / log(p_i))
 *
 *      e.g. N = 6, primes < N = {2, 3, 5};
 *      the exponent of the 1st prime will be 2 as 2^2 < 6 but 2^3 > 6;
 *      the exponent of the 2nd prime will be 1 as 3^1 < 6 but 3^2 > 6;
 *      the exponent of the 3rd prime will be 1 as 5^1 < 6 but 5^2 > 6;
 *      therefore, lcm = 2^2 * 3^1 * 5^1 = 60.
 *
 * This is an adaptation of the prime factorisation method for calculating
 * the LCM.
 */
unsigned long long lcmOfRangeUsingPrimes(unsigned short n)
{
    unsigned long long result {1};

    for (const unsigned long& prime : primeNumbers(n)) {
        if (prime * prime > n) {
            result *= prime;
        }
        else {
            auto power = pow(prime, trunc(log2(n) / log2(prime)));
            result *= static_cast<unsigned long long>(power);
        }
    }

    return result;
}

TEST_CASE("test lower constraints") {
    unsigned short nValues[] {1, 2, 3};
    unsigned long long expected[] {1, 2, 6};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], lcmOfRange(n));
        CHECK_EQ(expected[i], lcmOfRangeUsingPrimes(n));
    }
}

TEST_CASE("test mid constraints") {
    unsigned short nValues[] {6, 10, 20};
    unsigned long long expected[] {60, 2520, 232'792'560};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], lcmOfRange(n));
        CHECK_EQ(expected[i], lcmOfRangeUsingPrimes(n));
    }
}

TEST_CASE("test upper constraints") {
    unsigned short nValues[] {30, 40};
    unsigned long long expected[] {2'329'089'562'800, 5'342'931'457'063'200};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], lcmOfRange(n));
        CHECK_EQ(expected[i], lcmOfRangeUsingPrimes(n));
    }
}