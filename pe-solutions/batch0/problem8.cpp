/*
 * Problem 8: Largest Product in a Series
 *
 * https://projecteuler.net/problem=8
 *
 * Goal: Find the largest product of K adjacent digits in an N-digit number.
 *
 * Constraints: 1 <= K <= 13, K <= N <= 1000
 *
 * e.g.: N = 10 with input = "3675356291", K = 5
 *       products LTR = {1890, 3150, 3150, 900, 1620, 540}
 *       largest = 3150 -> {6*7*5*3*5} or {7*5*3*5*6}
 *
 */

#include <algorithm>
#include <string>

#include "../../doctest/doctest.h"

#include "pe-tests/get-test-resource.h"

/**
 * The constraints of this solution ensure that [series] will not exceed 13 characters,
 * so the max product of 13 '9's would be less than ULLONG_MAX.
 */
unsigned long long stringProduct(std::string_view series)
{
    // Possible to achieve with a single std::accumulate() but latter
    // does not allow (?) premature exit if character '0' is encountered
    unsigned long long product {1uLL};

    for (const char& ch : series) {
        if (ch == '0')
            return 0;
        product *= ch - '0';
    }

    return product;
}

unsigned long long largestSeriesProductRecursive(std::string_view number,
                                                 unsigned short n, unsigned short k)
{
    if (n == 1)
        return std::stoull(static_cast<std::string>(number));

    if (k == 1) {
        // ASCII value of 0 is 48
        return *std::max_element(number.cbegin(), number.cend()) - 48;
    }

    if (n == k)
        return stringProduct(number);

    return std::max(
            // first substring with k-adjacent digits
            largestSeriesProductRecursive(number.substr(0, k), k, k),
            // original string minus the first digit
            // substr() default 2nd arg is remainder after 1st arg index
            largestSeriesProductRecursive(number.substr(1), n - 1, k)
            );
}

unsigned long long largestSeriesProduct(std::string_view number, unsigned short n,
                                        unsigned short k)
{
    if (n == 1)
        return std::stoull(static_cast<std::string>(number));

    if (n == k)
        return stringProduct(number);

    unsigned long long largest {};

    for (int i {0}; i <= n - k; ++i) {
        largest = std::max(largest, stringProduct(number.substr(i, k)));
    }

    return largest;
}

TEST_CASE("test helper stringProduct()") {
    std::string series[] {"8", "1234", "63972201", "1111111111111",
                          "3675356291"};
    unsigned long long expected[] {8, 24, 0, 1, 1'020'600};

    for (const auto& s : series) {
        auto i = &s - &series[0];
        CHECK_EQ(expected[i], stringProduct(s));
    }
}

TEST_SUITE("test small strings") {
    TEST_CASE("when N == 1") {
        std::string_view series {"8"};
        unsigned short n {1};
        unsigned long long expected {8};

        CHECK_EQ(expected, largestSeriesProductRecursive(series, n, n));
        CHECK_EQ(expected, largestSeriesProduct(series, n, n));
    }

    TEST_CASE("when K == 1") {
        std::string_view series {"63972201"};
        unsigned short n {8}, k {1};
        unsigned long long expected {9};

        CHECK_EQ(expected, largestSeriesProductRecursive(series, n, k));
        CHECK_EQ(expected, largestSeriesProduct(series, n, k));
    }

    TEST_CASE("when N == K") {
        std::string_view series[] {"1234", "1111111111111"};
        unsigned short nValues[] {4, 13};
        unsigned long long expected[] {24, 1};

        for (const auto& s : series) {
            auto i = &s - &series[0];
            CHECK_EQ(expected[i],
                     largestSeriesProductRecursive(s, nValues[i], nValues[i]));
            CHECK_EQ(expected[i],
                     largestSeriesProduct(s, nValues[i], nValues[i]));
        }
    }

    TEST_CASE("when number contains '0' in all series") {
        std::string_view series {"2709360626"};
        unsigned short n {10}, k {5};
        unsigned long long expected {0};

        CHECK_EQ(expected, largestSeriesProductRecursive(series, n, k));
        CHECK_EQ(expected, largestSeriesProduct(series, n, k));
    }

    TEST_CASE("when number contains '0' in some series") {
        std::string_view series {"12034"};
        unsigned short n {5}, k {2};
        unsigned long long expected {12};

        CHECK_EQ(expected, largestSeriesProductRecursive(series, n, k));
        CHECK_EQ(expected, largestSeriesProduct(series, n, k));
    }

    TEST_CASE("when number is normal") {
        std::string_view series {"3675356291"};
        unsigned short n {10}, k {5};
        unsigned long long expected {3150};

        CHECK_EQ(expected, largestSeriesProductRecursive(series, n, k));
        CHECK_EQ(expected, largestSeriesProduct(series, n, k));
    }
}

TEST_CASE("test 100 digit number") {
    unsigned short n {100}, k {6};
    // create a number of all '1's except for 6 adjacent '6's
    std::string series(n, '1');
    for (int i {60}; i <= 65; ++i) {
        series[i] = '6';
    }
    unsigned long long expected {46656};  // 6^6

    CHECK_EQ(expected, largestSeriesProductRecursive(series, n, k));
    CHECK_EQ(expected, largestSeriesProduct(series, n, k));
}

TEST_CASE("test 1000 digit number") {
    unsigned short n {1000};
    unsigned short kValues[] {13, 4};
    unsigned long long expected[] {23'514'624'000, 5832};
    const std::string resource = getTestString(
            "../resources/largest-product-in-series-1000.txt"
            );

    for (const auto& k : kValues) {
        auto i = &k - &kValues[0];
        CHECK_EQ(expected[i], largestSeriesProductRecursive(resource, n, k));
        CHECK_EQ(expected[i], largestSeriesProduct(resource, n, k));
    }
}