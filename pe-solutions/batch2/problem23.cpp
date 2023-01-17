/*
 * Problem 23: Non-Abundant Sums
 *
 * https://projecteuler.net/problem=23
 *
 * Goal: Return whether N can be expressed as the sum of 2 abundant numbers.
 *
 * Constraints: 0 <= N <= 1e5
 *
 * Perfect Number: The sum of its proper divisors equals itself.
 * e.g. properD(6) = sum{1,2,3} = 6
 *
 * Deficient Number: The sum of its proper divisors is less than itself.
 * e.g. properD(4) = sum{1, 2} = 3
 *
 * Abundant Number: The sum of its proper divisors exceeds itself.
 * e.g. properD(12) = sum{1,2,3,4,6} = 16
 *
 * By mathematical analysis, all integers > 28123 can be expressed as the sum of 2
 * abundant numbers. This upper limit cannot, however, be reduced further even though
 * it is known that the largest number that cannot be expressed as the sum of 2
 * abundant numbers is less than this upper limit.
 *
 * e.g.: N = 24
 *       smallest abundant number = 12,
 *       so smallest integer that can be expressed = 12 + 12 = 24
 *       result = True
 */

#include <vector>

#include "../../doctest/doctest.h"

#include "pe-maths/sum-proper-divisors.h"

inline bool isAbundant(unsigned long num) { return sumProperDivisors(num) > num; }

/*
 * Solution is optimised based on:
 *
 *  - 12 being the smallest abundant number to exist, so the smallest integer expressed
 *  as a sum of abundants is 24.
 *
 *  - 945 being the smallest odd abundant number.
 *
 *  - An odd number has to be the sum of an even & odd number, so all odd numbers under
 *  957 (945 + 12) cannot be the sum of abundants, since all other abundants below 945
 *  are even.
 *
 *  - All integers > 20161 can be expressed as the sum of 2 abundant numbers, as shown
 *  in the final test case.
 *
 *  - xMax of x + y = N would be N / 2, to avoid duplicate checks.
 */
bool isSumOfAbundants(unsigned long n)
{
    if (n < 24 || (n < 957 && n % 2))
        return false;
    if (n > 20161)
        return true;

    auto xMax = n / 2;
    for (int x {12}, step {2}; x <= xMax; x += step) {
        // could consider removing prime numbers > 944 from loop as primes cannot be
        // abundant numbers, but the speed difference is negligible
        if (x == 944)
            step = 1;
        if (isAbundant(x) && isAbundant(n - x))
            return true;
    }

    return false;
}

/*
 * Project Euler specific implementation meant to find the sum of all positive integers
 * that cannot be written as the sum of 2 abundant numbers.
 *
 * N.B. The final test case shows that 20161 is the largest integer that cannot be
 * expressed as such, even though 28123 is provided as the upper limit.
 */
unsigned long sumOfAllNonAbundants()
{
    unsigned long sum {};

    for (unsigned long i {0}; i <= 20161; ++i) {
        if (!isSumOfAbundants(i))
            sum += i;
    }

    return sum;
}

TEST_SUITE("test isAbundant()") {
    TEST_CASE("for abundants") {
        unsigned long nValues[] {12, 18, 20, 24, 70, 104, 120,
                                 945};

        for (const auto& n: nValues) {
            CHECK(isAbundant(n));
        }
    }

    TEST_CASE("for non-abundants") {
        unsigned long nValues[] {6, 9, 21, 43, 86, 115};

        for (const auto& n: nValues) {
            CHECK_FALSE(isAbundant(n));
        }
    }
}

TEST_SUITE("test isSumOfAbundants()") {
    TEST_CASE(" with valid N") {
        unsigned long nValues[] {24, 110, 158, 234, 957, 20162,
                                 28122, 28123, 28124, 100'000};

        for (const auto& n: nValues) {
            CHECK(isSumOfAbundants(n));
        }
    }

    TEST_CASE("with invalid N") {
        unsigned long nValues[] {0, 10, 12, 13, 27, 49, 121,
                                 20161};

        for (const auto& n: nValues) {
            CHECK_FALSE(isSumOfAbundants(n));
        }
    }

    TEST_CASE("with integers above 20162") {
        std::vector<unsigned long> cannotBeExpressed;

        for (int n {20162}; n <= 28123; ++n) {
            if (!isSumOfAbundants(n))
                cannotBeExpressed.push_back(n);
        }

        CHECK(cannotBeExpressed.empty());
    }
}

TEST_CASE("test sumOfAllNonAbundants()") {
    unsigned long expected {4'179'871};

    CHECK_EQ(expected, sumOfAllNonAbundants());
}