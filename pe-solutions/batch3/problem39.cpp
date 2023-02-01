/*
 * Problem 39: Integer Right Triangles
 *
 * https://projecteuler.net/problem=39
 *
 * Goal: If P is the perimeter of a right-angle triangle, find the smallest value of
 * P <= N that has the maximum number of (a, b, c) solutions.
 *
 * Constraints: 12 <= N <= 5e6
 *
 * e.g. P = 120 has 3 solutions: (20, 48, 52), (24, 45, 51), & (30, 40, 50).
 *
 * e.g.: N = 12
 *       P = 12 as 12 is the only sum of a Pythagorean triplet (3, 4, 5)
 */

#include <cmath>
#include <vector>

#include "../../doctest/doctest.h"

#include "pe-maths/pythagorean.h"

/*
 * Brute solution based on the following:
 *
 *      - Pythagorean Triplets must either be all evens OR 2 odds with 1 even. So, the
 *      sum of triplets will only ever be an even number as the sum of evens is an even
 *      number, as is the sum of 2 odds.
 *
 *      - Since a < b < c and a + b + c = P, a will not be higher than P/3.
 *
 *      - If c = P - a - b is inserted into the equation a^2 + b^2 = c^2, then:
 *
 *              a^2 + b^2 = P^2 - 2aP - 2bP + 2ab + a^2 + b^2
 *
 *              b = P(P - 2 a) / 2(P - a)
 *
 *      which means values of P and a that result in an integer value b represent a
 *      valid Triplet.
 */
unsigned long mostTripletSolutionsBrute(unsigned long n)
{
    unsigned long bestP {12}, mostSols {1};

    for (unsigned long p {14}; p <= n; p += 2) {
        unsigned long pSols {};
        for (int a {4}; a < p / 3; ++a) {
            unsigned long long b = (p * (p - 2 * a)) % (2 * (p - a));
            if (b == 0uLL)
                pSols++;
        }
        if (pSols > mostSols) {
            bestP = p;
            mostSols = pSols;
        }
    }

    return bestP;
}

/*
 * Solution is influenced by the previously determined solution for finding primitive
 * Pythagorean Triplets (Batch 0 - Problem 9).
 */
unsigned long mostTripletSolutions(unsigned long n)
{
    unsigned long bestP {12}, mostSols {1};

    for (unsigned long p {14}; p <= n; p += 2) {
        unsigned long pSols {};
        const auto limit = p / 2;
        const auto mMax = static_cast<int>(std::ceil(std::sqrt(limit)));
        for (int m {2}; m <= mMax; ++m) {
            if (!(limit % m)) {
                auto kMax = p / (2 * m);
                auto k = m & 1 ? m + 2 : m + 1;
                while (k < 2 * m && k <= kMax) {
                    if (!(kMax % k) && isCoPrime(k, m))
                        pSols++;
                    k += 2;
                }
            }
        }
        if (pSols > mostSols) {
            bestP = p;
            mostSols = pSols;
        }
    }

    return bestP;
}

/*
 * Solution above is optimised further by relying solely on Euclid's formula to
 * generate all primitive Pythagorean triplets.
 *
 * Every perimeter that has a triplet will be represented as a count in pSols. This
 * array is finally converted to another that accumulates the perimeter (below the
 * given limit) with the most counts.
 *
 * N.B. The upper bound for m is found by substituting Euclid's formulae into the
 * perimeter formula & reducing to:
 *
 *      p = 2dm(m + n)
 *
 * which means when d = 1 & n = 1, at most 2m^2 must be below the given limit.
 */
unsigned long mostTripletSolutionsImproved(unsigned long limit)
{
    std::vector<unsigned long> pSols(limit + 1, 0uL);
    unsigned long m {2};
    while (2 * m * m < limit) {
        for (unsigned long n {1}; n < m; ++n) {
            // ensure both m and n are not odd and that m and n are co-prime (gcd == 1)
            if (m & 1 && n & 1 || !isCoPrime(m, n))
                continue;
            unsigned long d {1};
            while (true) {
                auto [a, b, c] = pythagoreanTriplet(m, n, d);
                auto p = a + b + c;
                if (p > limit)
                    break;
                pSols[p]++;
                d++;
            }
        }
        m++;
    }

    unsigned long bestP {12}, bestCount {1};
    std::vector<unsigned long> best(limit + 1);
    for (int i {0}; i <= limit; ++i) {
        if (i < 12) {
            best[i] = pSols[i];
        }
        else {
            auto count = pSols[i];
            if (count > bestCount) {
                bestP = i;
                bestCount = count;
            }
            best[i] = bestP;
        }
    }

    return best[limit];
}

TEST_CASE("test lower constraints") {
    unsigned long nValues[] {12, 15, 40, 50, 80, 100, 1000};
    unsigned long expected[] {12, 12, 12, 12, 60, 60, 840};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], mostTripletSolutionsBrute(n));
        CHECK_EQ(expected[i], mostTripletSolutions(n));
        CHECK_EQ(expected[i], mostTripletSolutionsImproved(n));
    }
}

TEST_CASE("test mid constraints") {
    unsigned long n {100'000};
    unsigned long expected {55440};

    CHECK_EQ(expected, mostTripletSolutionsBrute(n));
    CHECK_EQ(expected, mostTripletSolutions(n));
    CHECK_EQ(expected, mostTripletSolutionsImproved(n));
}