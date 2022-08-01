/**
 * Problem 10: Summation of Primes
 *
 * https://projecteuler.net/problem=10
 *
 * Goal: Find the sum of all prime numbers <= N.
 *
 * Constraints: 1 <= N <= 1e6
 *
 * e.g.: N = 5
 *      primes = {2, 3, 5}
 *      sum = 10
 */

#include "../../doctest/doctest.h"
#include "pe-maths/primes.h"
#include <numeric>
#include <stdexcept>
#include <vector>

// Stores the cumulative sum of prime numbers to allow quick access to the
// answers for multiple N <= [n].
// Solution mimics original Sieve of Eratosthenes algorithm that iterates
// over only odd numbers & their multiples, but uses boolean mask to alter a
// list of cumulative sums instead of returning a list of prime numbers.
// @return list of the cumulative sums of prime numbers <= index.
std::vector<unsigned long long> sumOfPrimesQD(unsigned long n)
{
    if (n % 2)
        throw std::invalid_argument(
                "Limit must be even otherwise loop check needed"
                );

    std::vector<bool> primesBool;
    // declaring vector with size would initialise with default & require
    // assignment instead of push_back()
    primesBool.reserve(n + 1);
    for (int i = 0; i <= n; i++) {
        primesBool.push_back(i > 2 && i % 2 || i == 2);
    }

    std::vector<unsigned long long> sums(n + 1);
    sums[2] = 2;
    for (unsigned long long i = 3; i <= n; i += 2) {
        unsigned long long prevSum = sums[i - 1];
        if (primesBool[i]) {
            // change next even number as well as current odd
            sums[i] = prevSum + i;
            sums[i + 1] = prevSum + i;
            if (i * i < n) {
                for (unsigned long long j = i * i; j <= n; j += 2 * i) {
                    primesBool[j] = false;
                }
            }
        }
        else {
            sums[i] = prevSum;
            sums[i + 1] = prevSum;
        }
    }

    return sums;
}

// Similar to the above solution in that it stores the cumulative sum of
// prime numbers to allow future quick access; however, it replaces the
// typical boolean mask from the Sieve of Eratosthenes algorithm with this
// cumulative cache.
// An unaltered element == 0 indicates a prime, with future multiples of that
// prime marked with a -1, before the former, and its even successor, are
// replaced by the total so far.
// @return list of the cumulative sums of prime numbers <= index.
std::vector<unsigned long long> sumOfPrimesQDOpt(unsigned long n)
{
    unsigned long long total {2};
    std::vector<unsigned long long> sums(n + 1);
    sums[2] = total;
    for (int i = 3; i <= n; i += 2) {
        if (sums[i] == 0) {
            total += i;
            // mark all multiples of this prime
            for (int j = i; j <= n; j += i) {
                sums[j] = -1;
            }
        }
        // change next even number as well as current odd
        sums[i] = total;
        sums[i + 1] = total;
    }

    return sums;
}

TEST_SUITE("test both quick draw solutions") {
    unsigned long limit {1'000'000};
    // static storage duration warning?
    std::vector<unsigned long long> allPrimes = sumOfPrimesQD(limit);
    std::vector<unsigned long long> allPrimesOpt = sumOfPrimesQDOpt(limit);

    TEST_CASE("create identical lists") {
        CHECK_EQ(allPrimes.size(), allPrimesOpt.size());

        CHECK_EQ(
                std::vector<unsigned long long>(
                        allPrimes.cend() - 10, allPrimes.cend()
                        ),
                std::vector<unsigned long long>(
                        allPrimesOpt.cend() - 10, allPrimesOpt.cend()
                )
                );
    }

    TEST_CASE("test lower constraints") {
        std::vector<unsigned long> nValues{ 2, 3, 5, 10 };
        std::vector<unsigned long long> expected{ 2, 5, 10, 17 };

        for (int i = 0; i < nValues.size(); i++) {
            std::vector<unsigned long> generated = primeNumbers(nValues[i]);
            CHECK_EQ(expected[i],
                     std::accumulate(generated.cbegin(), generated.cend(),
                                     static_cast<unsigned long long>(0))
            );
            CHECK_EQ(expected[i], allPrimes[nValues[i]]);
            CHECK_EQ(expected[i], allPrimesOpt[nValues[i]]);
        }
    }

    TEST_CASE("test mid constraints") {
        std::vector<unsigned long> nValues{ 100, 5000 };
        std::vector<unsigned long long> expected{ 1060, 1'548'136 };

        for (int i = 0; i < nValues.size(); i++) {
            std::vector<unsigned long> generated = primeNumbers(nValues[i]);
            CHECK_EQ(expected[i],
                     std::accumulate(generated.cbegin(), generated.cend(),
                                     static_cast<unsigned long long>(0))
            );
            CHECK_EQ(expected[i], allPrimes[nValues[i]]);
            CHECK_EQ(expected[i], allPrimesOpt[nValues[i]]);
        }
    }

    TEST_CASE("test upper constraints") {
        std::vector<unsigned long> nValues{ 300'000, 1'000'000 };
        std::vector<unsigned long long> expected{ 3'709'507'114,
                                                  37'550'402'023 };

        for (int i = 0; i < nValues.size(); i++) {
            std::vector<unsigned long> generated = primeNumbers(nValues[i]);
            CHECK_EQ(expected[i],
                     std::accumulate(generated.cbegin(), generated.cend(),
                                     static_cast<unsigned long long>(0))
            );
            CHECK_EQ(expected[i], allPrimes[nValues[i]]);
            CHECK_EQ(expected[i], allPrimesOpt[nValues[i]]);
        }
    }
}