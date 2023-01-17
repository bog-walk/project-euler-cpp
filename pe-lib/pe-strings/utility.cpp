#include "utility.h"

#include <numeric>

#include "../../doctest/doctest.h"

/*
 * @return string after the removal of leading and trailing characters matching
 * the provided argument.
 */
std::string trim(std::string_view line, std::string_view characters)
{
    line.remove_prefix(std::min(line.length(),
                                   line.find_first_not_of(characters)));
    line.remove_suffix(std::max(std::size_t {0},
                             line.length() - 1 - line.find_last_not_of(characters)));

    return std::string {line};
}

/*
 * Splits the original string into a vector of strings around occurrences of the
 * provided delimiter.
 *
 * Could have used stringStream getLine(), but the latter only takes a single character
 * delimiter.
 */
std::vector<std::string> split(std::string_view line,
                               std::string_view delimiter)
{
    // Could have used std::regex_token_iterator instead?
    std::size_t start;
    std::size_t end {0};

    std::vector<std::string> tokens;
    // std::string::npos is the greatest possible value for element of type size_t.
    // it indicates end of string/no match/-1.
    while ((start = line.find_first_not_of(delimiter, end)) != std::string::npos) {
        end = line.find(delimiter, start);
        auto token = line.substr(start, end - start);
        tokens.push_back(static_cast<std::string>(token));
    }

    return tokens;
}

/*
 * Default use results in the sum of all digits in a string representation of a str.
 *
 * @param [normaliser] character that will determine decimal value of each character in
 * string.
 * @return sum of decimal representation of characters in a string.
 */
unsigned long characterSum(std::string_view str, char normaliser)
{
    return std::accumulate(
            str.cbegin(),
            str.cend(),
            0uL,
            [&normaliser](unsigned long acc, const char& ch) {
                return acc + (ch - normaliser);
            });
}

TEST_SUITE("test trim()") {
    TEST_CASE("with default arg") {
        std::string_view line {"   This line has whitespace   \n"};
        std::string expected {"This line has whitespace"};

        CHECK_EQ(expected, trim(line));
    }

    TEST_CASE("with different arg") {
        std::string_view line {"#|Fancy line   |"};
        std::string expected {"Fancy line"};

        CHECK_EQ(expected, trim(line, "[ |#]"));
    }
}

TEST_SUITE("test split()") {
    TEST_CASE("with default arg") {
        std::string_view line {"How now brown cow"};
        std::vector<std::string> expected {"How", "now", "brown", "cow"};

        CHECK_EQ(expected, split(line));
    }

    TEST_CASE("with different arg") {
        std::string_view line {"1, 2, 3, 4, 5, 6"};
        std::vector<std::string> expected {"1", "2", "3", "4", "5", "6"};

        CHECK_EQ(expected, split(line, ", "));
    }
}

TEST_SUITE("test characterSum()") {
    TEST_CASE("with single digit numbers") {
        for (int i {0}; i < 10; ++i) {
            CHECK_EQ(i, characterSum(std::to_string(i)));
        }
    }

    TEST_CASE("with small numbers") {
        std::string numbers[] {"10", "111", "999999", "5678"};
        unsigned long expected[] {1, 3, 54, 26};

        for (auto& number : numbers) {
            auto i = &number - &numbers[0];
            CHECK_EQ(expected[i], characterSum(number));
        }
    }

    TEST_CASE("with large numbers") {
        std::string numbers[] {"123456789", "5000000000000000000",
                               "99999999999999999999", "12341234"};
        unsigned long expected[] {45, 5, 180, 20};

        for (auto& number : numbers) {
            auto i = &number - &numbers[0];
            CHECK_EQ(expected[i], characterSum(number));
        }
    }

    TEST_CASE("with non-number strings") {
        const char allCaps {'@'};
        std::string names[] {"PAMELA", "COLIN", "A", "ZZZZZZZZZZ"};
        unsigned long expected[] {48, 53, 1, 260};

        for (auto& name : names) {
            auto i = &name - &names[0];
            CHECK_EQ(expected[i], characterSum(name, allCaps));
        }
    }
}