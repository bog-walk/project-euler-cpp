#include "binomial-coeff.h"

#include "../../doctest/doctest.h"

#include "../pe-maths/factorial.cpp"

/**
 * Returns the number of ways to choose k items from n items without repetition and
 * without order, namely C(n, k).
 */
BigInt binomialCoefficient(unsigned long n, unsigned long k)
{
    if (k > n)
        return BigInt {0uLL};

    return factorial(n) / (factorial(k) * factorial(n - k));
}

TEST_SUITE("test binomialCoefficient()") {
    TEST_CASE("when k > n") {
        const BigInt expected {0uLL};

        CHECK_EQ(expected.toString(), binomialCoefficient(0, 1).toString());
        CHECK_EQ(expected.toString(), binomialCoefficient(1, 2).toString());
    }

    TEST_CASE("with lower constraints") {
        const BigInt expected {1uLL};

        CHECK_EQ(expected.toString(), binomialCoefficient(0, 0).toString());
        CHECK_EQ(expected.toString(), binomialCoefficient(1, 0).toString());
        CHECK_EQ(expected.toString(), binomialCoefficient(1, 1).toString());
    }

    TEST_CASE("with mid constraints") {
        unsigned long nValues[] {3, 4, 12, 20};
        unsigned long kValues[] {1, 2, 3, 7};
        BigInt expected[] {
                BigInt {3uLL}, BigInt {6uLL}, BigInt {220uLL},
                BigInt {77520uLL}};

        for (const auto& n: nValues) {
            auto i = &n - &nValues[0];
            CHECK_EQ(expected[i].toString(), binomialCoefficient(n, kValues[i]).toString());
        }
    }
}