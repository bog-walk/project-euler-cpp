#include "sum-proper-divisors.h"

#include "../../doctest/doctest.h"

/*
 * Calculates the sum of all divisors of num, not inclusive of num.
 *
 * Solution uses prime factorisation and is optimised based on the following:
 *
 * -    N == 1 has no proper divisor but 1 is a proper divisor of all other naturals.
 *
 * -    A perfect square would duplicate divisors if included.
 *
 * -    Odd numbers cannot have even divisors.
 */
unsigned long sumProperDivisors(unsigned long num)
{
    if (num < 2)
        return 0;

    unsigned long n {num}, p {2}, sum {1};

    while (p * p <= num && n > 1) {
        if (!(n % p)) {
            auto j = p * p;
            n /= p;
            while (!(n % p)) {
                j *= p;
                n /= p;
            }
            sum *= (j - 1);
            sum /= (p - 1);
        }
        if (p == 2)
            p++;
        else
            p += 2;
    }
    if (n > 1)
        sum *= (n + 1);

    return sum - num;
}

TEST_SUITE("test sumProperDivisors()") {
    TEST_CASE("with lower constraints") {
        unsigned long nValues[] {1, 2, 3, 12, 20};
        unsigned long expected[] {0, 1, 1, 16, 22};

        for (const auto& n: nValues) {
            auto i = &n - &nValues[0];
            CHECK_EQ(expected[i], sumProperDivisors(n));
        }
    }

    TEST_CASE("with perfect squares") {
        unsigned long nValues[] {36, 49};
        unsigned long expected[] {55, 8};

        for (const auto& n: nValues) {
            auto i = &n - &nValues[0];
            CHECK_EQ(expected[i], sumProperDivisors(n));
        }
    }

    TEST_CASE("with mid constraints") {
        unsigned long nValues[] {220, 284, 999};
        unsigned long expected[] {284, 220, 521};

        for (const auto& n: nValues) {
            auto i = &n - &nValues[0];
            CHECK_EQ(expected[i], sumProperDivisors(n));
        }
    }

    TEST_CASE("with upper constraints") {
        unsigned long nValues[] {5500, 100'000};
        unsigned long expected[] {7604, 146'078};

        for (const auto& n: nValues) {
            auto i = &n - &nValues[0];
            CHECK_EQ(expected[i], sumProperDivisors(n));
        }
    }
}