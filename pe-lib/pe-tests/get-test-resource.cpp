#include "get-test-resource.h"

#include "../../doctest/doctest.h"

TEST_SUITE("test all resource helpers") {
    std::string path;
    const int expectedSize {5};
    std::string expectedLine;

    TEST_CASE("setup constant") {
        path = "../../resources/fake-resource.txt";
        expectedLine = "1, 2, 3, 4, 5";
    }

    TEST_CASE("test getTestString()") {
        std::string expected {};
        for (int i {0}; i < 5; ++i) {
            expected.append(expectedLine);
        }

        std::string actual = getTestString(path);

        CHECK_EQ(expected, actual);
    }

    TEST_SUITE("test getTestResource()") {
        TEST_CASE("with default retrieval") {
            const std::vector<std::string> actual = getTestResource(path);

            CHECK_EQ(expectedSize, actual.size());
            CHECK_EQ(typeid(std::string), typeid(actual[0]));
            for (const std::string& line : actual) {
                CHECK_EQ(expectedLine, line);
            }
        }

        TEST_CASE("with transformed retrieval") {
            const std::vector<std::string> expectedVector {"1A", "2A", "3A", "4A", "5A"};

            auto actual = getTestResource<std::string>(path,
                                                       [](const std::string& s) {
                                                           return s + 'A';
                                                       },
                                                       ", ");

            CHECK_EQ(expectedSize, actual.size());
            CHECK_EQ(typeid(std::vector<std::string>), typeid(actual[0]));
            for (const auto& line : actual) {
                CHECK_EQ(expectedVector, line);
            }
        }
    }

    TEST_SUITE("test getTestGrid()") {
        unsigned short expectedFirst[] {1, 2, 3, 4, 5};

        TEST_CASE("when T is unsigned short") {
            auto** grid = getTestGrid<unsigned short>(path,
                                                      expectedSize, ", ");

            CHECK_EQ(typeid(unsigned short *), typeid(grid[0]));
            for (int i {0}; i < 5; ++i) {
                CHECK_EQ(expectedFirst[i], grid[0][i]);
            }

            delete[] grid;
        }

        TEST_CASE("when T is unsigned long") {
            auto** grid = getTestGrid<unsigned long>(path,
                                                     expectedSize, ", ");

            CHECK_EQ(typeid(unsigned long *), typeid(grid[0]));
            for (int i {0}; i < 5; ++i) {
                CHECK_EQ(expectedFirst[i], grid[0][i]);
            }

            delete[] grid;
        }
    }
}