#include "../../doctest/doctest.h"
#include "pythagorean.h"
#include <numeric>
#include <stdexcept>
#include <vector>

/**
 * Two integers are co-prime (relatively/mutually prime) if the only positive
 * integer that is a divisor of both of them is 1.
 */
bool isCoPrime(unsigned long x, unsigned long y)
{
    return std::gcd(x, y) == 1;
}

/**
 * Euclid's formula generates all Pythagorean triplets from 2 numbers,
 * [m] and [n].
*
* All triplets originate from a primitive one by multiplying them by d = gcd
 * (a,b,c).
 *
 * @throws std::invalid_argument if arguments do not follow [m] > [n] > 0,
 * or if both are odd, or if they are not co-prime, i.e. gcd(m, n) != 1.
 */
std::tuple<unsigned long, unsigned long, unsigned long> pythagoreanTriplet(
        unsigned long m,
        unsigned long n,
        unsigned long d
)
{
    if (n < 1 || n >= m)
        throw std::invalid_argument(
                "Positive integers assumed to be m > n > 0");
    // C++ value 1 equates to true
    if (m % 2 && n % 2)
        throw std::invalid_argument("Both integers cannot be odd");
    if (!isCoPrime(m, n))
        throw std::invalid_argument("Positive integers must be co-prime");

    unsigned long a = (m * m - n * n) * d;
    unsigned long b = 2 * m * n * d;
    unsigned long c = (m * m + n * n) * d;

    return { std::min(a, b), std::max(a, b), c };
}

TEST_SUITE("test isCoPrime()") {
    TEST_CASE("returns true for coprime pairs") {
        std::vector<std::pair<unsigned long, unsigned long>> pairs {
                std::pair(1, 2), std::pair(2, 3),
                std::pair(3, 5), std::pair(4, 7),
                std::pair(4, 9), std::pair(5, 7),
                std::pair(5, 9), std::pair(11, 13),
                std::pair(14, 15), std::pair(17, 19),
                std::pair(29, 31), std::pair(3, 67),
                std::pair(99, 100), std::pair(34, 79),
                std::pair(54, 67)
        };

        for (auto [ x, y ] : pairs) {
            CHECK(isCoPrime(x, y));
        }
    }

    TEST_CASE("returns false for non-coprime pairs") {
        std::vector<std::pair<unsigned long, unsigned long>> pairs {
                std::pair(3, 12), std::pair(4, 18),
                std::pair(5, 5), std::pair(6, 10),
                std::pair(24, 57), std::pair(13, 117),
                std::pair(99, 102)
        };

        for (auto [ x, y ] : pairs) {
            CHECK_FALSE(isCoPrime(x, y));
        }
    }
}

TEST_SUITE("test pythagoreanTriplet()") {
    TEST_CASE("function denies invalid input") {
        unsigned long d {1};
        std::vector<std::pair<unsigned long, unsigned long>> args {
                std::pair(1, 0), std::pair(1, 10),
                std::pair(5, 3), std::pair(9, 3)
        };

        for (auto [ m, n ] : args) {
            CHECK_THROWS_AS(pythagoreanTriplet(m, n, d), std::invalid_argument);
        }
    }

    TEST_CASE("correct for valid input") {
        unsigned long d {1};
        std::vector<std::pair<unsigned long, unsigned long>> args {
                std::pair(2, 1), std::pair(3, 2),
                std::pair(4, 1), std::pair(4, 3)
        };
        std::vector<std::tuple<unsigned long, unsigned long, unsigned long>>
        expected {
                std::tuple(3, 4, 5), std::tuple(5, 12, 13),
                std::tuple(8, 15, 17), std::tuple(7, 24, 25)
        };

        for (int i = 0; i < args.size(); i++) {
            CHECK_EQ(expected[i], pythagoreanTriplet(
                    args[i].first, args[i].second, d
                    ));
        }
    }
}