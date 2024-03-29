#include "palindrome.h"

#include <algorithm>
#include <string>

#include "../../doctest/doctest.h"

bool isPalindromeBuiltIn(const std::string_view s)
{
    std::string rev {s};
    std::reverse(rev.begin(), rev.end());

    return s == rev;
}

bool isPalindromeConstructor(const std::string_view s)
{
    std::string rev {s.rbegin(), s.rend()};

    return s == rev;
}

bool isPalindromeRecursive(std::string_view s)
{
    if (s.length() < 2)
        return true;
    if (s.front() != s.back())
        return false;

    s.remove_prefix(1);
    s.remove_suffix(1);

    return isPalindromeRecursive(s);
}

bool isPalindrome(const std::string_view s)
{
    const auto sLen = s.length();

    for (int i {0}; i < sLen / 2; ++i) {
        if (s[i] != s[sLen-1-i])
            return false;
    }

    return true;
}

bool isPalindromeNumber(unsigned long long n)
{
    if (n < 10)
        return true;

    unsigned long long rev {}, num {n};

    while (num) {
        rev = rev * 10 + num % 10;
        num /= 10;
    }

    return n == rev;
}

TEST_SUITE("test isPalindrome()") {
    TEST_CASE("returns true for palindromes") {
        std::string_view palindromes[] {"5", "22", "303", "9119"};

        for (const auto& p : palindromes) {
            CHECK(isPalindromeBuiltIn(p));
            CHECK(isPalindromeConstructor(p));
            CHECK(isPalindromeRecursive(p));
            CHECK(isPalindrome(p));
            CHECK(isPalindromeNumber(std::stoull(static_cast<std::string>(p))));
        }
    }

    TEST_CASE("returns false for non-palindromes") {
        std::string_view numbers[] {"10", "523", "8018", "124521"};

        for (const auto& n : numbers) {
            CHECK_FALSE(isPalindromeBuiltIn(n));
            CHECK_FALSE(isPalindromeConstructor(n));
            CHECK_FALSE(isPalindromeRecursive(n));
            CHECK_FALSE(isPalindrome(n));
            CHECK_FALSE(isPalindromeNumber(std::stoull(static_cast<std::string>(n))));
        }
    }
}