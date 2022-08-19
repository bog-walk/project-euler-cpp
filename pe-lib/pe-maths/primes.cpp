#include "primes.h"

#include "../../doctest/doctest.h"

/**
 * Sieve of Eratosthenes algorithm outputs all prime numbers less than or equal to [n],
 * with processing time cut in half by only allocating mask memory to odd numbers and
 * by only looping through multiples of odd numbers.
 */
std::vector<unsigned long> primeNumbers(unsigned long n)
{
    if (n < 2)
        return {};

    const auto oddSieve = (n - 1) / 2;
    // create mask representing [2, 3..n step 2]
    std::vector<bool> boolMask(oddSieve + 1, true);
    // boolMask[0] corresponds to prime 2 and is skipped
    for (int i {1}; i * i <= n; ++i) {
        if (boolMask[i]) {
            // j = next index at which multiple of odd prime exists
            auto j = 2 * i * (i + 1);
            while (j <= oddSieve) {
                boolMask[j] = false;
                j += 2 * i + 1;
            }
        }
    }

    std::vector<unsigned long> primes {2};

    for (int i {1}; i <= oddSieve; ++i) {
        if (boolMask[i])
            primes.push_back(2 * i + 1);
    }

    return primes;
}

TEST_SUITE("test primeNumbers()") {
    TEST_CASE("with N == 1") {
        unsigned long n {1};

        CHECK(primeNumbers(n).empty());
    }

    TEST_CASE("with small N") {
        unsigned long n {30};
        std::vector<unsigned long> expected {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

        CHECK_EQ(expected, primeNumbers(n));
    }

    TEST_CASE("with large N") {
        unsigned long n {10'000};
        int expectedSize {1229};
        std::vector<unsigned long> expectedTail {9887, 9901, 9907, 9923, 9929, 9931,
                                                 9941, 9949, 9967, 9973};

        std::vector<unsigned long> actual = primeNumbers(n);
        // slice the last 10 elements from the previous vector
        std::vector<unsigned long> actualTail {actual.cend() - 10, actual.cend()};

        CHECK_EQ(expectedSize, actual.size());
        CHECK_EQ(expectedTail, actualTail);
    }
}