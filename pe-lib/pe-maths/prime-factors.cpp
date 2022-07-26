#include "../../doctest/doctest.h"
#include <map>
#include <stdexcept>
#include <vector>
#include "prime-factors.h"

/**
 * Prime decomposition repeatedly divides out all prime factors using a Direct
 * Search Factorisation algorithm without any optimisation.
 *
 * @return map of prime factors (keys) and their exponents (values).
 * @throws IllegalArgumentException if [n] <= 1.
 */
std::map<unsigned long long, unsigned long> primeFactors(unsigned long long n)
{
    if (n <= 1)
        throw std::invalid_argument("Natural number must be greater than 1");
    std::map<unsigned long long, unsigned long> primes;
    for (unsigned long long factor {2}; factor * factor <= n; factor++) {
        while (n % factor == 0 && n != factor) {
            primes[factor]++;
            n /= factor;
        }
    }
    if (n > 1)
        primes[n]++;
    return primes;
}

TEST_CASE("test function denies invalid input") {
    // unsigned arg type would not allow negative N
    CHECK_THROWS_AS(primeFactors(0), std::invalid_argument);
    CHECK_THROWS_AS(primeFactors(1), std::invalid_argument);
}

TEST_CASE("test primeFactors()") {
    std::vector<unsigned long long> nValues {2, 3, 4, 12, 100, 999};
    std::vector<std::vector<unsigned long long>> expected {
            {2}, {3}, {2, 2}, {2, 2, 3}, {2, 2, 5, 5}, {3, 3, 3, 37}
    };

    for (int i {0}; i < nValues.size(); i++) {
        std::map<unsigned long long, unsigned long> primes =
                primeFactors(nValues[i]);
        std::vector<unsigned long long> flattened;
        for (const auto [k, v] : primes) {
            flattened.insert(flattened.end(), v, k);
        }
        CHECK_EQ(expected[i], flattened);
    }
}