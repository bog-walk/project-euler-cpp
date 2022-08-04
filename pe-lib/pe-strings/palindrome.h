#ifndef PROJECT_EULER_CPP_PALINDROME_H
#define PROJECT_EULER_CPP_PALINDROME_H

#include <string_view>

bool isPalindromeBuiltIn(std::string_view s);

bool isPalindromeConstructor(std::string_view s);

bool isPalindromeRecursive(std::string_view s);

bool isPalindrome(std::string_view s);

bool isPalindromeNumber(unsigned long long n);

#endif //PROJECT_EULER_CPP_PALINDROME_H