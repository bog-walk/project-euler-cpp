#include "../../doctest/doctest.h"
#include "get-test-resource.h"
#include <fstream>
#include <regex>

std::string trim(const std::string &line, const std::string& characters)
{
    std::string pattern = "^" + characters + "+|" + characters + "+$";
    return std::regex_replace(line, std::regex(pattern), "");
}

/**
 * Retrieves content of a test resource file as a single std::string.
 *
 * @param lineTrim sting of characters to remove from the left and right of
 * each file line.
 */
std::string getTestString(
        const std::string& filePath,
        const std::string& lineTrim
) {
    std::string resource;
    std::ifstream resFile (filePath);
    if (resFile.is_open()) {
        std::string line;
        while (std::getline(resFile, line)) {
            resource.append(trim(line, lineTrim));
        }
        resFile.close();
    }
    return resource;
}

/**
 * Retrieves content of a test resource file, with each line returned as an
 * unaltered std::string.
 *
 * @param lineTrim sting of characters to remove from the left and right of
 * each file line.
 */
std::vector<std::string> getTestResource(
        const std::string& filePath,
        const std::string& lineTrim
) {
    std::vector<std::string> resource;
    std::ifstream myFile (filePath);
    if (myFile.is_open()) {
        std::string line;
        while (std::getline(myFile, line)) {
            resource.push_back(trim(line, lineTrim));
        }
        myFile.close();
    }
    return resource;
}

TEST_SUITE("Test trim()") {
    TEST_CASE("test trim() with default arg") {
        std::string line {"   This line has whitespace   \n" };
        std::string expected { "This line has whitespace" };

        CHECK_EQ(expected, trim(line, "[ \n]"));
    }

    TEST_CASE("test trim() with different arg") {
        std::string line {"#|Fancy line   |" };
        std::string expected { "Fancy line" };

        CHECK_EQ(expected, trim(line, "[ |#]"));
    }
}

TEST_CASE("test getTestString()") {
    std::string path { "../../resources/fake-resource.txt" };
    std::string expected;
    for (int i = 0; i < 5; i++) {
        expected.append("1, 2, 3, 4, 5");
    }

    std::string resource = getTestString(path);

    CHECK_EQ(expected, resource);
}

TEST_CASE("test getTestResource()") {
    std::string path { "../../resources/fake-resource.txt" };
    int expectedSize {5};
    std::string expectedLine {"1, 2, 3, 4, 5"};

    std::vector<std::string> resource = getTestResource(path);

    CHECK_EQ(expectedSize, resource.size());
    CHECK_EQ(typeid(std::string), typeid(resource[0]));
    for (const std::string& line : resource) {
        CHECK_EQ(expectedLine, line);
    }
}