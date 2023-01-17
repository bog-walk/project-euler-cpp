/*
 * Problem 22: Names Scores
 *
 * https://projecteuler.net/problem=22
 *
 * Goal: Given an unsorted list of N names, first sort alphabetically, then, given 1 of
 * the names, multiply the sum of the value of all its characters by its position in
 * the alphabetical list.
 *
 * Constraints: 1 <= N <= 5200, len(NAME) < 12
 *
 * e.g.: input = [ALEX, LUIS, JAMES, BRIAN, PAMELA]
 *       sorted = [ALEX, BRIAN, JAMES, LUIS, PAMELA]
 *       name = PAMELA = 16 + 1 + 13 + 5 + 12 + 1 = 48
 *       position = 5th
 *       result = 5 * 48 = 240
 */

#include <algorithm>
#include <numeric>

#include "../../doctest/doctest.h"

#include "pe-tests/get-test-resource.h"

/*
 * Helper function returns a score for a name as detailed in documentation above.
 *
 * @param [index] zero-indexed position found in calling function's list.
 * @param [name] string assumed to be in ALL_CAPS, but this could be ensured by
 * including std::toupper() in the solution.
 */
unsigned long nameScore(unsigned short index, const std::string& name)
{
    // unicode decimal value of 'A' is 65, so is normalised to represent value 1
    // by subtracting '@' (decimal 64) instead
    return (index + 1) * characterSum(name, '@');
}

/*
 * Project Euler specific implementation that requires all the name scores of a 5000+
 * list to be summed.
 *
 * Since std::sort() is in-place, options for sorting a vector include:
 *
 *  -   pass the vector by value so a local copy is sorted and the original is intact.
 *  -   std::partial_sort_copy() after initialisation of a local vector.
 *  -   proxy-sort an index array instead to reference the original.
 *
 * N.B. If the input values were read into a std::set, they would be auto-stored in a
 * specific order.
 */
unsigned long sumOfNameScores(const std::vector<std::string>& input)
{
    std::vector<unsigned short> indices(input.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(
            indices.begin(),
            indices.end(),
            [&input](unsigned short a, unsigned short b) {
                return input[a] < input[b];
            });

    return std::accumulate(
            indices.cbegin(),
            indices.cend(),
            0uL,
            [i=0, &input](unsigned long acc, unsigned short nameI) mutable {
                return acc + nameScore(i++, input[nameI]);
            });
}

TEST_CASE("test nameScore()") {
    const std::string names[] {"PAMELA", "COLIN", "A", "ZZZZZZZZZZ"};
    unsigned short indices[] {4, 937, 0, 5199};
    unsigned long expected[] {240, 49714, 1, 1'352'000};

    for (const auto& name: names) {
        auto i = &name - &names[0];
        CHECK_EQ(expected[i], nameScore(indices[i], name));
    }
}

TEST_CASE("test with small list") {
    const std::vector<std::string> names {"ALEX", "LUIS", "JAMES", "BRIAN", "PAMELA"};
    unsigned short sortedI[] {0, 3, 2, 1, 4};
    unsigned long expectedScores[] {42, 244, 144, 88, 240};

    for (const auto& name: names) {
        auto i = &name - &names[0];
        CHECK_EQ(expectedScores[i], nameScore(sortedI[i], name));
    }

    unsigned long expectedSum {758};

    CHECK_EQ(expectedSum, sumOfNameScores(names));
}

TEST_CASE("test with medium list") {
    const std::vector<std::string> names {"OLIVIA", "ALEX", "MIA", "LUIS", "LEO", "JAMES",
                                          "BRIAN", "NOAH", "PAMELA", "AIDEN", "BENJAMIN",
                                          "HARPER", "MUHAMMAD", "PENELOPE", "RILEY",
                                          "JACOB", "SEBASTIAN", "LILY", "ELI", "IVY",
                                          "STELLA", "HANNAH", "VIOLET"};
    unsigned long expectedSum {16842};

    CHECK_EQ(expectedSum, sumOfNameScores(names));
}

TEST_CASE("test with large list") {
    const auto names = getTestResource("../resources/names-scores.txt");
    unsigned long expectedSum {871'198'282};

    CHECK_EQ(expectedSum, sumOfNameScores(names));
}