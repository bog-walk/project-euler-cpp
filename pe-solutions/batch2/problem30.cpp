/*
 * Problem 30: Digit Fifth Powers
 *
 * https://projecteuler.net/problem=30
 *
 * Goal: Calculate the sum of all numbers that can be written as the sum of the Nth
 * power of their digits.
 *
 * Constraints: 3 <= N <= 6
 *
 * e.g.: N = 4
 *       1634 = 1^4 + 6^4 + 3^4 + 4^4
 *       8208 = 8^4 + 2^4 + 0^4 + 8^4
 *       9474 = 9^4 + 4^4 + 7^4 + 4^4
 *       sum = 1634 + 8208 + 9474 = 19316
 */

#include <cmath>
#include <numeric>

#include "../../doctest/doctest.h"

#include "pe-combinatorics/combinations-with-repl.h"

std::vector<unsigned long> digitNthPowersBrute(unsigned long n)
{
    std::vector<unsigned long> nums;
    std::vector<unsigned long> powers(10);
    int i {};
    std::generate_n(powers.begin(), 10, [&i, &n]{
        return std::pow(i++, n);
    });

    auto start = std::max(100uL, powers[n-2]);
    auto end = static_cast<unsigned long>(std::min(999999.0, std::pow(9, n) * n));
    for (unsigned long num {start}; num < end; ++num) {
        auto digits {num};
        unsigned long sumOfPowers {};
        while (digits) {
            sumOfPowers += powers[digits%10];
            if (sumOfPowers > num)
                break;
            digits /= 10;
        }
        if (sumOfPowers == num) {
            nums.push_back(num);
        }
    }

    return nums;
}

/*
 * Considers all combinations of digits (0-9 with replacement) for max number of digits
 * that allow valid candidates, using a combinatorics algorithm.
 *
 * This algorithm returns all possible subsets, allowing element repetitions, but not
 * allowing arrangements that are identical except for order. This produces
 * significantly fewer subsets than a Cartesian product algorithm, which would not
 * differ between, e.g., 122 and 212. It is redundant to check both these numbers as
 * both would return the same comboSum (due to commutative addition).
 *
 * Instead, if the generated comboSum itself produces an identical comboSum, then it is
 * a valid number to include in the sum.
 */
std::vector<unsigned long> digitNthPowers(unsigned long n)
{
    std::vector<unsigned long> nums;
    std::vector<unsigned long> powers(10);
    int i {};
    std::generate_n(powers.begin(), 10, [&i, &n]{
        return std::pow(i++, n);
    });

    int maxDigits = n < 5 ? n : 6;
    std::vector<int> digits {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    CombinationsWithRepl<int> combos {maxDigits, digits};
    while (combos.hasNext()) {
        auto combo = combos.next();
        auto comboSum = std::accumulate(
                combo.cbegin(),
                combo.cend(),
                0uL,
                [&powers](unsigned long acc, int digit) {
                    return acc + powers[digit];
                });
        if (comboSum > 9) {
            auto comboSumAsString = std::to_string(comboSum);
            auto comboSum2 = std::accumulate(
                    comboSumAsString.cbegin(),
                    comboSumAsString.cend(),
                    0uL,
                    [&powers](unsigned long acc, char digit) {
                        return acc + powers[(digit - '0')];
                    });
            if (comboSum == comboSum2)
                nums.push_back(comboSum);
        }
    }

    std::sort(nums.begin(), nums.end());
    return nums;
}

TEST_CASE("test all constraints") {
    std::vector<std::vector<unsigned long>> expected {{153, 370, 371, 407},
                                                      {1634, 8208, 9474},
                                                      {4150, 4151, 54748, 92727, 93084,
                                                       194'979}, {548'834}};

    for (int n {3}; n <= 6; ++n) {
        CHECK_EQ(expected[n-3], digitNthPowersBrute(n));
        CHECK_EQ(expected[n-3], digitNthPowers(n));
    }
}