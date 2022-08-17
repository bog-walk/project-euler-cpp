/*
 * Problem 11: Largest Product in a Grid
 *
 * https://projecteuler.net/problem=11
 *
 * Goal: Find the largest product of 4 adjacent integers in the same direction (up,
 * down, left, right, diagonal) in an NxN grid.
 *
 * Constraints: 0 <= integer <= 100, 4 <= N <= 20
 *
 * e.g.: 1 1 1 1
 *       1 1 2 1
 *       1 1 3 1
 *       1 1 1 1
 *       column {1,2,3,1} = 6
 */

#include <numeric>

#include "../../doctest/doctest.h"

#include "pe-tests/get-test-resource.h"

// Size of array must be passed into function as array is being passed as pointer.
unsigned long product(std::size_t len, unsigned short* arr)
{
    // To use std::begin(arr) & std::end(arr), must pass array of fixed size
    // as ref, e.g. unsigned short (&arr)[10]
    return std::accumulate(
            &arr[0],
            &arr[len],
            static_cast<unsigned long>(1),
            std::multiplies<>());
}

// Pass by value a pointer to the decayed type 2D array
unsigned long largestProductInGrid(std::size_t len, unsigned short** grid)
{
    const int k {4};
    unsigned long largest {};

    for (int row {0}; row < len; ++row) {
        for (int col {0}; col <= len - k; ++col) {
            unsigned short right[k], down[k];
            for (int i {0}; i < k; ++i) {
                right[i] = grid[row][col + i];
                down[i] = grid[col + i][row];
            }
            unsigned long rightP = product(k, right);
            if (rightP > largest)
                largest = rightP;
            unsigned long downP = product(k, down);
            if (downP > largest)
                largest = downP;

            if (row <= len - k) {
                unsigned short leadingDiag[k], counterDiag[k];
                for (int i {0}; i < k; ++i) {
                    leadingDiag[i] = grid[row + i][col + i];
                    counterDiag[i] = grid[row + i][col + k - 1 - i];
                }
                unsigned long leadingDiagP = product(k, leadingDiag);
                if (leadingDiagP > largest)
                    largest = leadingDiagP;
                unsigned long counterDiagP = product(k, counterDiag);
                if (counterDiagP > largest)
                    largest = counterDiagP;
            }
        }
    }

    return largest;
}

TEST_CASE("test product()") {
    int n {4};
    unsigned short nums[] {26, 63, 78, 14};
    unsigned long expected {1'788'696};

    CHECK_EQ(expected, product(n, nums));
}

TEST_CASE("test with small grid") {
    int n {4};
    auto** grid = getTestGrid<unsigned short>(
            "../resources/largest-product-in-grid-4-by-4.txt",
            n);
    unsigned long expected {6};

    CHECK_EQ(expected, largestProductInGrid(n, grid));
}

TEST_CASE("test with medium grid") {
    int n {6};
    auto** grid = getTestGrid<unsigned short>(
            "../resources/largest-product-in-grid-6-by-6.txt",
            n);
    unsigned long expected {15};

    CHECK_EQ(expected, largestProductInGrid(n, grid));
}

TEST_CASE("test with large grid") {
    int n {20};
    auto** grid = getTestGrid<unsigned short>(
            "../resources/largest-product-in-grid-20-by-20.txt",
            n);
    unsigned long expected {70'600'674};

    // check large resource read properly
    CHECK_EQ(8, grid[0][0]);
    CHECK_EQ(48, grid[19][19]);
    CHECK_EQ(72, grid[15][7]);

    CHECK_EQ(expected, largestProductInGrid(n, grid));
}