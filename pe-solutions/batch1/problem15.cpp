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

#include <queue>

#include "../../doctest/doctest.h"

#include "pe-combinatorics/binomial-coeff.h"

/*
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
    BigInt routes = binomialCoefficient(n + m, m);

    return routes % mod;
}

/*
 * Solution uses breadth-first search summation to generate a graph that contains
 * all counts of possible paths from the start node (0, 0 or top left corner) to the
 * node at index[n][m].
 *
 * The original lattice is converted to a graph representation of its points (nodes)
 * instead of its edges. For example, exploring each node at each depth results in the
 * following cumulative count:
 *
 *      given N = 2, M = 2
 *        1
 *       1 1
 *      1 2 1
 *       3 3
 *        6 -> total count of paths to bottom right corner
 */
unsigned long long** latticePathBFS(unsigned short n, unsigned short m)
{
    unsigned long long mod {1'000'000'007uLL};
    // goal is bottom right (outer) corner, so an extra node exists for each outer edge
    auto* lattice = new unsigned long long*[n+1];
    for (int row {0}; row <= n; ++row) {
        lattice[row] = new unsigned long long[m+1];
        std::fill(lattice[row], lattice[row] + m + 1, 0uLL);
    }
    // path from root to itself
    lattice[0][0] = 1;

    // create queue to keep track of encountered but unexplored adjacent nodes
    std::queue<std::pair<unsigned short, unsigned short>> unvisited;
    unvisited.push(std::pair {1, 0});
    unvisited.push(std::pair {0, 1});

    while (!unvisited.empty()) {
        auto [row, col] = unvisited.front();
        unvisited.pop();
        if (lattice[row][col])  // already explored paths from root to this node
            continue;

        unsigned long long routes {};
        if (row > 0)
            routes += lattice[row-1][col];
        if (col > 0)
            routes += lattice[row][col-1];
        routes %= mod;
        lattice[row][col] = routes;
        // queue next 2 adjacent nodes (down & right) if they exist
        if (row < n)
            unvisited.push(std::pair {row + 1, col});
        if (col < m)
            unvisited.push(std::pair {row, col + 1});
    }

    return lattice;
}

TEST_SUITE("test latticePath()") {
    unsigned short maxSize {500};
    unsigned long long** lattice;

    TEST_CASE("setup BFS quick-draw") {
        lattice = latticePathBFS(maxSize, maxSize);
    }

    TEST_CASE("test lower constraints") {
        unsigned long long expected[] {2, 6, 20};

        for (unsigned short n {1}; n < 4; ++n) {
            const BigInt expectedBI {expected[n-1]};
            CHECK_EQ(expectedBI.toString(), latticePathRoutes(n, n).toString());
            CHECK_EQ(expected[n-1], lattice[n][n]);
        }
    }

    TEST_CASE("test mid constraints") {
        unsigned short nValues[] {20, 100};
        unsigned long long expected[] {846'527'861, 407'336'795};

        for (const auto& n : nValues) {
            auto i = &n - &nValues[0];
            const BigInt expectedBI {expected[i]};
            CHECK_EQ(expectedBI.toString(), latticePathRoutes(n, n).toString());
            CHECK_EQ(expected[i], lattice[n][n]);
        }
    }

    TEST_CASE("test upper constraints") {
        unsigned short nValues[] {200, 400};
        unsigned long long expected[] {587'893'473, 358'473'912};

        for (const auto& n : nValues) {
            auto i = &n - &nValues[0];
            const BigInt expectedBI {expected[i]};
            CHECK_EQ(expectedBI.toString(), latticePathRoutes(n, n).toString());
            CHECK_EQ(expected[i], lattice[n][n]);
        }
    }

    TEST_CASE("test rectangular grid") {
        unsigned short nValues[] {3, 9, 31};
        unsigned short mValues[] {2, 500, 399};
        unsigned long long expected[] {10, 663'599'170, 569'249'261};

        for (const auto& n : nValues) {
            auto i = &n - &nValues[0];
            const BigInt expectedBI {expected[i]};
            CHECK_EQ(expectedBI.toString(), latticePathRoutes(n, mValues[i]).toString());
            CHECK_EQ(expected[i], lattice[n][mValues[i]]);
        }
    }
}