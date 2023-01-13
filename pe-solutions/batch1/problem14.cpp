/*
 * Problem 14: Longest Collatz Sequence
 *
 * https://projecteuler.net/problem=14
 *
 * Goal: Find the largest starting number <= N that produces the longest Collatz sequence.
 *
 * Constraints: 1 <= N <= 5e6
 *
 * Collatz Sequence: Thought to all finish at 1, a sequence of positive integers is
 * generated using the hailstone calculator algorithm, such that:
 *      (even n) n -> n/2
 *      (odd n) n -> 3n + 1
 *
 * e.g.: N = 5
 *       1
 *       2 -> 1
 *       3 -> 10 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1
 *       4 -> 2 -> 1
 *       5 -> 16 -> 8 -> 4 -> 2 -> 1
 *       longest chain when starting number = 3
 */

#include <cmath>

#include "../../doctest/doctest.h"

namespace collatz {
    constexpr int limit {5'000'000};
    // cache for all previously counted collatz sequences
    unsigned long countedSeq[collatz::limit+1];
}

/*
 * @return length of a Collatz sequence given its starting number.
 */
unsigned long collatzLength(unsigned long start)
{
    unsigned long count {1};

    while (start != 1) {
        // if x AND 1 == 0 then x is even
        start = start & 1 ? start * 3 + 1 : start / 2;
        // bitwise AND between positive x and x-1 will be zero if x is a power of 2
        // c++ bitwise operator has lower precedence than arithmetic
        if (start && !(start & start - 1)) {
            count += static_cast<unsigned long>(std::log2(start)) + 1;
            break;
        }
        else {
            count++;
        }
    }

    return count;
}

/*
 * Recursive solution uses saved lengths of previously determined Collatz sequences to
 * speed performance of calling function.
 */
unsigned long collatzLengthMemo(unsigned long start)
{
    if (start <= collatz::limit && collatz::countedSeq[start-1])
        return collatz::countedSeq[start-1];

    unsigned long count {1};

    if ((start & 1))  // if x AND 1 > 0 then x is odd
        // add a division by 2 as oddStart * 3 + 1 gives an even number,
        // so both steps can be combined
        count += collatzLengthMemo((3 * start + 1) / 2);
    else
        count += collatzLengthMemo(start / 2);

    if (start <= collatz::limit)
        collatz::countedSeq[start-1] = count;

    return count;
}

/*
 * Generates all starting numbers < 5e6 that produce the longest sequence, stored in an
 * array with the largest starter <= n at index [n-1].
 */
unsigned long* generateLongestStarters()
{
    collatz::countedSeq[0] = 1;
    // cache for the starting number <= index that generates the longest sequence
    static unsigned long longestCountedUnderN[collatz::limit+1];
    longestCountedUnderN[0] = 1;

    unsigned long longestStarter {1}, longestCount {1};
    for (int i {2}; i <= collatz::limit; ++i) {
        auto currentLength = collatzLengthMemo(i);
        if (currentLength >= longestCount) {
            longestStarter = i;
            longestCount = currentLength;
        }
        longestCountedUnderN[i-1] = longestStarter;
    }

    return longestCountedUnderN;
}

TEST_CASE("test collatzLength()") {
    unsigned long startValues[] {1, 2, 3, 4, 5, 6, 7, 8,
                                 9, 10, 11, 12, 15, 17, 19,
                                 27};
    unsigned long expected[] {1, 2, 8, 3, 6, 9, 17, 4,
                              20, 7, 15, 10, 18, 13, 21,
                              112};

    for (const auto& start: startValues) {
        auto i = &start - &startValues[0];
        CHECK_EQ(expected[i], collatzLength(start));
    }
}

TEST_CASE("test longestCollatzUnderN using memoisation") {
    unsigned long* allStarters = generateLongestStarters();
    unsigned long startValues[] {1, 2, 3, 5, 10, 15, 19, 20,
                                 26, 100, 1000, 10'000, 100'000,
                                 1'000'000};
    unsigned long expected[] {1, 2, 3, 3, 9, 9, 19, 19,
                              25, 97,871, 6171, 77031, 837'799};

    for (const auto& start: startValues) {
        auto i = &start - &startValues[0];
        CHECK_EQ(expected[i], allStarters[start-1]);
    }
}