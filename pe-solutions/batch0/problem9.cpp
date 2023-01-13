/*
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

#include <cmath>
#include <functional>
#include <optional>

#include "../../doctest/doctest.h"

#include "pe-maths/pythagorean.h"

inline int product(const triple& triplet)
{
    auto p = std::get<0>(triplet) * std::get<1>(triplet) * std::get<2>(triplet);

    return static_cast<int>(p);
}

int maxTripProduct(unsigned short n,
                   const std::function<std::optional<triple>(unsigned short)>& solution)
{
    auto triplet = solution(n);
    return triplet.has_value() ? product(triplet.value()) : -1;
}

/*
 * Solution iterates through values of c and b with some limits:
 *
 *      - Set {3,4,5} being the smallest existing triplet, means c >= 5 and can be at
 *      most n/(2 - 1).
 *      - b cannot be <= a.
 *      - Triplet elements must either be all evens OR 2 odds with 1 even. Therefore,
 *      the sum of a triplet must be even as the sum of evens is an even number and the
 *      sum of 2 odds is an even number as well.
 *
 * @return std::tuple(a, b, c) if one exists, or nothing(?).
 */
std::optional<triple> maxTripletBruteBC(unsigned short n)
{
    if (n & 1)
        return {};

    std::optional<triple> maxTriplet;

    for (int c = n / 2 - 1; c >= 5; --c) {
        auto diff = n - c;
        for (int b = c - 1; b >= diff / 2; --b) {
            auto a = diff - b;
            if (b <= a)
                break;
            if (std::hypot(a, b) == c) {
                maxTriplet = {a, b, c};
                break;
            }
        }
    }

    return maxTriplet;
}

/*
 * Solution iterates through values of a only based on:
 *
 *      - Set {3,4,5} being the smallest existing triplet, so a must be >= 3 and can be
 *      at most n/(3 - 1).
 *      - Inserting c = n - a - b into the formula a^2 + b^2 = c^2 reduces to:
 *          2ab + 2bn = n^2 - 2an
 *          b = n(n - 2a)/2(n - a)
 *      - Exhaustive search shows that the first maximum triplet found will be the only
 *      solution, so the loop can be broken early.
 *      - Triplet elements must either be all evens OR 2 odds with 1 even. Therefore,
 *      the sum of a triplet must be even as the sum of evens is an even number and the
 *      sum of 2 odds is an even number as well.
 *
 * @return std::tuple(a, b, c) if one exists, or nothing(?).
 */
std::optional<triple> maxTripletBruteA(unsigned short n)
{
    if (n & 1)
        return {};

    std::optional<triple> maxTriplet;

    for (int a = n / 3 - 1; a >= 3; --a) {
        auto b = n * (n - 2 * a) / (2 * (n - a));
        auto c = n - a - b;
        if (a < b && std::hypot(a, b) == c) {
            maxTriplet = {a, b, c};
            break;
        }
    }

    return maxTriplet;
}

/*
 * Solution optimised based on:
 *
 *      - All Pythagorean triplets can be reduced to a primitive one by dividing out
 *      the gcd(a,b,c) = d, such that:
 *          a + b + c = 2m(m + n) * d, with n > m > 0.
 *      - A triplet is primitive if m XOR n is even and gcd(m,n) = 1. The latter occurs
 *      because gdc(a,b) = gcd(b,c) = gcd(c,a) = 1.
 *      - Exhaustive search shows that the first maximum triplet found will be the only
 *      solution, so the loop can be broken early.
 *
 * @return std::tuple(a, b, c) if one exists, or nothing(?).
 */
