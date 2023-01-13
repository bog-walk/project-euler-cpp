/*
 * Problem 17: Number to Words
 *
 * https://projecteuler.net/problem=17
 *
 * Goal #1: Print the given number N as capitalised words (with/without "And").
 *
 * Goal #2: Count the total number of letters used when the first N positive numbers are
 * converted to words, with "And" used in compliance with British usage.
 *
 * Constraints: 0 <= N <= 1e12
 *
 * e.g.: Goal #1 -> N = 243
 *       output = "Two Hundred And Forty Three"
 *       Goal #2 -> N = 5 -> {"One", "Two", "Three", "Four", "Five"}
 *       output = 19
 */

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "../../doctest/doctest.h"

namespace numberWords {
    // Clang-Tidy: Initialization of [these variables] with static storage duration may
    // throw an exception that cannot be caught?
    const std::string space {" "};
    const std::string andSpace {" And "};
    const std::string hundred {" Hundred"};
    const std::vector<std::string> underTwenty {"", "One", "Two", "Three", "Four",
                                                "Five", "Six", "Seven", "Eight",
                                                "Nine", "Ten", "Eleven", "Twelve",
                                                "Thirteen", "Fourteen", "Fifteen",
                                                "Sixteen", "Seventeen", "Eighteen",
                                                "Nineteen"};
    const std::vector<std::string> tens {"", "", "Twenty", "Thirty", "Forty", "Fifty",
                                         "Sixty", "Seventy", "Eighty", "Ninety"};
    const std::vector<std::string> powersOfTen {"", "Thousand", "Million", "Billion",
                                                "Trillion"};
}

std::string numberUnderHundred(unsigned long n)
{
    if (n < 20)
        return numberWords::underTwenty[n];

    auto& first = numberWords::tens[n/10];
    auto& second = numberWords::underTwenty[n%10];

    return second.empty() ? first : first + numberWords::space + second;
}

std::string numberUnderThousand(unsigned long n, bool andIncluded)
{
    if (n < 100)
        return numberUnderHundred(n);

    auto& first = numberWords::underTwenty[n/100];
    const auto& second = numberUnderHundred(n % 100);

    if (second.empty()) {
        return first + numberWords::hundred;
    }
    else {
        const auto& extra = andIncluded ? numberWords::andSpace : numberWords::space;
        return first + numberWords::hundred + extra + second;
    }
}

std::string numberWritten(unsigned long long number, bool andIncluded = true)
{
    if (number == 0uLL)
        return "Zero";

    // better way to handle strings without so many temporary strings?
    std::string words;
    int power {};

    while (number) {
        auto modThousand = static_cast<unsigned long>(number % 1000);
        if (modThousand) {
            auto first = numberUnderThousand(modThousand, andIncluded);
            auto& second = numberWords::powersOfTen[power];
            if (!second.empty())
                first = first.append(numberWords::space).append(second);
            if (words.empty())
                words = first;
            else
                words = first.append(numberWords::space).append(words);
        }
        number /= 1000;
        power++;
    }

    return words;
}

unsigned long countFirstNPositives(unsigned long long n)
{
    unsigned long sum {};

    for (unsigned long long num {1uLL}; num <= n; ++num) {
        auto written = numberWritten(num);
        sum += std::count_if(written.cbegin(), written.cend(), ::isalpha);
    }

    return sum;
}

TEST_CASE("test numberWritten() when And not included") {
    unsigned long long nValues[] {
        0, 1, 9,  // single-digit pick
        12, 17, 60,  // double-digit pick
        21, 84,  // double-digit double combo
        200, 5000,  // triple-digit double combo
        243,  // triple-digit triple combo
        8'004'792, 10'000'000'010, 1'010'000'000, 1'000'010'000,
        101'100'000'011,104'382'426'112  // long combos
    };
    std::string expected[] {"Zero", "One", "Nine", "Twelve",
                            "Seventeen", "Sixty", "Twenty One", "Eighty Four",
                            "Two Hundred", "Five Thousand",
                            "Two Hundred Forty Three",
                            "Eight Million Four Thousand Seven Hundred Ninety Two",
                            "Ten Billion Ten","One Billion Ten Million",
                            "One Billion Ten Thousand",
                            "One Hundred One Billion One Hundred Million Eleven",
    "One Hundred Four Billion Three Hundred Eighty Two Million Four Hundred Twenty Six Thousand One Hundred Twelve"};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], numberWritten(n, false));
    }
}

TEST_CASE("test numberWritten() for powers of ten") {
    std::string expected[] {"Ten", "One Hundred", "One Thousand",
                            "Ten Thousand", "One Hundred Thousand",
                            "One Million", "Ten Million", "One Hundred Million",
                            "One Billion"};

    for (int e {0}; e <= 7; ++e) {
        auto n = 10 * static_cast<unsigned long long>(std::pow(10, e));
        CHECK_EQ(expected[e], numberWritten(n, false));
    }
}

TEST_CASE("test numberWritten() when And included") {
    unsigned long long nValues[] {
            // 'and' not needed so should act the same
            4, 11, 66, 1'000'000'000,10'000'000'010,
            1'010'000'000, 1'000'010'000,
            // 'and' should be added
            243, 696, 8'004'792, 101'100'000'011,104'382'426'112
    };
    std::string expected[] {"Four", "Eleven", "Sixty Six","One Billion",
                            "Ten Billion Ten", "One Billion Ten Million",
                            "One Billion Ten Thousand",
                            "Two Hundred And Forty Three",
                            "Six Hundred And Ninety Six",
                            "Eight Million Four Thousand Seven Hundred And Ninety Two",
                            "One Hundred And One Billion One Hundred Million Eleven",
    "One Hundred And Four Billion Three Hundred And Eighty Two Million Four Hundred And Twenty Six Thousand One Hundred And Twelve"};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], numberWritten(n));
    }
}

TEST_CASE("test countFirstNPositives()") {
    unsigned long long n {1000};
    unsigned long expected {21124};

    CHECK_EQ(expected, countFirstNPositives(n));
}