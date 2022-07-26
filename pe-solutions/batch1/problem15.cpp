/*
 * Problem 15: Lattice Paths
 *
 * https://projecteuler.net/problem=15
 *
 * Goal: Find the number of routes through an NxM grid, starting at (0,0) & ending at
 * (n,m), while only being able to move right or down.
 *
 * Constraints: 1 <= N <= 500, 1 <= M <= 500
 *
 * e.g.: N = 2, M = 2
 *       routes = 6 -> {RRDD, RDRD, RDDR, DRRD, DRDR, DDRR}
 */

#include "../../doctest/doctest.h"

#include "pe-combinatorics/binomial-coeff.h"

/**
 * Calculates distinct permutations with identical items.
 *
 * Solution is based on the formula:
 *
 *      x! / Pi(i!)
 *
 * where x is the number of items to be combined & i represents the groups of
 * indistinguishable items to undergo product notation.
 *
 * Note that, if the lattice was assured to be square, the number of routes would be
 * equal to the central binomial coefficient C(2n, n) found as the midline number in
 * the (2n)th row of Pascal's triangle.
 *
 * The formula for a rectangular grid with C(n+m, n) becomes:
 *
 *      (n + m)! / n!m!
 *
 * since grid dimensions determine the number of steps taken & there is a deterministic
 * proportion of R vs D steps.
 *
 * @return number of valid routes scaled down to modulo (1e9 + 7).
 */
BigInt latticePathRoutes(unsigned short n, unsigned short m)
{
    BigInt mod {1'000'000'007uLL};
    auto routes = binomialCoefficient(n + m, m);

    return routes % mod;
}

TEST_CASE("test lower constraints") {
    BigInt expected[] {BigInt {2uLL}, BigInt {6uLL}, BigInt {20uLL},};

    for (unsigned short n {1}; n < 4; ++n) {
        CHECK_EQ(expected[n-1].toString(), latticePathRoutes(n, n).toString());
    }
}

TEST_CASE("test mid constraints") {
    unsigned short n {20};
    BigInt expected {846'527'861uLL};

    CHECK_EQ(expected.toString(), latticePathRoutes(n, n).toString());
}

TEST_CASE("test rectangular grid") {
    unsigned short n {3}, m {2};
    BigInt expected {10uLL};

    CHECK_EQ(expected.toString(), latticePathRoutes(n, m).toString());
}