/**
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

#include "../../doctest/doctest.h"
#include <map>
#include <vector>
#include "pe-maths/prime-factors.h"

// Uses prime decomposition via helper function.
unsigned long long largestPrimeFactor(unsigned long long n)
{
    std::map<unsigned long long, unsigned long> primes = primeFactors(n);
    unsigned long long largest {2};
    for (const auto [k, _] : primes) {
        if (k > largest)
            largest = k;
    }
    return largest;
}

// Uses prime decomposition via trial division without any optimisation.
unsigned long long largestPrimeFactorSimple(unsigned long long n)
{
    for (unsigned long long factor {2}; factor * factor <= n; factor++) {
        while (n % factor == 0 && n != factor) {
            n /= factor;
        }
    }
    return n;
}

TEST_CASE("test lower constraints") {
    std::vector<unsigned long long> nValues {10, 48};
    std::vector<unsigned long long> expected {5, 3};

    for (int i {0}; i < nValues.size(); i++) {
        CHECK_EQ(expected[i], largestPrimeFactorSimple(nValues[i]));
        CHECK_EQ(expected[i], largestPrimeFactor(nValues[i]));
    }
}

TEST_CASE("test when N is small prime") {
    std::vector<unsigned long long> nValues {17, 23};

    for (int i {0}; i < nValues.size(); i++) {
        CHECK_EQ(nValues[i], largestPrimeFactorSimple(nValues[i]));
        CHECK_EQ(nValues[i], largestPrimeFactor(nValues[i]));
    }
}

TEST_CASE("test mid constraints") {
    std::vector<unsigned long long> nValues {147, 330};
    std::vector<unsigned long long> expected {7, 11};

    for (int i {0}; i < nValues.size(); i++) {
        CHECK_EQ(expected[i], largestPrimeFactorSimple(nValues[i]));
        CHECK_EQ(expected[i], largestPrimeFactor(nValues[i]));
    }
}

TEST_CASE("test larger mid constraints") {
    std::vector<unsigned long long> nValues {13195, 200'000};
    std::vector<unsigned long long> expected {29, 5};

    for (int i {0}; i < nValues.size(); i++) {
        CHECK_EQ(expected[i], largestPrimeFactorSimple(nValues[i]));
        CHECK_EQ(expected[i], largestPrimeFactor(nValues[i]));
    }
}

TEST_CASE("test when N is large prime, including Euler's") {
    std::vector<unsigned long long> nValues {7919, 2'147'483'647};

    for (int i {0}; i < nValues.size(); i++) {
        CHECK_EQ(nValues[i], largestPrimeFactorSimple(nValues[i]));
        CHECK_EQ(nValues[i], largestPrimeFactor(nValues[i]));
    }
}

TEST_CASE("test when N has few large factors") {
    unsigned long long n {600'851'475'143};
    unsigned long long expected {6857};

    CHECK_EQ(expected, largestPrimeFactorSimple(n));
    CHECK_EQ(expected, largestPrimeFactor(n));
}

TEST_CASE("test when N has many small factors") {
    unsigned long long n = 1e12;
    unsigned long long expected {5};

    CHECK_EQ(expected, largestPrimeFactorSimple(n));
    CHECK_EQ(expected, largestPrimeFactor(n));
}