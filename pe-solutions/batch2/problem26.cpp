/*
 * Problem 26: Reciprocal Cycles
 *
 * https://projecteuler.net/problem=26
 *
 * Goal: Find the value of the smallest d less than N for which 1/d contains the longest
 * recurring cycle in its decimal fraction part.
 *
 * Constraints: 4 <= N <= 1e4
 *
 * Repetend/Reptend: The infinitely repeated digit sequence of the decimal
 * representation of a number.
 *
 * e.g.: N = 10
 *       1/2 = 0.5
 *       1/3 = 0.(3) -> 1-digit recurring cycle
 *       1/4 = 0.25
 *       1/5 = 0.2
 *       1/6 = 0.1(6) -> 1-digit recurring cycle
 *       1/7 = 0.(142857) -> 6-digit recurring cycle
 *       1/8 = 0.125
 *       1/9 = 0.(1) -> 1-digit recurring cycle
 *       result = 7
 */

#include "../../doctest/doctest.h"

#include "pe-custom/big-int.h"
#include "pe-maths/primes.h"

/*
 * Solution based on the following:
 *
 *  -   If a fraction contains a repetend, the latter's length (K) will never be
 *  greater than the fraction's denominator minus 1.
 *
 *  -   A denominator of 3 produces the first repetend, with K = 1.
 *
 *  -   All fractions that are a power of 1/2 or the product of 1/5 times a power of
 *  1/2 will have exact decimal equivalents, not repetends.
 *
 *  -   Multiples of a denominator will have same K value (multiples of 7 are special
 *  in that both K and repetend will be equal).
 *
 *  -   For each 1/p, where p is a prime number but not 2 or 5, for k in [1, p), 10^k %
 *  p produces a repetend, when the remainder is 1.
 *
 *          e.g. p = 11 -> (10^1 - 1) % 11 != 0, but (10^2 - 1) / 11 has 99 evenly
 *          divided by 11 giving 9. Since k = 2, there must be 2 repeating digits, so
 *          repetend = 09.
 *
 * N.B. This solution does not tolerate N >1e3 well.
 */
unsigned long longestRepetendDenomUsingPrimes(unsigned long n)
{
    if (n < 8)
        return 3;

    // only primes considered as only the smallest N is required & anything larger
    // would be a multiple of a smaller prime with equivalent K
    const auto allPrimes = primeNumbers(n - 1);
    // could use std::set_difference() with std::back_inserter(delta)
    const std::vector<unsigned long> primes(allPrimes.cbegin() + 3, allPrimes.cend());

    unsigned long denominator {3}, longestK {1};
    BigInt one {BigInt::one()}, ten {BigInt::ten()};

    for (auto& p : primes) {
        BigInt pBI {1uLL * p};
        for (int k {1}; k < p; ++k) {
            if (ten.modPow(BigInt {1uLL * k}, pBI) == one) {
                if (k > longestK) {
                    longestK = k;
                    denominator = p;
                }
                break;
            }
        }
    }

    return denominator;
}

/*
 * Solution using primes above is optimised based on the following:
 *
 *  -   Full Repetend Primes are primes that, as 1/p, will have the longest repetend of
 *  k = p - 1. A prime qualifies if, for k in [1, p-1], only the last k returns True
 *  for 10^k % p = 1.
 *
 *          e.g. p = 7 -> for k in [1, 7), 10^k % p = [3, 2, 6, 4, 5, 1], so 7 is a
 *          full repetend prime.
 *
 *  -   Other than N = 3 and N = 6 both having K = 1, repetend length increases as
 *  primes increase since the longest repetends will be produced by full repetend
 *  primes & not be repeated. So the loop can be started from the largest prime and
 *  broken once the first full repetend prime is found.
 */
unsigned long longestRepetendDenomUsingPrimesImproved(unsigned long n)
{
    if (n < 8)
        return 3;

    const auto allPrimes = primeNumbers(n - 1);
    const std::vector<unsigned long> primes(allPrimes.rbegin(), allPrimes.rend());

    unsigned long denominator {3};
    BigInt one {BigInt::one()}, ten {BigInt::ten()};

    for (auto& p : primes) {
        unsigned long k {1};
        BigInt pBI {1uLL * p};
        while (ten.modPow(BigInt {1uLL * k}, pBI) != one) {
            k++;
        }
        if (k == p - 1) {
            denominator = p;
            break;
        }
    }

    return denominator;
}

/*
 * Repeatedly divides & stores decimal parts until a decimal part is repeated &
 * compares length of stored parts.
 */
unsigned long longestRepetendDenominator(unsigned long n)
{
    const auto upperN = n & 1 ? n - 2 : n - 1;
    const auto lowerN = upperN / 2;
    unsigned long longestK {};
    std::vector<unsigned long> remainders;

    for (unsigned long i {upperN}; i >= lowerN; i -= 2) {
        if (longestK >= i)
            break;
        // better to just initialise a fresh local vector every iteration?
        remainders.resize(i);
        std::fill(remainders.begin(), remainders.end(), 0uLL);
        unsigned long remainder {1}, pos {};
        while (!remainders[remainder] && remainder) {
            remainders[remainder] = pos++;
            remainder = remainder * 10 % i;
        }
        if (pos - remainders[remainder] >= longestK) {
            longestK = pos - remainders[remainder];
            n = i;
        }
    }

    return n;
}

TEST_CASE("test lower constraints") {
    unsigned long nValues[] {5, 10, 14, 19, 25};
    unsigned long expected[] {3, 7, 7, 17, 23};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], longestRepetendDenomUsingPrimes(n));
        CHECK_EQ(expected[i], longestRepetendDenomUsingPrimesImproved(n));
        CHECK_EQ(expected[i], longestRepetendDenominator(n));
    }
}

TEST_CASE("test mid constraints") {
    unsigned long nValues[] {46, 50, 70};
    unsigned long expected[] {29, 47, 61};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], longestRepetendDenomUsingPrimes(n));
        CHECK_EQ(expected[i], longestRepetendDenomUsingPrimesImproved(n));
        CHECK_EQ(expected[i], longestRepetendDenominator(n));
    }
}

TEST_CASE("test upper constraints") {
    unsigned long nValues[] {1000, 5000, 10'000};
    unsigned long expected[] {983, 4967, 9967};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], longestRepetendDenomUsingPrimes(n));
        CHECK_EQ(expected[i], longestRepetendDenomUsingPrimesImproved(n));
        CHECK_EQ(expected[i], longestRepetendDenominator(n));
    }
}