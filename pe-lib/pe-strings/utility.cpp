#include "utility.h"

#include "../../doctest/doctest.h"

/**
 * Returns a std::string after the removal of leading and trailing characters matching
 * the provided argument.
 */
std::string trim(std::string_view line, std::string_view characters)
{
    line.remove_prefix(
            std::min(line.length(), line.find_first_not_of(characters)));
    line.remove_suffix(
            std::max(size_t {0},
                     line.length() - 1 - line.find_last_not_of(characters)));

    return std::string {line};
}

// Could have used stringStream getLine, but only takes single character delimiter
/**
 * Splits the original std::string into a std::vector of std::string around occurrences
 * of the provided delimiter.
 */
std::vector<std::string> split(std::string_view line, std::string_view delimiter)
{
    // Could have used std::regex_token_iterator instead
    size_t start;
    size_t end {0};

    std::vector<std::string> tokens;
    // std::string::npos is the greatest possible value for element of type size_t
    // indicates end of string/no match/ -1
    while ((start = line.find_first_not_of(delimiter, end)) != std::string::npos) {
        end = line.find(delimiter, start);
        auto token = line.substr(start, end - start);
        tokens.push_back(static_cast<std::string>(token));
    }

    return tokens;
}

TEST_SUITE("test trim()") {
    TEST_CASE("with default arg") {
        std::string_view line {"   This line has whitespace   \n"};
        std::string expected {"This line has whitespace"};

        CHECK_EQ(expected, trim(line, " \n"));
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