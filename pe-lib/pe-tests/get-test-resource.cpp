#include "get-test-resource.h"

#include <fstream>

#include "../../doctest/doctest.h"

std::string trim(std::string_view line, std::string_view characters)
{
    line.remove_prefix(
            std::min(line.length(), line.find_first_not_of(characters)));
    line.remove_suffix(
            std::max(size_t {0},
                     line.length() - 1 - line.find_last_not_of(characters)));

    return std::string {line};
}

/**
 * Retrieves content of a test resource file as a single std::string.
 *
 * @param lineTrim characters to remove from the left and right of each file line.
 */
std::string getTestString(std::string_view filePath, std::string_view lineTrim)
{
    std::string resource {};
    std::ifstream resFile(static_cast<std::string>(filePath));

    if (resFile.is_open()) {
        std::string line {};
        while (std::getline(resFile, line)) {
            resource.append(trim(line, lineTrim));
        }
        resFile.close();
    }

    return resource;
}

/**
 * Retrieves content of a test resource file, with each line returned as a trimmed, but
 * otherwise unaltered std::string.
 *
 * @param lineTrim characters to remove from the left and right of each file line.
 */
std::vector<std::string> getTestResource(std::string_view filePath,
                                         std::string_view lineTrim)
{
    std::vector<std::string> resource {};
    std::ifstream myFile(static_cast<std::string>(filePath));

    if (myFile.is_open()) {
        std::string line {};
        while (std::getline(myFile, line)) {
            resource.push_back(trim(line, lineTrim));
        }
        myFile.close();
    }

    return resource;
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

TEST_CASE("test getTestString()") {
    std::string_view path {"../../resources/fake-resource.txt"};
    std::string expected {};
    for (int i {}; i < 5; ++i) {
        expected.append("1, 2, 3, 4, 5");
    }

    std::string resource = getTestString(path);

    CHECK_EQ(expected, resource);
}

TEST_CASE("test getTestResource()") {
    std::string_view path {"../../resources/fake-resource.txt"};
    int expectedSize {5};
    std::string expectedLine {"1, 2, 3, 4, 5"};

    std::vector<std::string> resource = getTestResource(path);

    CHECK_EQ(expectedSize, resource.size());
    CHECK_EQ(typeid(std::string), typeid(resource[0]));
    for (const std::string& line : resource) {
        CHECK_EQ(expectedLine, line);
    }
}