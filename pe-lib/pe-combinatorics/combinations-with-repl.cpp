#include "combinations-with-repl.h"

#include "../../doctest/doctest.h"

TEST_SUITE("test CombinationsWithRepl") {
    TEST_CASE("prevents iteration with invalid input") {
        CombinationsWithRepl<char> combos1 {0, "ABCD"};
        CombinationsWithRepl<char> combos2 {2, ""};

        CHECK_FALSE(combos1.hasNext());
        CHECK_FALSE(combos2.hasNext());
    }

    TEST_CASE("correct when r > n") {
        std::vector<std::string> input {"A", "B"};
        CombinationsWithRepl<std::string> combos {4, input};
        std::vector<std::vector<std::string>> expected {{"A", "A", "A", "A"},
                                                        {"A", "A", "A", "B"},
                                                        {"A", "A", "B", "B"},
                                                        {"A", "B", "B", "B"},
                                                        {"B", "B", "B", "B"}};
        int current {};

        while (combos.hasNext()) {
            CHECK_EQ(expected[current++], combos.next());
        }
        CHECK_EQ(expected.size(), current);
    }

    TEST_CASE("correct when n = 1") {
        CombinationsWithRepl<char> combos {1, "A"};
        std::vector<char> expected {'A'};
        int current {};

        while (combos.hasNext()) {
            current++;
            CHECK_EQ(expected, combos.next());
        }
        CHECK_EQ(1, current);
    }

    TEST_CASE("returns the correct combinations for small string") {
        CombinationsWithRepl<char> combos {2, "ABCD"};
        std::vector<std::vector<char>> expected {{'A', 'A'}, {'A', 'B'},
                                                        {'A', 'C'}, {'A', 'D'},
                                                        {'B', 'B'}, {'B', 'C'},
                                                        {'B', 'D'}, {'C', 'C'},
                                                        {'C', 'D'}, {'D', 'D'}};
        int current {};

        while (combos.hasNext()) {
            CHECK_EQ(expected[current++], combos.next());
        }
        CHECK_EQ(expected.size(), current);
    }

    TEST_CASE("returns all combinations for small vector") {
        std::vector<int> input {0, 1, 2, 3};
        CombinationsWithRepl<int> combos {3, input};
        int expectedSize {20};
        int current {};

        while (combos.hasNext()) {
            current++;
            combos.next();
        }
        CHECK_EQ(expectedSize, current);
    }

    TEST_CASE("returns all combinations for large vector") {
        std::vector<int> input {1, 2, 3, 4, 5, 6, 7, 8, 9};
        CombinationsWithRepl<int> combos {6, input};
        int expectedSize {3003};
        int current {};

        while (combos.hasNext()) {
            current++;
            combos.next();
        }
        CHECK_EQ(expectedSize, current);
    }
}