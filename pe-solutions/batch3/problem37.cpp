/*
 * Problem 37: Truncatable Primes
 *
 * https://projecteuler.net/problem=37
 *
 * Goal: Find the sum of all primes less than N that are truncatable both from left to
 * right and from right to left (single-digit primes are not considered).
 *
 * Constraints: 100 <= N <= 1e6
 *
 * Truncatable Prime: A prime that remains prime as digits are continuously removed
 * from LTR or RTL.
 * e.g. 3797 -> 797 -> 97 -> 7 and 3797 -> 379 -> 37 -> 3
 *
 * e.g.: N = 50
 *       truncatable primes = {23, 37}
 *       sum = 60
 */

#include <algorithm>
#include <cmath>
#include <string>

#include "../../doctest/doctest.h"

#include "pe-maths/primes.h"

/*
 * Solution speed optimised based on the following:
 *
 *      - There are only 11 such qualifying numbers.
 *
 *      - A number must start and end with a single-digit prime.
 *
 *      - No point in considering double-digit primes less than 23.
 *
 *      - Above 100, pattern shows that qualifying numbers must start and end in a 3 or 7.
 *
 *      - Above 1000, pattern shows that qualifying numbers must have their first &
 *      last 3 digits be a prime number.
 *
 *      - No need to check first & last digits again in final loop.
 *
 *      - A binary search algorithm is used to check the primes list.
 */
unsigned long sumOfTruncPrimes(unsigned long n)
{
    auto primes = primeNumbers(n - 1);
    unsigned long sum {}, count {};
    const std::string group1 {"2357"}, group2 {"37"};
    auto characterFound = [](const std::string& toCheck, const char ch) {
        return std::find(toCheck.cbegin(), toCheck.cend(), ch) != toCheck.cend();
    };

    for (const auto& prime : primes) {
        if (prime < 23)
            continue;
        std::string p = std::to_string(prime);
        auto digits = p.length();
        if (digits == 2) {
            if (!characterFound(group1, p.front()) ||
                !characterFound(group1, p.back()))
                continue;
        }
        else {
            if (!characterFound(group2, p.front()) ||
                !characterFound(group2, p.back()))
                continue;
            if (digits >= 4) {
                auto sub = std::stoul(p.substr(0, 3));
                if (!std::binary_search(primes.cbegin(), primes.cend(), sub))
                    continue;
                sub = std::stoul(p.substr(digits - 3));
                if (!std::binary_search(primes.cbegin(), primes.cend(), sub))
                    continue;
            }
        }
        if (digits > 2) {
            bool shouldContinue {false};
            for (int i {2}; i < digits; ++i) {
                auto sub = std::stoul(p.substr(0, i));
                if (!std::binary_search(primes.cbegin(), primes.cend(), sub)) {
                    shouldContinue = true;
                    break;
                }
                sub = std::stoul(p.substr(digits - i));
                if (!std::binary_search(primes.cbegin(), primes.cend(), sub)) {
                    shouldContinue = true;
                    break;
                }
            }
            if (shouldContinue)
                continue;
        }
        sum += prime;
        count++;
        if (count == 11)
            break;
    }

    return sum;
}

TEST_CASE("test all constraints") {
    unsigned long long expected[] {186, 1986, 8920, 8920, 748'317};

    for (int e {2}; e <= 6; ++e) {
        auto n = static_cast<unsigned long>(std::pow(10.0, e));
        CHECK_EQ(expected[e-2], sumOfTruncPrimes(n));
    }
}