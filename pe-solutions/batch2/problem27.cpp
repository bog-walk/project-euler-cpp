/*
 * Problem 27: Quadratic Primes
 *
 * https://projecteuler.net/problem=27
 *
 * Goal: Find coefficients a & b for the quadratic expression that produces the maximum
 * number of primes for consecutive values of n in [0, N].
 *
 * Constraints: 42 <= N <= 2000
 *
 * Quadratic Formula: n^2 + an + b, where |a| < N & |b| <= N.
 *
 * Euler's Quadratic formula -> n^2 + n + 41, produces 40 primes for the consecutive
 * values of [0, 39].
 * The formula -> n^2 - 79n + 1601, produces 80 primes for the consecutive values of
 * [0, 79].
 *
 * e.g.: N = 42
 *       formula -> n^2 - n + 41, produces 42 primes
 *       result = -1 41
 */

#include <tuple>

#include "../../doctest/doctest.h"

#include "pe-maths/is-prime.h"
#include "pe-maths/primes.h"

/*
 * A brute force of all a, b combinations that is optimised based on the following:
 *
 *  -   When n = 0, formula -> 0^2 + 0 + b = b, which means that b must be a prime number
 *  itself.
 *
 *  -   When n = 1, formula -> 1^2 + a + b, so, with b being prime:
 *          - if b = 2, then a must be even for result to be an odd prime.
 *          - if b > 2, then a must be odd for result to be an odd prime.
 *
 * @return tuple of (a, b, count_of_primes)
 */
std::tuple<int, int, int> quadPrimeCoeff(int maxN)
{
    std::tuple<int, int, int> bestQuadratic {0, 0, 0};
    const auto primes = primeNumbers(maxN);

    auto lowestA = maxN & 1 ? -maxN - 2 : -maxN - 1;
    // a will only be even if b == 2, so loop through odd values only & adjust later
    for (int a {lowestA}; a < maxN; a += 2) {
        for (auto & b : primes) {
            int n {};
            auto adjustedA = b != 2 ? a : a - 1;
            while (true) {
                int quad = n * n + adjustedA * n + static_cast<int>(b);
                // a function that accepts unsigned long argument will simply overflow if
                // given a negative number, instead of prohibiting execution
                if (quad <= 1 || !isPrime(quad))
                    break;
                n++;
            }
            if (n > std::get<2>(bestQuadratic))
                bestQuadratic = {a, b, n + 1};
        }
    }

    return bestQuadratic;
}

TEST_CASE("test lower constraints") {
    unsigned long nValues[] {42, 50};
    std::tuple<int, int, int> expected[] {{-1, 41, 42}, {-5, 47, 44}};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], quadPrimeCoeff(n));
    }
}

TEST_CASE("test upper constraints") {
    unsigned long nValues[] {1000, 2000};
    std::tuple<int, int, int> expected[] {{-61, 971, 72}, {-79, 1601, 81}};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], quadPrimeCoeff(n));
    }
}