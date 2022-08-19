#include "is-prime.h"

#include <cmath>

#include "../../doctest/doctest.h"

/**
 * Checks if [n] is prime.
 */
bool isPrime(unsigned long n)
{
    // 2 is the only even prime
    if (n == 2 || n == 3)
        return true;

    // primes > 3 are of the form 6k(+/-1) (i.e. they are never multiples of 3)
    if (n < 2 || !(n % 2) || !(n % 3))
        return false;

    // n can only have 1 prime factor > sqrt(n): n itself!
    const auto max = static_cast<unsigned long>(std::sqrt(n));
    // as multiples of prime 5 have not been assessed yet
    // 11, 13, 17, 19, & 23 will all bypass this loop
    for (int step {5}; step <= max; step += 6) {
        if (!(n % step) || !(n % (step + 2)))
            return false;
    }

    return true;
}

TEST_SUITE("test isPrime()") {
    TEST_CASE("with small primes") {
        unsigned long nValues[] {2, 5, 11, 17, 29, 7919,
                                 514'229, 9'369'319};

        for (const auto& n : nValues) {
            CHECK(isPrime(n));
        }
    }

    TEST_CASE("with small composites") {
        unsigned long nValues[] {1, 4, 9, 14, 221, 9523, 22041,
                                 997'653, 999'715'709};

        for (const auto& n : nValues) {
            CHECK_FALSE(isPrime(n));
        }
    }
}