#include "prime-factors.h"

#include <stdexcept>
#include <vector>

#include "../../doctest/doctest.h"

/**
 * Prime decomposition repeatedly divides out all prime factors using a Direct Search
 * Factorisation algorithm without any optimisation.
 *
 * @return map of prime factors (keys) and their exponents (values).
 * @throws std::invalid_argument if [n] <= 1.
 */
pfMap primeFactors(unsigned long long n)
{
    if (n <= 1)
        throw std::invalid_argument("Natural number must be greater than 1");

    pfMap primes;

    for (unsigned long long factor {2uLL}; factor * factor <= n; ++factor) {
        // C++ value 0 equates to false
        while (!(n % factor) && n != factor) {
            // unlike at(), operator[] inserts key if doesn't exist
            primes[factor]++;
            n /= factor;
        }
    }

    if (n > 1)
        primes[n]++;

    return primes;
}

TEST_SUITE("test primeFactors()") {
    TEST_CASE("denies invalid input") {
        // unsigned arg type would not allow negative N
        CHECK_THROWS_AS(primeFactors(0), std::invalid_argument);
        CHECK_THROWS_AS(primeFactors(1), std::invalid_argument);
    }

    TEST_CASE("correct for valid input") {
        unsigned long long nValues[] {2, 3, 4, 12, 100, 999};
        std::vector<unsigned long long> expected[] {{2}, {3},
                                                    {2, 2},
                                                    {2, 2, 3},
                                                    {2, 2, 5, 5},
                                                    {3, 3, 3, 37}};

        for (const auto& n : nValues) {
            auto i = &n - &nValues[0];
            pfMap primes = primeFactors(n);
            std::vector<unsigned long long> flattened;
            for (const auto& [k, v] : primes) {
                flattened.insert(flattened.end(), v, k);
            }
            CHECK_EQ(expected[i], flattened);
        }
    }
}