#include "is-pandigital.h"

#include "../../doctest/doctest.h"

#include "utility.h"

/*
 * Checks if str contains all digits between 1 and n inclusive.
 *
 * Using all characters in the provided std::string as characters to trim is a trick to
 * check whether a string contains only characters from another string.
 * e.g. trim("1234", "4231") == "".
 */
bool isPandigital(std::string_view str, unsigned short n)
{
    if (str.length() != n)
        return false;

    std::string digits {"1234567890"};
    // resize() truncates collection if n < length/size
    digits.resize(n);

    return trim(digits, str).empty();
}

TEST_SUITE("test isPandigital()") {
    TEST_CASE("returns true for valid strings") {
        const std::string nums[] {"1", "231", "54321", "564731982", "1234560789"};
        unsigned short digits[] {1, 3, 5, 9, 10};

        for (auto& num : nums) {
            auto i = &num - &nums[0];
            CHECK(isPandigital(num, digits[i]));
        }
    }

    TEST_CASE("returns false for invalid strings") {
        const std::string nums[] {"", "1", "85018", "810"};
        unsigned short digits[] {10, 4, 5, 3};

        for (auto& num : nums) {
            auto i = &num - &nums[0];
            CHECK_FALSE(isPandigital(num, digits[i]));
        }
    }
}