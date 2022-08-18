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

#include <vector>

#include "../../doctest/doctest.h"

#include "pe-custom/rolling-queue.h"
#include "pe-tests/get-test-resource.h"

/**
 * Solution simulates manual addition from RTL, using custom RollingQueue class to
 * abstract away the need to maintain output length with every iteration.
 */
std::string addInReverse(const std::vector<std::string>& numbers)
{
    const auto n = numbers.size();
    if (n == 1)
        return numbers.front().substr(0, 10);

    RollingQueue<unsigned long> output {10};
    unsigned short carryOver {0};
    // std::size_t is unsigned long long
    // standard for-loop causes infinite loop as unsigned numbers always non-negative
    // n.b post-fix decrementor as 2nd not 3rd arg, otherwise final step missed
    for (std::size_t i = numbers.front().size(); i--;) {
        unsigned short sum {0};
        for (int j {0}; j < n; ++j) {
            auto num = numbers[j][i] - 48;  // ASCII value of '0'
            sum += num;
        }
        sum += carryOver;
        output.push(sum % 10);
        carryOver = sum / 10;
    }

    while (carryOver) {
        output.push(carryOver % 10);
        carryOver /= 10;
    }

    output.reversed();
    return output.toString();
}

TEST_CASE("test when N = 1") {
    const std::vector<std::string> numbers {"123456789123456789"};
    const std::string expected {"1234567891"};

    CHECK_EQ(expected, addInReverse(numbers));
}

TEST_CASE("test when numbers have few digits") {
    const std::vector<std::string> numbers {"123", "456", "789", "812", "234"};
    const std::string expected {"2414"};

    CHECK_EQ(expected, addInReverse(numbers));
}

TEST_CASE("test when numbers have medium amount of digits") {
    const std::vector<std::string> numbers {"6041184107", "5351558590", "1833324270"};
    const std::string expected {"1322606696"};

    CHECK_EQ(expected, addInReverse(numbers));
}

TEST_CASE("test with max digits and lower N") {
    const std::vector<std::string> numbers = getTestResource(
            "../resources/large-sum-5N.txt");
    const std::string expected {"2728190129"};

    CHECK_EQ(expected, addInReverse(numbers));
}
/*
TEST_CASE("test with max digits and mid N") {
    const std::vector<std::string> numbers = getTestResource(
            "../resources/large-sum-100N.txt");
    const std::string expected {"?"};

    CHECK_EQ(expected, addInReverse(numbers));
}

TEST_CASE("test with max digits and upper N") {
    const std::vector<std::string> numbers = getTestResource(
            "../resources/large-sum-1000N.txt");
    const std::string expected {"?"};

    CHECK_EQ(expected, addInReverse(numbers));
}*/