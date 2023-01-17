/*
 * Problem 24: Lexicographic Permutations
 *
 * https://projecteuler.net/problem=24
 *
 * Goal: Return the Nth lexicographic permutation of "abcdefghijklm".
 *
 * Constraints: 1 <= N <= 13!
 *
 * Lexicographic Permutation: The alphabetically/numerically ordered arrangements of an
 * object. e.g. "abc" -> {"abc", "acb", "bac", "bca", "cab", "cba"}
 *
 * e.g.: N = 1 -> "abcdefghijklm"
 *       N = 2 -> "abcdefghijkml"
 */

#include <string>

#include "../../doctest/doctest.h"

#include "pe-maths/factorial.h"

/*
 * Recursive solution uses factorial (permutations without repetition) to calculate the
 * next character in the permutation based on batch position.
 *
 * e.g. "abcd" has 4! = 24 permutations & each letter will have 6 permutations in which
 * that letter will be the 1st in the order. If n = 13, this permutation will be in
 * batch 2 (starts with "c") at position 1 (both 0-indexed). So "c" is removed and n =
 * 1 is used with the new string "abd". This continues until n = 0 and "cabd" is returned
 * by the base case.
 *
 * N.B. The creation of a storage string to pass into every recursive call is
 * unnecessary. Also, the amount of factorial calls can be reduced since x! / x = (x - 1)!
 *
 * @param [n] the nth permutation requested should be zero-indexed.
 * @param [input] the object to generate permutations of should be already sorted in
 * ascending order.
 */
std::string lexicographicPerm(unsigned long long n, std::string input)
{
    if (input.length() == 1)
        return input;

    auto batchSize = factorial(input.length() - 1).toULLong();
    auto i = n / batchSize;
    char starter = input[i];
    input.erase(i, 1);

    return starter + lexicographicPerm(n % batchSize, input);
}

/*
 * Solution uses built-in STL function to repeatedly invoke the next permutation.
 *
 * @param [n] the nth permutation requested should be zero-indexed.
 * @param [input] the object to generate permutations of should be already sorted in
 * ascending order.
 */
std::string lexicographicPermBuiltIn(unsigned long long n, std::string input)
{
    //if (n == 0uLL)
        //return input;

    while (n--) {
        std::next_permutation(input.begin(), input.end());
    }

    return input;
}

TEST_CASE("test with short strings") {
    const std::string input {"abc"};
    std::string expected[] {"abc", "acb", "bac", "bca", "cab",
                            "cba"};

    for (unsigned long long n {0}; n <= 5; ++n) {
        CHECK_EQ(expected[n], lexicographicPerm(n, input));
        CHECK_EQ(expected[n], lexicographicPermBuiltIn(n, input));
    }
}

TEST_CASE("test with long strings") {
    const std::string input {"0123456789"};
    // 10! = 3'628'800 permutations, already 0-indexed
    unsigned long long permutations[] {0, 362'880, 1'088'640, 3'628'799,
                                       999'999};
    std::string expected[] {"0123456789", "1023456789", "3012456789",
                            "9876543210", "2783915460"};

    for (const auto& p : permutations) {
        auto i = &p - &permutations[0];
        CHECK_EQ(expected[i], lexicographicPerm(p, input));
        CHECK_EQ(expected[i], lexicographicPermBuiltIn(p, input));
    }
}