/*
 * Problem 34: Digit Factorials
 *
 * https://projecteuler.net/problem=34
 *
 * Goal: Find the sum of all numbers less than N that divide the sum of the factorial
 * of their digits (& therefore have minimum 2 digits).
 *
 * Constraints: 10 <= N <= 1e5
 *
 * Factorion: A natural number that equals the sum of the factorials of its digits.
 * The only non-single-digit factorions are: 145 and 40585.
 *
 * e.g.: N = 20
 *       qualifying numbers = {19}
 *       as 1! + 9! = 362_881, which % 19 = 0
 *       e.g. 18 does not work as 1! + 8! = 40321, which % 18 > 0
 *       sum = 19
 */

#include <numeric>

#include "../../doctest/doctest.h"

#include "pe-maths/factorial.h"

/*
 * HackerRank specific implementation that finds the sum of all numbers < n that are
 * divisors of the sum of the factorials of their digits.
 *
 * Increase this solution's efficiency by creating an array of upper constraint size
 * (plus 1) & looping once through all numbers then caching the result plus the
 * previously calculated sum, if it matches the necessary requirements.
 */
unsigned long sumOfDigitFactorialsHR(unsigned long n)
{
    unsigned long sum {};
    // better to just declare a const array hard-coded in using?
    unsigned long factorials[10];
    for (int i {0}; i < 10; ++i) {
        factorials[i] = factorial(i).toULong();
    }

    for (unsigned long num {10}; num < n; ++num) {
        auto numStr = std::to_string(num);
        auto numSum = std::accumulate(
                numStr.cbegin(),
                numStr.cend(),
                0uL,
                [&factorials](unsigned long acc, const char& ch) {
                    return acc + factorials[ch - '0'];
                });
        if (!(numSum % num))
            sum += num;
    }

    return sum;
}

/*
 * Project Euler specific implementation that finds the sum of all numbers that are
 * factorions.
 *
 * The numbers cannot have more than 7 digits, as 9! * 8 returns only a 7-digit number.
 *
 * 9! * 7 returns 2_540_160, so the 1st digit of the 7-digit number cannot be greater
 * than 2.
 */
unsigned long sumOfDigitFactorialsPE()
{
    unsigned long sum {};
    // better to just declare a const array hard-coded in using?
    unsigned long factorials[10];
    for (int i {0}; i < 10; ++i) {
        factorials[i] = factorial(i).toULong();
    }

    const int limit {2'000'000};
    for (int num {10}; num < limit; ++num) {
        unsigned long numSum {};
        auto digits = std::to_string(num);
        for (const auto& ch : digits) {
            numSum += factorials[ch-'0'];
            // prevents further unnecessary calculation
            if (numSum > num)
                break;
        }
        if (numSum == num)
            sum += numSum;
    }

    return sum;
}

TEST_CASE("test HR problem with lower constraints") {
    unsigned long nValues[] {10, 20, 30, 40, 50};
    unsigned long expected[] {0, 19, 19, 19, 19};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumOfDigitFactorialsHR(n));
    }
}

TEST_CASE("test HR problem with mid constraints") {
    unsigned long nValues[] {100, 200, 500};
    unsigned long expected[] {239, 384, 603};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumOfDigitFactorialsHR(n));
    }
}

TEST_CASE("test PE problem") {
    unsigned long expected {40730};

    CHECK_EQ(expected, sumOfDigitFactorialsPE());
}