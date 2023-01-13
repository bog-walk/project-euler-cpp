/*
 * Problem 3: Largest Prime Factor
 *
 * https://projecteuler.net/problem=3
 *
 * Goal: Find the largest prime factor of N.
 *
 * Constraints: 10 <= N <= 1e12
 *
 * Fundamental Theorem of Arithmetic: There will only ever be a unique set of
 * prime factors for any number greater than 1.
 *
 * e.g.: N = 10
 *       prime factors = {2, 5}
 *       largest = 5
 */

#include <algorithm>

#include "../../doctest/doctest.h"

#include "pe-custom/extension.h"
#include "pe-maths/prime-factors.h"

/*
 * Uses prime decomposition via helper functions.
 */
unsigned long long largestPrimeFactor(unsigned long long n)
{
    pfMap primes = primeFactors(n);
    std::vector<unsigned long long> factors = getMapKeys(primes);

    return *std::max_element(factors.cbegin(), factors.cend());
}

/*
 * Identical to the manual loop solution above (in helper getMapKeys) except that key
 * values are extracted to a vector using std::transform(), from which the maximum element
 * is returned.
 *
 * N.B. A vector constructor would only be able to convert a map to a vector of
 * key-value pairs, as it doesn't take an operation argument.
 * N.N.B. std::transform() could be replaced with std::for_each() but that's
 * essentially a loop, as shown in the helper function pe-custom/utility/getMapKeys().
 */
unsigned long long largestPrimeFactorBuiltIn(unsigned long long n)
{
    pfMap factors = primeFactors(n);

    std::vector<unsigned long long> primes;
    std::transform(factors.cbegin(),
                   factors.cend(),
                   std::back_inserter(primes),
                   [](const auto& factor) {
                        return factor.first;
                    });

    return *std::max_element(primes.cbegin(), primes.cend());
}

/*
 * Uses prime decomposition via trial division without any optimisation.
 *
 * Identical to the helper function without the need for temporary storage.
 */
unsigned long long largestPrimeFactorSimple(unsigned long long n)
{
    for (unsigned long long factor {2uLL}; factor * factor <= n; ++factor) {
        while (!(n % factor) && n != factor) {
            n /= factor;
        }
    }

    return n;
}

TEST_CASE("test lower constraints") {
    unsigned long long nValues[] {10, 48};
    unsigned long long expected[] {5, 3};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], largestPrimeFactorSimple(n));
        CHECK_EQ(expected[i], largestPrimeFactor(n));
        CHECK_EQ(expected[i], largestPrimeFactorBuiltIn(n));
    }
}

TEST_CASE("test when N is small prime") {
    unsigned long long nValues[] {17, 23};

    for (const auto& n : nValues) {
        CHECK_EQ(n, largestPrimeFactorSimple(n));
        CHECK_EQ(n, largestPrimeFactor(n));
        CHECK_EQ(n, largestPrimeFactorBuiltIn(n));
    }
}

TEST_CASE("test mid constraints") {
    unsigned long long nValues[] {147, 330};
    unsigned long long expected[] {7, 11};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], largestPrimeFactorSimple(n));
        CHECK_EQ(expected[i], largestPrimeFactor(n));
        CHECK_EQ(expected[i], largestPrimeFactorBuiltIn(n));
    }
}

TEST_CASE("test upper mid constraints") {
    unsigned long long nValues[] {13195, 200'000};
    unsigned long long expected[] {29, 5};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], largestPrimeFactorSimple(n));
        CHECK_EQ(expected[i], largestPrimeFactor(n));
        CHECK_EQ(expected[i], largestPrimeFactorBuiltIn(n));
    }
}

TEST_CASE("test when N is large prime, including Euler's") {
    unsigned long long nValues[] {7919, 2'147'483'647};

    for (const auto& n : nValues) {
        CHECK_EQ(n, largestPrimeFactorSimple(n));
        CHECK_EQ(n, largestPrimeFactor(n));
        CHECK_EQ(n, largestPrimeFactorBuiltIn(n));
    }
}

TEST_CASE("test when N has few large factors") {
    unsigned long long n {600'851'475'143};
    unsigned long long expected {6857};

    CHECK_EQ(expected, largestPrimeFactorSimple(n));
    CHECK_EQ(expected, largestPrimeFactor(n));
    CHECK_EQ(expected, largestPrimeFactorBuiltIn(n));
}

TEST_CASE("test when N has many small factors") {
    unsigned long long n {static_cast<unsigned long long>(1e12)};
    unsigned long long expected {5};

    CHECK_EQ(expected, largestPrimeFactorSimple(n));
    CHECK_EQ(expected, largestPrimeFactor(n));
    CHECK_EQ(expected, largestPrimeFactorBuiltIn(n));
}