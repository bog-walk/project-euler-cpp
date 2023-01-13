#include "factorial.h"

#include "../../doctest/doctest.h"

#include "../pe-custom/big-int.cpp"

/*
 * Recursive solution uses custom BigInt class to handle overflow.
 *
 * 13! overflows 32 bits and 21! overflows 64 bits and 59! produces a result that is
 * greater than 1e80 (postulated to be the number of particles in the universe).
 */
BigInt factorial(unsigned long n, const BigInt& run)
{
    if (n == 0)
        return BigInt::one();
    if (n == 1)
        return run;

    return factorial(n - 1, run * BigInt {n});
}

TEST_SUITE("test factorial()") {
    TEST_CASE("with lower constraints") {
        const BigInt expected[] {
            BigInt {1uLL}, BigInt {1uLL}, BigInt {2uLL},
            BigInt {6uLL}, BigInt {24uLL}, BigInt {120uLL}};

        for (int i {0}; i < 6; ++i) {
            CHECK_EQ(expected[i], factorial(i));
        }
    }

    TEST_CASE("with normal value") {
        unsigned long n {10};
        const BigInt expected {3'628'800uLL};

        CHECK_EQ(expected, factorial(n));
    }
}