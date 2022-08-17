#include "get-test-resource.h"

#include "../../doctest/doctest.h"

#include "../pe-strings/utility.cpp"

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

TEST_CASE("test getTestString()") {
    std::string_view path {"../../resources/fake-resource.txt"};
    std::string expected {};
    for (int i {}; i < 5; ++i) {
        expected.append("1, 2, 3, 4, 5");
    }

    std::string resource = getTestString(path);

    CHECK_EQ(expected, resource);
}

TEST_SUITE("test getTestResource()") {
    TEST_CASE("with default retrieval") {
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

    TEST_CASE("with transformed retrieval") {
        std::string_view path {"../../resources/fake-resource.txt"};
        int expectedSize {5};
        std::vector<std::string> expectedLine {"1A", "2A", "3A", "4A", "5A"};

        auto resource = getTestResource<std::string>(
                path,
                [](const std::string& s) {
                    return s + 'A';
                },
                ", ");

        CHECK_EQ(expectedSize, resource.size());
        CHECK_EQ(typeid(std::vector<std::string>), typeid(resource[0]));
        for (const std::vector<std::string>& line : resource) {
            CHECK_EQ(expectedLine, line);
        }
    }
}

TEST_SUITE("test getTestGrid()") {
    TEST_CASE("when T is unsigned short") {
        std::string_view path {"../../resources/fake-resource.txt"};
        int expectedSize {5};
        unsigned short expectedFirst[] {1, 2, 3, 4, 5};

        auto** grid = getTestGrid<unsigned short>(
                path, expectedSize, ", "
        );

        CHECK_EQ(typeid(unsigned short *), typeid(grid[0]));
        for (int i {0}; i < 5; ++i) {
            CHECK_EQ(expectedFirst[i], grid[0][i]);
        }
    }

    TEST_CASE("when T is unsigned long") {
        std::string_view path {"../../resources/fake-resource.txt"};
        int expectedSize {5};
        unsigned long expectedFirst[] {1, 2, 3, 4, 5};

        auto** grid = getTestGrid<unsigned long>(
                path, expectedSize, ", "
        );

        CHECK_EQ(typeid(unsigned long *), typeid(grid[0]));
        for (int i {0}; i < 5; ++i) {
            CHECK_EQ(expectedFirst[i], grid[0][i]);
        }
    }
}