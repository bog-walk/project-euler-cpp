/*
 * Problem 18: Maximum Path Sum 1
 *
 * https://projecteuler.net/problem=18
 *
 * Goal: Find the maximum sum from tree root to leaf node of an N-rowed tree, which
 * contains i integers on each ith row, by moving to an adjacent number on the row below.
 *
 * Constraints: 1 <= N <= 15, numbers in [0,100)
 *
 * e.g.: N = 4
 *           3
 *          7 4
 *         2 4 6
 *        8 5 9 3
 *        max path -> 3+7+4+9
 *        output = 23
 */

#include <algorithm>

#include "../../doctest/doctest.h"

#include "pe-custom/pyramid-tree.h"

/*
 * Solution starts from the bottom of the tree (last nested list), repeatedly
 * finding the maximum of adjacent values and adding it to the parent value 1 row
 * above, until the tree root is reached. This new root value is returned as the
 * maximum path value.
 */
unsigned long maxPathSum(unsigned short rows, unsigned short** elements)
{
    for (int row {rows}; row--;) {
        for (int col {0}; col < row; ++col) {
            elements[row-1][col] += std::max(elements[row][col], elements[row][col+1]);
        }
    }

    return elements[0][0];
}

/*
 * Generates a graph using custom PyramidTree class & finds the largest valued route
 * using post-order traversal.
 */
unsigned long maxPathSumPyramid(unsigned short rows, size_t len,
                                unsigned short* elements)
{
    PyramidTree pyramid {rows, len, elements};

    return pyramid.maxSumPostOrderTraversal(pyramid.root);
}

/*
 * Converts a 1D array of elements into a 2D array pyramid (consecutive nested arrays
 * have 1 more element than the previous array).
 */
unsigned short **getNestedPyramid(unsigned short rows, const unsigned short *elements)
{
    auto* pyramid = new unsigned short*[rows];
    int start {};

    for (int row {0}; row < rows; ++row) {
        auto end = start + row;
        // could slice array if using std::valarray or vector constructor
        pyramid[row] = new unsigned short[row+1];
        for (int i {0}; i <= row; ++i) {
            pyramid[row][i] = elements[start+i];
        }
        start = end + 1;
    }

    return pyramid;
}

TEST_CASE("test with small tree") {
    unsigned short n {4};
    unsigned short elements[] {3, 7, 4, 2, 4, 6, 8, 5,
                               9, 3};
    unsigned long expected {23};
    auto** pyramid = getNestedPyramid(n, elements);

    CHECK_EQ(expected, maxPathSum(n, pyramid));
    CHECK_EQ(expected, maxPathSumPyramid(n, std::size(elements), elements));
}

TEST_CASE("test with large tree") {
    unsigned short n {15};
    unsigned short elements[] {75, 95, 64, 17, 47, 82, 18,
                               35, 87, 10,20, 4, 82, 47,
                               65, 19, 1, 23, 75, 3,34,
                               88, 2, 77, 73, 7, 63, 67,
                               99, 65,4, 28, 6, 16, 70,
                               92, 41, 41, 26, 56,83, 40,
                               80, 70, 33, 41, 48, 72, 33,
                               47,32, 37, 16, 94, 29, 53,
                               71, 44, 65, 25, 43, 91,52,
                               97, 51, 14, 70, 11,33, 28,
                               77, 73, 17, 78, 39, 68, 17,
                               57, 91, 71, 52, 38,17, 14,
                               91, 43,58, 50, 27, 29, 48,
                               63, 66, 4, 68,89, 53, 67,
                               30, 73, 16, 69,87, 40,
                               31, 4, 62, 98, 27, 23,
                               9, 70, 98,73, 93, 38,
                               53, 60, 4, 23};
    unsigned long expected {1074};
    auto** pyramid = getNestedPyramid(n, elements);

    CHECK_EQ(expected, maxPathSum(n, pyramid));
    CHECK_EQ(expected, maxPathSumPyramid(n, std::size(elements), elements));
}