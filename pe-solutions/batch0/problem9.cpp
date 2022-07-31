/**
 * Problem 9: Special Pythagorean Triplet
 *
 * https://projecteuler.net/problem=9
 *
 * Goal: If there exists any Pythagorean triplet for which a + b + c = N,
 * find the maximum product among all such triplets, otherwise return -1.
 *
 * Constraints: 1 <= N <= 3000
 *
 * Pythagorean Triplet: A set of 3 natural numbers, such that a < b < c &&
 * a^2 + b^2 = c^2.
 *
 * e.g.: N = 12
 *       triplets = {{3,4,5}}; as 3 + 4 + 5 == 12
 *       product = 3*4*5 = 60
 */

#include "../../doctest/doctest.h"
#include "pe-maths/pythagorean.h"
#include <cmath>
#include <functional>
#include <optional>
#include <vector>
using namespace std;

unsigned long product(tuple<int, int, int> triplet)
{
    return get<0>(triplet) * get<1>(triplet) * get<2>(triplet);
}

unsigned long maxTripletProduct(
        int n,
        const function<optional<tuple<int, int, int>>(int)>& solution
        )
{
    auto triplet = solution(n);
    return triplet.has_value() ? product(triplet.value()) : -1;
}

// Solution iterates through values of c and b with some limits:
//      - Set {3,4,5} being the smallest existing triplet, means c >= 5 and
//      can be at most [n]/(2 - 1).
//      - b cannot be <= a.
//      - Triplet elements must either be all evens OR 2 odds with 1 even.
//      Therefore, the sum of a triplet must be even as the sum of evens is
//      an even number and the sum of 2 odds is an even number as well.
// @return std::tuple(a, b, c) if one exists, or nothing(?).
optional<tuple<int, int, int>> maxTripletBruteBC(int n)
{
    if (n % 2)
        return {};

    optional<tuple<int, int, int>> maxTriplet;
    for (int c = n / 2 - 1; c >= 5; c--) {
        auto diff = n - c;
        for (int b = c - 1; b >= diff / 2; b--) {
            auto a = diff - b;
            if (b <= a)
                break;
            if (hypot(a, b) == 1.0 * c) {
                maxTriplet = tuple(a, b, c);
                break;
            }
        }
    }

    return maxTriplet;
}

// Solution iterates through values of a only based on:
//      - Set {3,4,5} being the smallest existing triplet, so a must be >= 3
//      and can be at most [n]/(3 - 1).
//      - Inserting c = [n] - a - b into the formula a^2 + b^2 = c^2 reduces to:
//          2ab + 2bn = n^2 - 2an
//          b = n(n - 2a)/2(n - a)
//      - Exhaustive search shows that the first maximum triplet found will
//      be the only solution, so the loop can be broken early.
//      - Triplet elements must either be all evens OR 2 odds with 1 even.
//      Therefore, the sum of a triplet must be even as the sum of evens is
//      an even number and the sum of 2 odds is an even number as well.
// @return std::tuple(a, b, c) if one exists, or nothing(?).
optional<tuple<int, int, int>> maxTripletBruteA(int n)
{
    if (n % 2)
        return {};

    optional<tuple<int, int, int>> maxTriplet;
    for (int a = n / 3 - 1; a >= 3; a--) {
        auto b = n * (n - 2 * a) / (2 * (n - a));
        auto c = n - a - b;
        if (a < b && hypot(a, b) == 1.0 * c) {
            maxTriplet = tuple(a, b, c);
            break;
        }
    }

    return maxTriplet;
}

// Solution optimised based on:
//      - All Pythagorean triplets can be reduced to a primitive one by
//      dividing out the gcd(a,b,c) = d, such that:
//          a + b + c = 2m(m + n) * d, with n > m > 0.
//      - A triplet is primitive if m XOR n is even and gcd(m,n) = 1. The
//      latter occurs because gdc(a,b) = gcd(b,c) = gcd(c,a) = 1.
//      - Exhaustive search shows that the first maximum triplet found will
//      be the only solution, so the loop can be broken early.
// @return std::tuple(a, b, c) if one exists, or nothing(?).
optional<tuple<int, int, int>> maxTripletOptimised(int num)
{
    if (num % 2)
        return {};

    optional<tuple<int, int, int>> maxTriplet;
    auto limit = num / 2;
    auto mMax = static_cast<int>(ceil(sqrt(1.0 * limit)));
    for (int m = 2; m < mMax; m++) {
        if (limit % m == 0) {
            // find even divisor m (> 1) of num/2
            auto kMax = limit / m;
            while (kMax % 2 == 0) {
                // find odd divisor k (= m + n) of num/2m
                kMax /= 2;
            }
            auto k = m % 2 ? m + 2 : m + 1;
            while (k < 2 * m && k <= kMax) {
                if (kMax % k == 0 && isCoPrime(k, m)) {
                    maxTriplet = pythagoreanTriplet(
                            m, k - m, limit / (k * m)
                            );
                    break;
                }
                k += 2;
            }
            if (maxTriplet.has_value())
                break;
        }
    }

    return maxTriplet;
}

