/*
 * Problem 40: Champernowne's Constant
 *
 * https://projecteuler.net/problem=40
 *
 * Goal: Calculate the value of the expression d_i1 * d_i2 * .. d_i7 if d_in represents
 * the nth digit of the fractional part of Champernowne's Constant.
 *
 * Constraints: 1 <= i1..i7 <= 1e18
 *
 * Champernowne's Constant: The irrational decimal fraction created by concatenating
 * all positive integers to the right of a decimal point, such that:
 *
 *      C = 0.123456789101112131415161718192021...
 *
 * e.g.: i1..i7 = 1 2 3 4 5 6 7
 *       expression = 1 * 2 * 3 * 4 * 5 * 6 * 7
 *       result = 5040
 */

#include <cmath>
#include <string>
#include <vector>

#include "../../doctest/doctest.h"

/*
 * Assuming that the positive integers of Champernowne's Constant are separated into
 * series representing the number of digits of each successive integer, k, and the
 * range of each series is calculated by [10^(k-1), (10^k) - 1], then each series will
 * have 9 * 10^(k-1) terms and, thereby, a total of k * 9 * 10^(k-1) digits.
 *
 * The cumulative total digits per series can be used to determine in which series
 * range the requested index sits as well as its position in the series.
 *
 * e.g. Searching for the 2000th digit in Champernowne's Constant (C):
 *
 *      - 2000 - 180 - 9 = 1811 < 2700.
 *
 *      - Therefore, it is found in the 1811th digit in series 3.
 *
 *      - 1811 is first zero-indexed (as C does not start with 0).
 *
 *      - As series 3 has 3 digits in every term, 1810 / 3 gives the location of the
 *      1810th digit as being in the 603rd term -> 100 + 603 = 703.
 *
 *      - 1810 % 3 = 1, so the 1810th digit is at index 1 of 703.
 *
 *      - Therefore, the 2000th digit in C is 0.
 */
unsigned long getConstant(unsigned long long index, const unsigned long long* magnitudes,
                          const unsigned long long* kDigits)
{
    unsigned long k {1};
    while (index > kDigits[k-1]) {
        // final reduced index represents the ith digit in the kth series
        index -= kDigits[k-1];
        k++;
    }
    index -= 1;  // zero-indexed
    // kTerm represents the ordinal position in the series
    // termI represents the index of the digit within the term found below
    auto kTerm = index / k;
    auto termI = index % k;
    std::string term = std::to_string(magnitudes[k-1] + kTerm);

    return term[termI] - '0';
}

unsigned long champernownesProduct(const std::vector<unsigned long long>& digits)
{
    // increase efficiency by pre-computing magnitudes & amount of digits in all series
    unsigned long long magnitudes[17], kDigits[17];
    for (int i {0}; i < 17; ++i) {
        magnitudes[i] = static_cast<unsigned long long>(std::pow(10.0, i));
        kDigits[i] = (i + 1) * 9 * magnitudes[i];
    }

    unsigned long product {1};

    for (const auto& digit : digits) {
        product *= getConstant(digit, magnitudes, kDigits);
    }

    return product;
}

TEST_CASE("test getConstant()") {
    std::vector<unsigned long long> indices[] {{1}, {10}, {33}, {65},
                                               {90}, {145}, {200}, {298},
                                               {1'000'000'000'000},
                                               {999'999'999'934'999'995},
                                               {1'000'000'000'000'000'000}};
    unsigned long expected[] {1, 1, 1, 7, 5, 7, 0, 1,
                              1, 7, 3};

    for (const auto& index: indices) {
        auto i = &index - &indices[0];
        CHECK_EQ(expected[i], champernownesProduct(index));
    }
}

TEST_SUITE("test champernownesProduct()") {
    TEST_CASE("with lower constraints") {
        std::vector<unsigned long long> digits[] {{1, 2, 3,  4,  5,  6,  7},
                                                  {8, 9, 10, 11, 12, 13, 14},
                                                  {1, 5, 10, 15, 20, 25, 30}};
        unsigned long expected[] {5040, 0, 140};

        for (const auto& digit: digits) {
            auto i = &digit - &digits[0];
            CHECK_EQ(expected[i], champernownesProduct(digit));
        }
    }

    TEST_CASE("with mid constraints") {
        std::vector<unsigned long long> digits[] {{10, 20, 30, 40, 50, 60, 70},
                                                  {11, 21, 31, 41, 51, 61, 71},
                                                  {1, 2, 4, 8, 16, 32, 64, 128}};
        unsigned long expected[] {144, 0, 2304};

        for (const auto& digit: digits) {
            auto i = &digit - &digits[0];
            CHECK_EQ(expected[i], champernownesProduct(digit));
        }
    }

    TEST_CASE("with upper constraints") {
        std::vector<unsigned long long> small(7);
        std::vector<unsigned long long> large(7);
        for (int e {0}; e < 7; ++e) {
            small[e] = static_cast<unsigned long long>(std::pow(10.0, e));
            large[e] = static_cast<unsigned long long>(std::pow(10.0, e + 12));
        }

        std::vector<unsigned long long> digits[] {small, large,
                                                  {9'999'999'999'999'995,
                                                   999'999'999'934'999'995,
                                                   999'992'599'999'999'996,
                                                   999'991'999'999'998,
                                                   999'999'999'999'999'999,
                                                   999'123'999'999'999'999,
                                                   1'000'000'000'000'000'000}};
        unsigned long expected[] {210, 5040, 370'440};

        for (const auto& digit: digits) {
            auto i = &digit - &digits[0];
            CHECK_EQ(expected[i], champernownesProduct(digit));
        }
    }
}