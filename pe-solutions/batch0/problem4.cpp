/*
 * Problem 4: Largest Palindrome Product
 *
 * https://projecteuler.net/problem=4
 *
 * Goal: Find the largest palindrome less than N that is made from the product
 * of two 3-digit numbers.
 *
 * Constraints: 101_101 < N < 1e6
 *
 * e.g.: N = 800_000
 *       869 * 913 = 793_397
 */

#include <string>

#include "../../doctest/doctest.h"

#include "pe-strings/palindrome.h"

/*
 * Brute iteration through all products of 3-digit numbers while storing the largest
 * confirmed palindrome product so far.
 */
unsigned long largestPalindromeProductBrute(unsigned long n)
{
    unsigned long largest {};

    for (int x {101}; x <= 999; ++x) {
        for (int y {x}; y <= 999; ++y) {
            auto product = x * y;
            if (product >= n)
                break;
            if (product > largest && isPalindrome(std::to_string(product)))
                largest = product;
        }
    }

    return largest;
}

/*
 * Brute iteration through all products of 3-digit numbers starting with the largest
 * numbers and terminating the inner loop early if product starts to get too small.
 */
unsigned long largestPalindromeProductBruteBackwards(unsigned long n)
{
    unsigned long largest {};

    for (int x {999}; x >= 101; --x) {
        for (int y {999}; y >= x; --y) {
            auto product = x * y;
            // combo will be too small to pursue further
            if (product <= largest)
                break;
            if (product < n && isPalindrome(std::to_string(product))) {
                largest = product;
                break;
            }
        }
    }

    return largest;
}

/*
 * A palindrome of the product of two 3-digit integers can be at most 6-digits long &
 * one of the integers must have a factor of 11, based on the following algebra:
 *
 *      P = 100_000x + 10_000y + 1000z + 100z + 10y + x
 *      P = 100_001x + 10_010y + 1100z
 *      P = 11*(9091x + 910y + 100z)
 *
 * Rather than stepping down to each palindrome & searching for a valid product, this
 * solution tries all product combos that involve one of the integers being a multiple
 * of 11.
 */
unsigned long largestPalindromeProduct(unsigned long n)
{
    unsigned long largest {};

    for (int x {990}; x >= 110; x -= 11) {
        for (int y {999}; y >= 101; --y) {
            auto product = x * y;
            // combo will be too small to pursue further
            if (product <= largest)
                break;
            if (product < n && isPalindrome(std::to_string(product))) {
                largest = product;
                break;
            }
        }
    }

    return largest;
}

/*
 * Converts a 3-digit integer to a 6-digit palindrome.
 */
inline unsigned long getPalindrome(unsigned long n)
{
    return n * 1000 + n % 10 * 100 + n / 10 % 10 * 10 + n / 100;
}

bool is3DigProduct(unsigned long n)
{
    for (int factor1 {999}; factor1 * factor1 >= n; --factor1) {
        if (!(n % factor1)) {
            auto factor2 = n / factor1;
            // equivalent to (f2-101) * (f2-999) <= 0
            if (101 <= factor2 && factor2 <= 999)
                return true;
        }
    }

    return false;
}

/*
 * Brute iteration through all palindromes less than n checks if each palindrome
 * could be a valid product of two 3-digit numbers.
 */
unsigned long largestPalindromeProductAlt(unsigned long n)
{
    unsigned long num = n / 1000;
    if (getPalindrome(num) > n)
       num--;

    while (num > 101) {
        auto palindrome = getPalindrome(num);
        if (is3DigProduct(palindrome))
            return palindrome;
        num--;
    }

    return 101'101;
}

TEST_CASE("test lower constraints") {
    unsigned long nValues[] {101'102, 101'110};
    unsigned long expected[] {101'101, 101'101};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], largestPalindromeProductBrute(n));
        CHECK_EQ(expected[i], largestPalindromeProductBruteBackwards(n));
        CHECK_EQ(expected[i], largestPalindromeProduct(n));
        CHECK_EQ(expected[i], largestPalindromeProductAlt(n));
    }
}

TEST_CASE("test mid constraints") {
    unsigned long nValues[] {794'000, 650'001};
    unsigned long expected[] {793'397, 649'946};

    for (const auto& n : nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], largestPalindromeProductBrute(n));
        CHECK_EQ(expected[i], largestPalindromeProductBruteBackwards(n));
        CHECK_EQ(expected[i], largestPalindromeProduct(n));
        CHECK_EQ(expected[i], largestPalindromeProductAlt(n));
    }
}

TEST_CASE("test when N is palindrome") {
    unsigned long n {332'223}, expected {330'033};

    CHECK_EQ(expected, largestPalindromeProductBrute(n));
    CHECK_EQ(expected, largestPalindromeProductBruteBackwards(n));
    CHECK_EQ(expected, largestPalindromeProduct(n));
    CHECK_EQ(expected, largestPalindromeProductAlt(n));
}

TEST_CASE("test upper constraints") {
    unsigned long n {999'999}, expected {906'609};

    CHECK_EQ(expected, largestPalindromeProductBrute(n));
    CHECK_EQ(expected, largestPalindromeProductBruteBackwards(n));
    CHECK_EQ(expected, largestPalindromeProduct(n));
    CHECK_EQ(expected, largestPalindromeProductAlt(n));
}