#include "combinations.h"

#include "../../doctest/doctest.h"

TEST_SUITE("test Combinations") {
    TEST_CASE("prevents iteration with invalid input") {
        const std::string input {"ABCD"};
        Combinations<char> combos1 {0, input};
        Combinations<char> combos2 {5, input};
        Combinations<char> combos3 {3, ""};

        CHECK_FALSE(combos1.hasNext());
        CHECK_FALSE(combos2.hasNext());
        CHECK_FALSE(combos3.hasNext());
    }

    TEST_CASE("correct when n = 1") {
        Combinations<char> combos {1, "A"};
        std::vector<char> expected {'A'};
        int current {};

        while (combos.hasNext()) {
            current++;
            CHECK_EQ(expected, combos.next());
        }
        CHECK_EQ(1, current);
    }

    TEST_CASE("returns the correct combinations for small string") {
        Combinations<char> combos {2, "ABCD"};
        std::vector<std::vector<char>> expected {{'A', 'B'}, {'A', 'C'}, {'A', 'D'},
                                                 {'B', 'C'}, {'B', 'D'}, {'C', 'D'}};
        int current {};

        while (combos.hasNext()) {
            CHECK_EQ(expected[current++], combos.next());
        }
        CHECK_EQ(expected.size(), current);
    }

    TEST_CASE("returns all combinations for small vector") {
        std::vector<int> input {0, 1, 2, 3};
        Combinations<int> combos {3, input};
        std::vector<std::vector<int>> expected {{0, 1, 2}, {0, 1, 3}, {0, 2, 3},
                                                {1, 2, 3}};
        int current {};

        while (combos.hasNext()) {
            CHECK_EQ(expected[current++], combos.next());
        }
        CHECK_EQ(expected.size(), current);
    }

    TEST_CASE("returns all combinations for large vector") {
        std::vector<int> input(19);
        std::iota(input.begin(), input.end(), 1);
        Combinations<int> combos {15, input};
        int expectedSize {3876};
        int current {};

        while (combos.hasNext()) {
            current++;
            combos.next();
        }
        CHECK_EQ(expectedSize, current);
    }
}