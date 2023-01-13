#include "prime-factors.h"

#include <stdexcept>

#include "../../doctest/doctest.h"

/*
 * Prime decomposition repeatedly divides out all prime factors using a Direct Search
 * Factorisation algorithm without any optimisation.
 *
 * @return map of prime factors (keys) and their exponents (values).
 * @throws std::invalid_argument if n <= 1.
 */
pfMap primeFactors(unsigned long long n)
{
    if (n <= 1)
        throw std::invalid_argument("Natural number must be greater than 1");

    pfMap primes;

    for (unsigned long long factor {2uLL}; factor * factor <= n; ++factor) {
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

/*
 * Flattens prime factor decomposition, such that, for example:
 *      pf(10) = {2: 2, 5: 2} -> {2, 2, 5, 5}
 *
 * @return list of prime factors duplicated by their exponent count.
 */
std::vector<unsigned long long> primeFactorsFlattened(const pfMap& factors)
{
    std::vector<unsigned long long> flattened;

    for (const auto& [k, v] : factors) {
        // list used since it is potentially expensive to insert(pos, count, value) if
        // count is high?
        std::vector<unsigned long long> f(v, k);
        flattened.insert(flattened.end(), f.begin(), f.end());
    }

    return flattened;
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
            const auto& factors = primeFactors(n);
            std::vector<unsigned long long> flattened = primeFactorsFlattened(factors);

            CHECK_EQ(expected[i], flattened);
        }
    }
}