/*
 * Problem 32: Pandigital Products
 *
 * https://projecteuler.net/problem=32
 *
 * Goal: Find the sum of all products whose identity expression can be written as an
 * N-pandigital number.
 *
 * Constraints: 4 <= N <= 9
 *
 * Pandigital Number: An N-digit number that uses all digits from 1 to N exactly once,
 * e.g. N = 5 -> 15234.
 *
 * Identity expression: 39(multiplicand) * 186(multiplier) = 7254(product).
 * Therefore, 7254 is a product of a pandigital identity expression.
 *
 * e.g.: N = 4
 *       identities = {3 * 4 = 12}
 *       sum = 12
 */

#include <algorithm>
#include <numeric>
#include <set>
#include <string>

#include "../../doctest/doctest.h"

#include "pe-strings/is-pandigital.h"

/*
 * Iterative solution assesses the pandigital quality of all identity expressions
 * produced by multiplier, multiplicand combinations within a specified limit.
 */
unsigned long sumPandigitalProductsBrute(unsigned short n)
{
    // stored as set to ensure no duplicate permutation results
    std::set<unsigned long> products;

    const int multiplicandEnd = n < 7 ? 9 : 98;
    const int multiplierMax = n == 8 ? 987 : n == 9 ? 9876 : 98;
    for (unsigned long a {2}; a <= multiplicandEnd; ++a) {
        for (unsigned long b = a + 1; b <= multiplierMax; ++b) {
            auto prod = a * b;
            // is there a better way?
            auto str = std::to_string(a) + std::to_string(b) + std::to_string(prod);
            if (isPandigital(str, n))
                products.insert(prod);
        }
    }

    return std::accumulate(products.cbegin(), products.cend(), 0uL);
}

/*
 * Solution uses std::next_permutation() to assess all arrangements of the required
 * digits for a valid identity expression.
 */
unsigned long sumPandigitalProductsAllPerms(unsigned short n)
{
    // stored as set to ensure no duplicate permutation results
    std::set<unsigned long> products;

    // neither multiplicand nor multiplier can have more digits than product
    int prodMaxDigits = n < 7 ? 2 : n < 8 ? 3 : 4;
    std::string digits {"123456789"};
    digits.resize(n);
    while (std::next_permutation(digits.begin(), digits.end())) {
        // if multiplicand has 1 digit, it would be found at index 0
        for (int a {1}; a <= 2; ++a) {
            // find start index of product based on multiplicand & product digits
            auto pIndex = a + std::min(prodMaxDigits, n - prodMaxDigits - a);
            auto multiplicand = std::stoul(digits.substr(0, a));
            auto multiplier = std::stoul(digits.substr(a, pIndex - a));
            auto product = std::stoul(digits.substr(pIndex));
            if (multiplicand * multiplier == product) {
                products.insert(product);
                break;
            }
            // expressions with < 7 digits can only have multiplier be 1 digit
            if (n < 7)
                break;
        }
    }

    return std::accumulate(products.cbegin(), products.cend(), 0uL);
}

TEST_CASE("test all constraints") {
    unsigned long expected[] {12, 52, 162, 0, 13458, 45228};

    for (unsigned short n {4}; n <= 9; ++n) {
        CHECK_EQ(expected[n-4], sumPandigitalProductsBrute(n));
        CHECK_EQ(expected[n-4], sumPandigitalProductsAllPerms(n));
    }
}