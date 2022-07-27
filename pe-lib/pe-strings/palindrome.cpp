#include "../../doctest/doctest.h"
#include <algorithm>
#include <string>
#include <vector>
#include "palindrome.h"

using namespace std;

bool isPalindromeBuiltIn(const string& s)
{
    string rev = s;
    reverse(rev.begin(), rev.end());
    return s == rev;
}

bool isPalindromeConstructor(const string& s)
{
    string rev = string(s.rbegin(), s.rend());
    return s == rev;
}

bool isPalindromeRecursive(string s)
{
    if (s.length() < 2)
        return true;

    if (s.front() != s.back())
        return false;

    // substr() 2nd arg is length of substring, not end index
    return isPalindromeRecursive(s.substr(1, s.length() - 2));
}

bool isPalindrome(string s)
{
    unsigned long long len = s.length();
    if (len == 1)
        return true;

    for (int i = 0; i < len / 2; i++) {
        if (s[i] != s[len - 1 - i])
            return false;
    }

    return true;
}

bool isPalindromeNumber(unsigned long long n)
{
    if (n < 10)
        return true;

    unsigned long long rev {0};
    unsigned long long num = n;
    while (num > 0) {
        rev = rev * 10 + num % 10;
        num /= 10;
    }

    return n == rev;
}

TEST_CASE("test returns true for palindromes") {
    vector<string> palindromes {"5", "22", "303", "9119"};

    for (const string& p : palindromes) {
        CHECK(isPalindromeBuiltIn(p));
        CHECK(isPalindromeConstructor(p));
        CHECK(isPalindromeRecursive(p));
        CHECK(isPalindrome(p));
        CHECK(isPalindromeNumber(stoull(p)));
    }
}

TEST_CASE("test returns false for non-palindromes") {
    vector<string> palindromes {"10", "523", "8018", "124521"};

    for (const string& p : palindromes) {
        CHECK_FALSE(isPalindromeBuiltIn(p));
        CHECK_FALSE(isPalindromeConstructor(p));
        CHECK_FALSE(isPalindromeRecursive(p));
        CHECK_FALSE(isPalindrome(p));
        CHECK_FALSE(isPalindromeNumber(stoull(p)));
    }
}