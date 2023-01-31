#include "cartesian-product.h"

#include <numeric>

#include "../../doctest/doctest.h"

TEST_SUITE("test product()") {
    TEST_CASE("when elements is empty") {
        std::vector<std::vector<int>> inputs {};

        CHECK(product<int>(inputs).empty());

        inputs.push_back({1, 2});
        inputs.emplace_back();

        CHECK(product<int>(inputs).empty());
    }

    TEST_CASE("when element has single iterable with single element") {
        std::vector<std::vector<char>> inputs {{'A'}};

        CHECK_EQ(inputs, product<char>(inputs));
    }

    TEST_CASE("with single multi-element iterable") {
        std::vector<std::vector<char>> inputs {{'A', 'B', 'C'}};
        std::vector<std::vector<char>> expected {{'A'}, {'B'}, {'C'}};

        CHECK_EQ(expected, product<char>(inputs));
    }

    TEST_CASE("with 2 small-sized iterables of different lengths") {
        std::vector<std::vector<char>> inputs {{'A', 'B', 'C', 'D'}, {'x', 'y'}};

        std::vector<std::vector<char>> expected {{'A', 'x'}, {'A', 'y'}, {'B', 'x'},
                                                 {'B', 'y'}, {'C', 'x'}, {'C', 'y'},
                                                 {'D', 'x'}, {'D', 'y'}};

        CHECK_EQ(expected, product<char>(inputs));
    }

    TEST_CASE("with multiple small-sized iterables") {
        std::vector<std::vector<int>> inputs {{0, 1, 2}, {1, 2, 3}, {5, 6, 7, 8}};
        std::size_t expectedSize {36};
        std::vector<std::vector<int>> expectedHead {{0, 1, 5}, {0, 1, 6}, {0, 1, 7},
                                                    {0, 1, 8}, {0, 2, 5}, {0, 2, 6}};
        std::vector<std::vector<int>> expectedTail {{2, 2, 7}, {2, 2, 8}, {2, 3, 5},
                                                    {2, 3, 6}, {2, 3, 7}, {2, 3, 8}};
        auto actual = product<int>(inputs);
        std::vector<std::vector<int>> actualHead {actual.cbegin(), actual.cbegin() + 6};
        std::vector<std::vector<int>> actualTail {actual.cend() - 6, actual.cend()};

        CHECK_EQ(expectedSize, actual.size());
        CHECK_EQ(expectedHead, actualHead);
        CHECK_EQ(expectedTail, actualTail);
    }

    TEST_CASE("with 2 large-sized iterables") {
        std::vector<std::vector<char>> inputs;
        std::vector<char> alpha(16);
        std::iota(alpha.begin(), alpha.end(), 'A');
        std::vector<char> digits(9);
        std::iota(digits.begin(), digits.end(), '1');
        inputs.push_back(alpha);
        inputs.push_back(digits);
        std::size_t expectedSize {144};
        std::vector<std::vector<char>> expectedHead {{'A', '1'}, {'A', '2'}, {'A', '3'},
                                                    {'A', '4'}, {'A', '5'}, {'A', '6'}};
        std::vector<std::vector<char>> expectedTail {{'P', '4'}, {'P', '5'}, {'P', '6'},
                                                    {'P', '7'}, {'P', '8'}, {'P', '9'}};
        auto actual = product<char>(inputs);
        std::vector<std::vector<char>> actualHead {actual.cbegin(), actual.cbegin() + 6};
        std::vector<std::vector<char>> actualTail {actual.cend() - 6, actual.cend()};

        CHECK_EQ(expectedSize, actual.size());
        CHECK_EQ(expectedHead, actualHead);
        CHECK_EQ(expectedTail, actualTail);
    }

    TEST_CASE("with 1 iterable but repeat > 1") {
        std::vector<std::vector<int>> inputs {{0, 1, 2}};
        std::vector<std::vector<int>> expected {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1},
                                                {1, 2}, {2, 0}, {2, 1}, {2, 2}};

        CHECK_EQ(expected, product<int>(inputs, 2));
    }

    TEST_CASE("with 2 iterables but repeat > 1") {
        std::vector<std::vector<char>> inputs {{'a', 'b'}, {'c', 'd'}};
        std::size_t expectedSize {64};
        std::vector<std::vector<char>> expectedHead {{'a', 'c', 'a', 'c', 'a', 'c'},
                                                     {'a', 'c', 'a', 'c', 'a', 'd'},
                                                     {'a', 'c', 'a', 'c', 'b', 'c'},
                                                     {'a', 'c', 'a', 'c', 'b', 'd'},
                                                     {'a', 'c', 'a', 'd', 'a', 'c'}};
        std::vector<std::vector<char>> expectedTail {{'b', 'd', 'b', 'c', 'b', 'd'},
                                                     {'b', 'd', 'b', 'd', 'a', 'c'},
                                                     {'b', 'd', 'b', 'd', 'a', 'd'},
                                                     {'b', 'd', 'b', 'd', 'b', 'c'},
                                                     {'b', 'd', 'b', 'd', 'b', 'd'}};
        auto actual = product<char>(inputs, 3);
        std::vector<std::vector<char>> actualHead {actual.cbegin(), actual.cbegin() + 5};
        std::vector<std::vector<char>> actualTail {actual.cend() - 5, actual.cend()};

        CHECK_EQ(expectedSize, actual.size());
        CHECK_EQ(expectedHead, actualHead);
        CHECK_EQ(expectedTail, actualTail);
    }
}