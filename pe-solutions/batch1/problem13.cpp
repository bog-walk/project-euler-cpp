/*
 * Problem 13: Large Sum
 *
 * https://projecteuler.net/problem=13
 *
 * Goal: Find the first 10 digits of the sum of N 50-digit numbers.
 *
 * Constraints: 1 <= N <= 1e3
 *
 * e.g.: N.B. This is a scaled-down example (first 2 digits of N 5-digit numbers)
 *       N = 3
 *       input = [34827, 93726, 90165]
 *       sum = 218_718
 *       1st 2 digits = 21
 */

#include <numeric>

#include "../../doctest/doctest.h"

#include "pe-custom/big-int.h"
#include "pe-custom/rolling-queue.h"
#include "pe-tests/get-test-resource.h"

/*
 * Solution simulates manual addition from RTL, using custom RollingQueue class to
 * abstract away the need to maintain output length with every iteration.
 */
std::string addInReverse(const std::vector<std::string>& numbers)
{
    const int ten {10};
    const auto n = numbers.size();
    if (n == 1)
        return numbers.front().substr(0, ten);

    RollingQueue<unsigned long> output(ten);
    unsigned short carryOver {};
    // std::size_t is unsigned long long
    // standard for-loop causes infinite loop as unsigned numbers always non-negative
    // n.b post-fix decrementor as 2nd not 3rd arg, otherwise final step missed
    for (std::size_t i = numbers.front().size(); i--;) {
        unsigned short sum {};
        for (int j {0}; j < n; ++j) {
            auto num = numbers[j][i] - '0';
            sum += num;
        }
        sum += carryOver;
        output.push(sum % ten);
        carryOver = sum / ten;
    }

    while (carryOver) {
        output.push(carryOver % ten);
        carryOver /= ten;
    }

    output.reversed();
    return output.toString();
}

std::string sliceSum(const std::vector<std::string>& numbers)
{
    return std::accumulate(
            numbers.cbegin(),
            numbers.cend(),
            BigInt::zero(),
            [](BigInt& acc, std::string num) {
                return acc + BigInt {num};
            })
            .toString()
            .substr(0, 10);
}

TEST_CASE("test when N = 1") {
    const std::vector<std::string> numbers {"123456789123456789"};
    const std::string expected {"1234567891"};

    CHECK_EQ(expected, addInReverse(numbers));
    CHECK_EQ(expected, sliceSum(numbers));
}

TEST_CASE("test when numbers have few digits") {
    const std::vector<std::string> numbers {"123", "456", "789", "812", "234"};
    const std::string expected {"2414"};

    CHECK_EQ(expected, addInReverse(numbers));
    CHECK_EQ(expected, sliceSum(numbers));
}

TEST_CASE("test when numbers have medium amount of digits") {
    const std::vector<std::string> numbers {"6041184107", "5351558590", "1833324270"};
    const std::string expected {"1322606696"};

    CHECK_EQ(expected, addInReverse(numbers));
    CHECK_EQ(expected, sliceSum(numbers));
}

TEST_CASE("test with max digits and lower N") {
    const std::vector<std::string> numbers = getTestResource(
            "../resources/large-sum-5N.txt");
    const std::string expected {"2728190129"};

    CHECK_EQ(expected, addInReverse(numbers));
    CHECK_EQ(expected, sliceSum(numbers));
}

TEST_CASE("test with max digits and mid N") {
    const std::vector<std::string> numbers = getTestResource(
            "../resources/large-sum-100N.txt");

    CHECK_EQ(sliceSum(numbers), addInReverse(numbers));
}

TEST_CASE("test with max digits and upper N") {
    const std::vector<std::string> numbers = getTestResource(
            "../resources/large-sum-1000N.txt");

    CHECK_EQ(sliceSum(numbers), addInReverse(numbers));
}