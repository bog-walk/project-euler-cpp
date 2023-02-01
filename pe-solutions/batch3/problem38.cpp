/*
 * Problem 38: Pandigital Multiples
 *
 * https://projecteuler.net/problem=38
 *
 * Goal: Find all multipliers M below N that provide a K-pandigital concatenated
 * product when used with a multiplicand starting with 1 onwards.
 *
 * Constraints: 100 <= N <= 1e5, K in {8, 9} and 1 < M
 *
 * e.g.: N = 100, K = 8
 *       multipliers = {18, 78}, since:
 *       18 * (1, 2, 3, 4) -> 18|36|54|72
 *       78 * (1, 2, 3) -> 78|156|234
 */

#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>

#include "../../doctest/doctest.h"

#include "pe-strings/is-pandigital.h"

/*
 * Since a 9-digit pandigital number is the limit, the multiplier will never be larger
 * than 4 digits (as a 5-digit number times 2 would produce another 5-digit number).
 *
 * N.B. The logic behind the inner loop breaks could all be replaced by the
 * isPandigital() helper function used in the PE implementation at the bottom.
 */
std::vector<unsigned long> findPandigitalMultipliers(unsigned long n, unsigned short k)
{
    std::vector<unsigned long> multipliers;

    const unsigned long limit = std::min(9876uL, n);
    const char kChar = '0' + k;
    for (unsigned long num {2}; num <= limit; ++num) {
        std::string concat;
        unsigned long multiplicand {1};
        while (concat.length() < k) {
            const auto product = std::to_string(num * multiplicand);
            // ensure result only 1 to k pandigital
            if (std::any_of(product.cbegin(), product.cend(),
                            [&kChar, &concat](auto& ch) {
                return ch == '0' || ch > kChar ||
                    std::find(concat.cbegin(), concat.cend(), ch) != concat.cend();
            }))
                break;
            // avoid products that contain duplicate digits
            std::unordered_set<char> productSet {product.cbegin(), product.cend()};
            if (product.length() != productSet.size())
                break;
            concat += product;
            multiplicand++;
        }
        if (concat.length() == k)
            multipliers.push_back(num);
    }

    return multipliers;
}

/*
 * Solution is identical to the above one except that no conversions to std::string or
 * std::unordered_set are used to check for duplicate digits in a number.
 *
 * Instead, 2 unsigned short values are maintained as comparison bit masks and each
 * potential number is iterated over (& decremented) right-to-left to check if each bit
 * has already been stored in the current bit mask.
 */
std::vector<unsigned long> findPandigitalBitmask(unsigned long n, unsigned short k)
{
    std::vector<unsigned long> multipliers;
    unsigned short bitMask {};  // 16 bits for 9 digits max
    for (unsigned short i {1}; i <= k; ++i) {
        // update comparison mask to make the bit 1 at digit index.
        // left shift has higher precedence than bitwise OR and |=
        bitMask |= 1 << i;
    }

    const unsigned long limit = std::min(9876uL, n);
    for (unsigned long num {2}; num <= limit; ++num) {
        unsigned long pandigital {}, multiplicand {1};
        unsigned short bitCurrent {};
        while (bitCurrent < bitMask) {
            auto product = num * multiplicand;
            auto toCheck {product};
            // update current mask with each right-most digit
            while (toCheck) {
                auto digit = toCheck % 10;
                toCheck /= 10;
                pandigital *= 10;
                // set bit at digit's int value to 1
                unsigned long bit = 1 << digit;
                // check if bit has already been encountered
                // relational operators have higher precedence than bitwise operators
                if (digit == 0 || (bit & bitCurrent) > 0) {
                    // force a break out of both inner and outer loops
                    bitCurrent = bitMask + 1;
                    break;
                }
                bitCurrent |= bit;
            }
            // a way to break to outer? to avoid calculations without a double check?
            pandigital += product;
            multiplicand++;
        }
        if (bitCurrent == bitMask)
            multipliers.push_back(num);
    }

    return multipliers;
}

/*
 * Project Euler specific implementation that finds the largest 1 to 9 pandigital
 * number that can be formed as a concatenated product.
 *
 * Solution is based on the following:
 *
 *      - Since the multiplier must at minimum be multiplied by both 1 & 2, it cannot
 *      be larger than 4 digits to ensure product is only 9 digits.
 *
 *      - The default largest would be M = 9 * (1, 2, 3, 4, 5) = 918_273_645, so M must
 *      begin  with the digit 9.
 *
 *      - The 2-digit minimum (M = 91) would result in either an 8-/11-digit product
 *      once multiplied by 3 and 4. The 3-digit minimum (M = 912) would result in
 *      either a 7-/11-digit product once multiplied by 2 and 3.
 *
 *      - So M must be a 4-digit number multiplied by 2 to get a 9-digit product and at
 *      minimum will be 9182 (1st 4 digits of default) and at max 9876.
 *
 *      - Multiplying 9xxx by 2 will at minimum result in 18xxx, always generating a
 *      new digit 1, so M cannot itself contain the digit 1, setting the new minimum
 *      to 9234.
 *
 *      - Lastly, multiplying [98xx, 95xx, -1] by 2 will at minimum result in 19xxx,
 *      always generating another digit 9, so M's 2nd digit must be < 5, setting the
 *      new maximum to 9487.
 */
std::string largest9Pandigital()
{
    std::string largest {};

    for (int m {9487}; m >= 9234; --m) {
        largest = std::to_string(m) + std::to_string(m * 2);
        if (isPandigital(largest, 9))
            break;
    }

    return largest;
}

TEST_CASE("test HR problem when K = 8") {
    unsigned short k {8};
    unsigned long nValues[] {100, 1000, 10'000};
    std::vector<unsigned long> expected[] {{18, 78}, {18, 78},
                                           {18, 78, 1728, 1764, 1782,
                                            1827, 2178, 2358,2718, 2817,
                                            3564, 3582, 4176, 4356}};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], findPandigitalMultipliers(n, k));
        CHECK_EQ(expected[i], findPandigitalBitmask(n, k));
    }
}

TEST_CASE("test HR problem when K = 9") {
    unsigned short k {9};
    unsigned long nValues[] {100, 1000, 10'000};
    std::vector<unsigned long> expected[] {{9},
                                           {9, 192, 219, 273, 327},
                                           {9, 192, 219, 273, 327,
                                            6729, 6792, 6927, 7269,
                                            7293, 7329, 7692, 7923,
                                            7932, 9267, 9273, 9327}};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], findPandigitalMultipliers(n, k));
        CHECK_EQ(expected[i], findPandigitalBitmask(n, k));
    }
}

TEST_CASE("test PE problem") {
    const std::string expected {"932718654"};

    CHECK_EQ(expected, largest9Pandigital());
}