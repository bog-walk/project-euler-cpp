#include "pythagorean.h"

#include <stdexcept>

#include "../../doctest/doctest.h"

/*
 * Euclid's formula generates all Pythagorean triplets from 2 numbers, m and n.
 *
 * All triplets originate from a primitive one by multiplying them by d = gcd (a,b,c).
 *
 * @throws std::invalid_argument if arguments do not follow m > n > 0, or if both
 * are odd, or if they are not co-prime, i.e. gcd(m, n) != 1.
 */
triple pythagoreanTriplet(unsigned long m, unsigned long n, unsigned long d)
{
    if (n < 1 || n >= m)
        throw std::invalid_argument("Positive integers assumed to be m > n > 0");
    if (m % 2 && n % 2)
        throw std::invalid_argument("Both integers cannot be odd");
    if (!isCoPrime(m, n))
        throw std::invalid_argument("Positive integers must be co-prime");

    auto a = (m * m - n * n) * d;
    auto b = 2 * m * n * d;
    auto c = (m * m + n * n) * d;

    return {std::min(a, b), std::max(a, b), c};
}

TEST_SUITE("test isCoPrime()") {
    TEST_CASE("returns true for coprime pairs") {
        std::pair<unsigned long, unsigned long> pairs[] {{1, 2}, {2, 3}, {3, 5},
                                                         {4, 7}, {4, 9}, {5, 7},
                                                         {5, 9}, {11, 13},
                                                         {14, 15}, {17, 19},
                                                         {29, 31}, {3, 67},
                                                         {99, 100}, {34, 79},
                                                         {54, 67}};

        for (const auto& [x, y] : pairs) {
            CHECK(isCoPrime(x, y));
        }
    }

    TEST_CASE("returns false for non-coprime pairs") {
        std::pair<unsigned long, unsigned long> pairs[] {{3, 12}, {4, 18}, {5, 5},
                                                         {6, 10}, {24, 57},
                                                         {13, 117}, {99, 102}};

        for (const auto& [x, y] : pairs) {
            CHECK_FALSE(isCoPrime(x, y));
        }
    }
}

TEST_SUITE("test pythagoreanTriplet()") {
    const unsigned long d {1};

    TEST_CASE("denies invalid input") {
        std::pair<unsigned long, unsigned long> args[] {{1, 0}, {1, 10}, {5,3},
                                                        {9,3}};

        for (const auto& [m, n] : args) {
            CHECK_THROWS_AS(pythagoreanTriplet(m, n, d), std::invalid_argument);
        }
    }

    TEST_CASE("with valid input") {
        std::pair<unsigned long, unsigned long> args[] {{2, 1}, {3, 2}, {4, 1},
                                                        {4, 3}};
        triple expected[] {{3, 4, 5}, {5, 12, 13}, {8, 15, 17}, {7, 24, 25}};

        for (const auto& p : args) {
            auto i = &p - &args[0];
            CHECK_EQ(expected[i], pythagoreanTriplet(p.first, p.second, d));
        }
    }
}