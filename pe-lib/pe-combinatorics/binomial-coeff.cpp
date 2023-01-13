#include "binomial-coeff.h"

#include "../../doctest/doctest.h"

#include "../pe-maths/factorial.cpp"

/*
 * @return number of ways to choose k items from n items without repetition and
 * without order, namely C(n, k).
 */
BigInt binomialCoefficient(unsigned long n, unsigned long k)
{
    if (k > n)
        return BigInt::zero();

    return factorial(n) / (factorial(k) * factorial(n - k));
}

TEST_SUITE("test binomialCoefficient()") {
    TEST_CASE("when k > n") {
        const BigInt expected {"0"};

        CHECK_EQ(expected, binomialCoefficient(0, 1));
        CHECK_EQ(expected, binomialCoefficient(1, 2));
    }

    TEST_CASE("with lower constraints") {
        const BigInt expected {"1"};

        CHECK_EQ(expected, binomialCoefficient(0, 0));
        CHECK_EQ(expected, binomialCoefficient(1, 0));
        CHECK_EQ(expected, binomialCoefficient(1, 1));
    }

    TEST_CASE("with mid constraints") {
        unsigned long nValues[] {3, 4, 12, 20};
        unsigned long kValues[] {1, 2, 3, 7};
        BigInt expected[] {BigInt {"3"}, BigInt {"6"},
                           BigInt {"220"}, BigInt {"77520"}};

        for (const auto& e: expected) {
            auto i = &e - &expected[0];
            CHECK_EQ(e, binomialCoefficient(nValues[i], kValues[i]));
        }
    }
}