std::optional<triple> maxTripletOptimised(unsigned short num)
{
    if (num & 1)
        return {};

    std::optional<triple> maxTriplet;

    const auto limit = num / 2;
    const auto mMax = std::ceil(std::sqrt(limit));
    for (int m {2}; m < mMax; ++m) {
        if (!(limit % m)) {
            // find even divisor m (> 1) of num/2
            auto kMax = limit / m;
            while (!(kMax & 1)) {
                // find odd divisor k (= m + n) of num/2m
                kMax /= 2;
            }
            auto k = m % 2 ? m + 2 : m + 1;
            while (k < 2 * m && k <= kMax) {
                if (!(kMax % k) && isCoPrime(k, m)) {
                    maxTriplet = pythagoreanTriplet(m, k - m, limit / (k * m));
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

TEST_CASE("test no value returned if no triplet found") {
    unsigned short nValues[] {4, 6, 31, 99, 100};

    for (const auto& n : nValues) {
        CHECK_FALSE(maxTripletBruteBC(n).has_value());
        CHECK_FALSE(maxTripletBruteA(n).has_value());
        CHECK_FALSE(maxTripletOptimised(n).has_value());
    }
}

TEST_SUITE("test tuple returned if triplet found") {
    TEST_CASE("with lower constraints") {
        unsigned short nValues[] {12, 24, 30};
        triple expected[] {{3, 4, 5}, {6, 8, 10}, {5, 12, 13}};

        for (const auto& n : nValues) {
            auto i = &n - &nValues[0];
            CHECK_EQ(expected[i], maxTripletBruteBC(n));
            CHECK_EQ(expected[i], maxTripletBruteA(n));
            CHECK_EQ(expected[i], maxTripletOptimised(n));
        }
    }

    TEST_CASE("with mid constraints") {
        unsigned short nValues[] {90, 650, 1000};
        triple expected[] {{15, 36, 39}, {25, 312, 313}, {200, 375, 425}};

        for (const auto& n : nValues) {
            auto i = &n - &nValues[0];
            CHECK_EQ(expected[i], maxTripletBruteBC(n));
            CHECK_EQ(expected[i], maxTripletBruteA(n));
            CHECK_EQ(expected[i], maxTripletOptimised(n));
        }
    }

    TEST_CASE("with upper constraints") {
        unsigned short nValues[] {2214, 3000};
        triple expected[] {{533, 756, 925}, {750, 1000, 1250}};

        for (const auto& n : nValues) {
            auto i = &n - &nValues[0];
            CHECK_EQ(expected[i], maxTripletBruteBC(n));
            CHECK_EQ(expected[i], maxTripletBruteA(n));
            CHECK_EQ(expected[i], maxTripletOptimised(n));
        }
    }
}

TEST_SUITE("test helper maxTripProduct()") {
    TEST_CASE("when no triplet found") {
        unsigned short nValues[] {1, 10, 1231};
        int expected {-1};

        for (const auto& n : nValues) {
            CHECK_EQ(expected, maxTripProduct(n, maxTripletBruteBC));
            CHECK_EQ(expected, maxTripProduct(n, maxTripletBruteA));
            CHECK_EQ(expected, maxTripProduct(n, maxTripletOptimised));
        }
    }

    TEST_CASE("with lower constraints") {
        unsigned short n {12};
        int expected {60};

        CHECK_EQ(expected, maxTripProduct(n, maxTripletBruteBC));
        CHECK_EQ(expected, maxTripProduct(n, maxTripletBruteA));
        CHECK_EQ(expected, maxTripProduct(n, maxTripletOptimised));
    }

    TEST_CASE("with mid constraints") {
        unsigned short nValues[] {90, 1000};
        int expected[] {21060, 31'875'000};

        for (const auto& n : nValues) {
            auto i = &n - &nValues[0];
            CHECK_EQ(expected[i], maxTripProduct(n, maxTripletBruteBC));
            CHECK_EQ(expected[i], maxTripProduct(n, maxTripletBruteA));
            CHECK_EQ(expected[i], maxTripProduct(n, maxTripletOptimised));
        }
    }

    TEST_CASE("with upper constraints") {
        unsigned short n {3000};
        int expected {937'500'000};

        CHECK_EQ(expected, maxTripProduct(n, maxTripletBruteBC));
        CHECK_EQ(expected, maxTripProduct(n, maxTripletBruteA));
        CHECK_EQ(expected, maxTripProduct(n, maxTripletOptimised));
    }
}