TEST_CASE("test maxTriplet returns no value when none found") {
    vector<int> nValues { 4, 6, 31, 99, 100 };

    for (int n : nValues) {
        CHECK_FALSE(maxTripletBruteBC(n).has_value());
        CHECK_FALSE(maxTripletBruteA(n).has_value());
        CHECK_FALSE(maxTripletOptimised(n).has_value());
    }
}

TEST_SUITE("test maxTriplet returns tuple") {
    TEST_CASE("with lower constraints") {
        vector<int> nValues { 12, 24, 30 };
        vector<tuple<int, int, int>> expected {
            tuple(3, 4, 5), tuple(6, 8, 10), tuple(5, 12, 13)
        };

        for (int i = 0; i < nValues.size(); i++) {
            CHECK_EQ(expected[i], maxTripletBruteBC(nValues[i]));
            CHECK_EQ(expected[i], maxTripletBruteA(nValues[i]));
            CHECK_EQ(expected[i], maxTripletOptimised(nValues[i]));
        }
    }

    TEST_CASE("with mid constraints") {
        vector<int> nValues { 90, 650, 1000 };
        vector<tuple<int, int, int>> expected {
                tuple(15, 36, 39), tuple(25, 312, 313), tuple(200, 375, 425)
        };

        for (int i = 0; i < nValues.size(); i++) {
            CHECK_EQ(expected[i], maxTripletBruteBC(nValues[i]));
            CHECK_EQ(expected[i], maxTripletBruteA(nValues[i]));
            CHECK_EQ(expected[i], maxTripletOptimised(nValues[i]));
        }
    }

    TEST_CASE("with upper constraints") {
        vector<int> nValues { 2214, 3000 };
        vector<tuple<int, int, int>> expected {
                tuple(533, 756, 925), tuple(750, 1000, 1250)
        };

        for (int i = 0; i < nValues.size(); i++) {
            CHECK_EQ(expected[i], maxTripletBruteBC(nValues[i]));
            CHECK_EQ(expected[i], maxTripletBruteA(nValues[i]));
            CHECK_EQ(expected[i], maxTripletOptimised(nValues[i]));
        }
    }
}

TEST_SUITE("test maxTripletProduct()") {
    TEST_CASE("when none found") {
        vector<int> nValues { 1, 10, 1231 };

        for (int n : nValues) {
            CHECK_EQ(-1, maxTripletProduct(n, maxTripletBruteBC));
            CHECK_EQ(-1, maxTripletProduct(n, maxTripletBruteA));
            CHECK_EQ(-1, maxTripletProduct(n, maxTripletOptimised));
        }
    }

    TEST_CASE("with lower constraints") {
        int n {12};
        unsigned long expected {60};

        CHECK_EQ(expected, maxTripletProduct(n, maxTripletBruteBC));
        CHECK_EQ(expected, maxTripletProduct(n, maxTripletBruteA));
        CHECK_EQ(expected, maxTripletProduct(n, maxTripletOptimised));
    }

    TEST_CASE("with mid constraints") {
        vector<int> nValues { 90, 1000, 3000 };
        vector<unsigned long> expected { 21060, 31'875'000, 937'500'000 };

        for (int i = 0; i < nValues.size(); i++) {
            CHECK_EQ(expected[i], maxTripletProduct(
                    nValues[i],
                    maxTripletBruteBC
                    ));
            CHECK_EQ(expected[i], maxTripletProduct(
                    nValues[i],
                    maxTripletBruteA
            ));
            CHECK_EQ(expected[i], maxTripletProduct(
                    nValues[i],
                    maxTripletOptimised
            ));
        }
    }
}