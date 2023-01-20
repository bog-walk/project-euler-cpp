/*
 * Problem 28: Number Spiral Diagonals
 *
 * https://projecteuler.net/problem=28
 *
 * Goal: Return the sum (mod 1e9 + 7) of the diagonal numbers in an NxN grid that is
 * generated using a spiral pattern.
 *
 * Constraints: 1 <= N <= 1e18, with N always odd
 *
 * Spiral Pattern: Start with 1 & move to the right in a clockwise direction,
 * incrementing the numbers.
 *
 * e.g.: N = 5
 *       grid = 21  22  23  24  25
 *              20  7   8   9   10
 *              19  6   1   2   11
 *              18  5   4   3   12
 *              17  16  15  14  13
 *       iral = {1,3,5,7,9,13,17,21,25}
 *       sum = 101
 */

#include <cmath>

#include "../../doctest/doctest.h"

#include "pe-custom/big-int.h"

namespace spiral {
    const BigInt modulus {1'000'000'007uLL};
    const BigInt one {BigInt::one()}, two {"2"}, three {"3"};
}

/*
 * @return integer value of result % (1e9 + 7)
 */
unsigned long spiralDiagSumBrute(unsigned long long n)
{
    BigInt nBI {n}, sum {spiral::one}, num {spiral::one}, step {spiral::two};

    while (step < nBI) {
        for (int i {0}; i < 4; ++i) {
            num += step;
            sum += num;
        }
        step += spiral::two;
    }

    return (sum % spiral::modulus).toULong();
}

/*
 * Solution based on the formula:
 *
 *      f(n) = 4(2n + 1)^2 - 12n + f(n - 1),
 *
 *      with n as the centre ring number, and
 *
 *      f(0) = 1, as it is the only element, and
 *
 *      f(1) = 25, as the first ring creates a 3x3 grid.
 *
 * So the side of a ring is 2N + 1 wide with the upper right corner being (2n + 1)^2 or
 * the area. So n would need to be divided by 2.
 *
 * @return integer value of result % (1e9 + 7)
 */
unsigned long spiralDiagSumFormulaBrute(unsigned long long n)
{
    BigInt fN {spiral::one}, num {spiral::one};
    BigInt maxNum {static_cast<unsigned long long>(std::ceil(n / 2.0))};

    while (num < maxNum) {
        auto even = spiral::two * num;
        auto odd = even + spiral::one;
        fN += BigInt {"4"} * odd * odd - BigInt {"6"} * even;
        num++;
    }

    return (fN % spiral::modulus).toULong();
}
/*
 * Solution optimised based on the same formula as above, but reduced to:
 *
 *      f(n) = 16n^2 + 4n + 4 + f(n - 1)
 *
 * Third order polynomial function required as the 3rd delta between consecutive f(n)
 * gives a constant, such that ->
 *
 *      ax^3 + bx^2 + cx + d
 *
 * Solving for f(0) to f(3) derives the closed-form formula:
 *
 *      f(n) = (16n^3 + 30n^2 + 26n + 3) / 3
 *
 * @return integer value of result % (1e9 + 7)
 */
unsigned long spiralDiagSumFormulaDerived(unsigned long long n)
{
    BigInt x {(n - 1) / 2};
    auto sum = BigInt {"16"} * x.pow(spiral::three);
    sum += BigInt {"30"} * x * x;
    sum += BigInt {"26"} * x + spiral::three;
    sum /= spiral::three;

    return (sum % spiral::modulus).toULong();
}

TEST_CASE("test lower constraints") {
    unsigned long long nValues[] {1, 3, 5, 7};
    unsigned long expected[] {1, 25, 101, 261};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], spiralDiagSumBrute(n));
        CHECK_EQ(expected[i], spiralDiagSumFormulaBrute(n));
        CHECK_EQ(expected[i], spiralDiagSumFormulaDerived(n));
    }
}

TEST_CASE("test mid constraints") {
    unsigned long long nValues[] {1001, 7001};
    unsigned long expected[] {669'171'001, 789'195'405};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], spiralDiagSumBrute(n));
        CHECK_EQ(expected[i], spiralDiagSumFormulaBrute(n));
        CHECK_EQ(expected[i], spiralDiagSumFormulaDerived(n));
    }
}

TEST_CASE("test upper constraints") {
    unsigned long long n {1'000'001};
    unsigned long expected {4'315'867};

    CHECK_EQ(expected, spiralDiagSumBrute(n));
    CHECK_EQ(expected, spiralDiagSumFormulaBrute(n));
    CHECK_EQ(expected, spiralDiagSumFormulaDerived(n));
}