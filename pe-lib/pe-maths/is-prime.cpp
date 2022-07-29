#include "../../doctest/doctest.h"
#include <cmath>
#include <vector>
#include "is-prime.h"

/**
 * Checks if [n] is prime.
 */
bool isPrime(unsigned long n)
{
    // 2 is the only even prime
    if (n == 2 || n == 3)
        return true;

    // primes > 3 are of the form 6k(+/-1) (i.e. they are never multiples of 3)
    if (n < 2 || n % 2 == 0 || n % 3 == 0)
        return false;

    // n can only have 1 prime factor > sqrt(n): n itself!
    auto max = static_cast<unsigned long>(sqrt(n));
    // as multiples of prime 5 have not been assessed yet
    unsigned long step {5};
    // 11, 13, 17, 19, & 23 will all bypass this loop
    while (step <= max) {
        if (n % step == 0 || n % (step + 2) == 0)
            return false;
        step += 6;
    }

    return true;
}

TEST_CASE("test small primes") {
    std::vector<unsigned long> nValues { 2, 5, 11, 17, 29, 7919,
                                         514'229, 9'369'319 };

    for (unsigned long num : nValues) {
        CHECK(isPrime(num));
    }
}

TEST_CASE("test small composites") {
    std::vector<unsigned long> nValues { 1, 4, 9, 14, 221, 9523, 22041,
                                         997'653, 999'715'709 };

    for (unsigned long num : nValues) {
        CHECK_FALSE(isPrime(num));
    }
}