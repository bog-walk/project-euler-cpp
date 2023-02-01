/*
 * Problem 36: Double-Base Palindromes
 *
 * https://projecteuler.net/problem=36
 *
 * Goal: Find the sum of all natural numbers (without leading zeroes) less than N that
 * are palindromic in both base 10 and base K.
 *
 * Constraints: 10 <= N <= 1e6, 2 <= K <= 9
 *
 * e.g.: N = 10, K = 2
 *       result = {(1, 1), (3, 11), (5, 101), (7, 111), (9, 1001)}
 *       sum = 25
 */

#include <string>

#include "../../doctest/doctest.h"

#include "pe-strings/palindrome.h"

// std::stoull has base arg but keeps throwing exceptions?
std::string numberToBase(unsigned long num, unsigned short base)
{
    std::string baseK;

    while (num) {
        char digit = '0' + (num % base);
        baseK.insert(0, 1, digit);
        num /= base;
    }

    return baseK;
}

unsigned long sumOfPalindromesBrute(unsigned long n, unsigned short k)
{
    unsigned long sum {};

    for (unsigned long i {1}; i < n; ++i) {
        if (isPalindromeNumber(i)) {
            auto strBaseK = numberToBase(i, k);
            if (isPalindrome(strBaseK))
                sum += i;
        }
    }

    return sum;
}

/*
 * Returns the decimal representation of the nth odd-/even-length palindrome in the
 * specified base.
 * e.g. The 2nd odd-length base 2 palindrome is 101 == 5.
 */
unsigned long getPalindrome(unsigned long num, unsigned short base, bool oddLength = true)
{
    unsigned long n {num};
    if (oddLength)  // base 2 -> n shr 1
        n /= base;

    while (n) {
        // base 2 -> palindrome shl 1 + n and 1
        num = num * base + n % base;
        // base 2 -> n shr 1
        n /= base;
    }

    return num;
}

/*
 * Solution is optimised by only iterating over generated base-k palindromes less than N.
 *
 * This also means that, unlike in the brute force solution, only 1 number (the base-10
 * result) needs to be checked as a palindrome.
 */
unsigned long sumOfPalindromes(unsigned long n, unsigned short k)
{
    unsigned long sum {};

    bool oddTurn {true};
    // generate both odd & even length palindromes
    for (int r {0}; r < 2; ++r) {
        int i {1};
        // generate decimal representation of base-k palindrome
        auto decimalRepr = getPalindrome(i, k, oddTurn);
        do {
            // check if decimal is also a palindrome
            if (isPalindromeNumber(decimalRepr))
                sum += decimalRepr;
            i++;
            // locally initialised var not available as do-while condition?
            decimalRepr = getPalindrome(i, k, oddTurn);
        } while (decimalRepr < n);
        oddTurn = false;
    }

    return sum;
}

TEST_CASE("test lower constraints") {
    unsigned long n {10};
    unsigned short kValues[] {2, 4, 6, 8};
    unsigned long long expected[] {25, 11, 22, 37};

    for (const auto& k: kValues) {
        auto i = &k - &kValues[0];
        CHECK_EQ(expected[i], sumOfPalindromesBrute(n, k));
        CHECK_EQ(expected[i], sumOfPalindromes(n, k));
    }
}

TEST_CASE("test mid constraints") {
    unsigned long n {1000};
    unsigned short kValues[] {3, 5, 7};
    unsigned long long expected[] {2638, 1940, 855};

    for (const auto& k: kValues) {
        auto i = &k - &kValues[0];
        CHECK_EQ(expected[i], sumOfPalindromesBrute(n, k));
        CHECK_EQ(expected[i], sumOfPalindromes(n, k));
    }
}

TEST_CASE("test upper constraints") {
    unsigned long nValues[] {1'000'000, 1'000'000, 1'000'000'000};
    unsigned short kValues[] {2, 9, 2};
    unsigned long long expected[] {872'187, 782'868, 2'609'044'274};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumOfPalindromesBrute(n, kValues[i]));
        CHECK_EQ(expected[i], sumOfPalindromes(n, kValues[i]));
    }
}