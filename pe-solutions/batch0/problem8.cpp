/**
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

#include "../../doctest/doctest.h"
#include "pe-tests/get-test-resource.h"
#include <algorithm>
#include <string>
#include <vector>

// The constraints of this solution ensure that [series] will not exceed 13
// characters, so the max product of 13 '9's would be less than ULLONG_MAX.
unsigned long long stringProduct(const std::string& series)
{
    //Possible to achieve with a single std::accumulate() but this function
    // does not allow (?) premature exit if a '0' is encountered
    unsigned long long product {1};
    for (char ch : series) {
        if (ch == '0')
            return 0;
        product *= (ch - 48);  // ASCII value of 0 is 48
    }
    return product;
}

unsigned long long largestSeriesProductRecursive(
        const std::string& number,
        unsigned short n,
        unsigned short k
        )
{
    if (n == 1)
        return std::stoull(number);

    if (k == 1) {
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

unsigned long long largestSeriesProduct(
        const std::string& number,
        unsigned short n,
        unsigned short k
)
{
    if (n == 1)
        return std::stoull(number);

    if (n == k)
        return stringProduct(number);

    unsigned long long largest {0};
    for (int i = 0; i <= n - k; i++) {
        largest = std::max(
                largest,
                stringProduct(number.substr(i, k))
                );
    }
    return largest;
}

TEST_CASE("test helper stringProduct()") {
    std::vector<std::string> series { "8", "1234", "63972201",
                                       "1111111111111", "3675356291" };
    std::vector<unsigned long long> expected { 8, 24, 0, 1, 1'020'600 };

    for (int i = 0; i < series.size(); i++) {
        CHECK_EQ(expected[i], stringProduct(series[i]));
    }
}

TEST_SUITE("test small strings") {
    TEST_CASE("test when N == 1") {
        std::string series {"8"};
        unsigned short n {1};
        unsigned long long expected {8};

        CHECK_EQ(expected, largestSeriesProductRecursive(series, n, n));
        CHECK_EQ(expected, largestSeriesProduct(series, n, n));
    }

    TEST_CASE("test when K == 1") {
        std::string series {"63972201"};
        unsigned short n {8};
        unsigned short k {1};
        unsigned long long expected {9};

        CHECK_EQ(expected, largestSeriesProductRecursive(series, n, k));
        CHECK_EQ(expected, largestSeriesProduct(series, n, k));
    }

    TEST_CASE("test when N == K") {
        std::vector<std::string> series{ "1234", "1111111111111" };
        std::vector<unsigned short> nValues{ 4, 13 };
        std::vector<unsigned long long> expected{ 24, 1 };

        for (int i = 0; i < series.size(); i++) {
            CHECK_EQ(expected[i], largestSeriesProductRecursive(
                    series[i], nValues[i], nValues[i]
                    ));
            CHECK_EQ(expected[i], largestSeriesProduct(
                    series[i], nValues[i], nValues[i]
                    ));
        }
    }

    TEST_CASE("test when number contains '0' in all series") {
        std::string series {"2709360626"};
        unsigned short n {10};
        unsigned short k {5};
        unsigned long long expected {0};

        CHECK_EQ(expected, largestSeriesProductRecursive(series, n, k));
        CHECK_EQ(expected, largestSeriesProduct(series, n, k));
    }

    TEST_CASE("test when number contains '0' in some series") {
        std::string series {"12034"};
        unsigned short n {5};
        unsigned short k {2};
        unsigned long long expected {12};

        CHECK_EQ(expected, largestSeriesProductRecursive(series, n, k));
        CHECK_EQ(expected, largestSeriesProduct(series, n, k));
    }
    TEST_CASE("test when number is normal") {
        std::string series {"3675356291"};
        unsigned short n {10};
        unsigned short k {5};
        unsigned long long expected {3150};

        CHECK_EQ(expected, largestSeriesProductRecursive(series, n, k));
        CHECK_EQ(expected, largestSeriesProduct(series, n, k));
    }
}

TEST_CASE("test with 100 digit number") {
    unsigned short n {100};
    unsigned short k {6};
    // create a number of all '1's except for 6 adjacent '6's
    std::string series(n, '1');
    for (int i = 60; i <= 65; i++) {
        series[i] = '6';
    }
    unsigned long long expected {46656};  // 6^6

    CHECK_EQ(expected, largestSeriesProductRecursive(series, n, k));
    CHECK_EQ(expected, largestSeriesProduct(series, n, k));
}

TEST_CASE("test with 1000 digit number") {
    unsigned short n {1000};
    std::vector<unsigned short> kValues{ 13, 4 };
    std::vector<unsigned long long> expected{ 23'514'624'000, 5832};
    std::string resource = getTestString(
            "../resources/largest-product-in-series-1000.txt"
            );
    for (int i = 0; i < kValues.size(); i++) {
        CHECK_EQ(expected[i], largestSeriesProductRecursive(
                resource, n, kValues[i]
                ));
        CHECK_EQ(expected[i], largestSeriesProduct(
                resource, n, kValues[i]
                ));
    }
}