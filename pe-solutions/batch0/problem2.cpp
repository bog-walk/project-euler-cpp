/**
 * Problem 2: Even Fibonacci Numbers
 *
 * https://projecteuler.net/problem=2
 *
 * Goal: Find the sum of all even numbers in the Fibonacci sequence less than N.
 *
 * Constraints: 10 <= N <= 4e16
 *
 * e.g.: N = 44
 *       even fibonacci < N = {2, 8, 34}
 *       sum = 44
 */

#include <vector>
#include "../../doctest/doctest.h"

// Brute iteration over all Fibonacci terms using the formula:
//      F_n = F_{n-2} + F_{n-1}
unsigned long long sumOfEvenFibsNaive(unsigned long long n)
{
    unsigned long long sum {0};
    unsigned long long prev2 {1};
    unsigned long long prev1 {2};
    while (prev1 < n) {
        if ((prev1 & 1) == 0)
            sum += prev1;
        auto nextFib = prev1 + prev2;
        prev2 = prev1;
        prev1 = nextFib;
    }
    return sum;
}

// Sums every 3rd term in the sequence starting with 2, based on the observed
// pattern that every 3rd Fibonacci number after 2 is even. This occurs
// because the sequence begins with 2 odd numbers, the sum of which must be
// even, then the sum of an odd and even number, twice, will produce 2 odd
// numbers, etc...
unsigned long long sumOfEvenFibsBrute(unsigned long long n)
{
    unsigned long long sum {0};
    unsigned long long prev {1};
    unsigned long long evenFib {2};
    while (evenFib < n) {
        sum += evenFib;
        auto next = prev + evenFib;
        prev = next + evenFib;
        evenFib = next + prev;
    }
    return sum;
}

// Sums every 3rd term in the sequence starting with 2, using the formula:
//      F_n = F_{n-6} + 4F_{n-3}
unsigned long long sumOfEvenFibsFormula(unsigned long long n)
{
    unsigned long long sum {10};
    std::vector<unsigned long long> evenFibs {2, 8};  // [F(3), F(6)]
    while (true) {
        auto nextEvenFib = evenFibs[0] + 4 * evenFibs[1];
        if (nextEvenFib >= n)
            break;
        sum += nextEvenFib;
        evenFibs[0] = evenFibs[1];
        evenFibs[1] = nextEvenFib;
    }
    return sum;
}

TEST_CASE("test lower constraints") {
    std::vector<unsigned long long> nValues {10, 30, 44};
    std::vector<unsigned long long> expected {10, 10, 44};

    for (int i {0}; i < nValues.size(); i++) {
        CHECK_EQ(expected[i], sumOfEvenFibsNaive(nValues[i]));
        CHECK_EQ(expected[i], sumOfEvenFibsBrute(nValues[i]));
        CHECK_EQ(expected[i], sumOfEvenFibsFormula(nValues[i]));
    }
}

TEST_CASE("test mid constraints") {
    std::vector<unsigned long long> nValues {200, 2200};
    std::vector<unsigned long long> expected {188, 798};

    for (int i {0}; i < nValues.size(); i++) {
        CHECK_EQ(expected[i], sumOfEvenFibsNaive(nValues[i]));
        CHECK_EQ(expected[i], sumOfEvenFibsBrute(nValues[i]));
        CHECK_EQ(expected[i], sumOfEvenFibsFormula(nValues[i]));
    }
}

TEST_CASE("test when N is even fibonacci") {
    std::vector<unsigned long long> nValues {34, 10946};
    std::vector<unsigned long long> expected {10, 3382};

    for (int i {0}; i < nValues.size(); i++) {
        CHECK_EQ(expected[i], sumOfEvenFibsNaive(nValues[i]));
        CHECK_EQ(expected[i], sumOfEvenFibsBrute(nValues[i]));
        CHECK_EQ(expected[i], sumOfEvenFibsFormula(nValues[i]));
    }
}

TEST_CASE("test when N is next to even fibonacci") {
    std::vector<unsigned long long> nValues {9, 2583};
    std::vector<unsigned long long> expected {10, 798};

    for (int i {0}; i < nValues.size(); i++) {
        CHECK_EQ(expected[i],sumOfEvenFibsNaive(nValues[i]));
        CHECK_EQ(expected[i], sumOfEvenFibsBrute(nValues[i]));
        CHECK_EQ(expected[i], sumOfEvenFibsFormula(nValues[i]));
    }
}

TEST_CASE("test upper constraints") {
    unsigned long long maxN = 4e16;
    std::vector<unsigned long long> nValues {4'000'000, maxN};
    std::vector<unsigned long long> expected {4'613'732,
                                              49'597'426'547'377'748};

    for (int i {0}; i < nValues.size(); i++) {
        CHECK_EQ(expected[i], sumOfEvenFibsNaive(nValues[i]));
        CHECK_EQ(expected[i], sumOfEvenFibsBrute(nValues[i]));
        CHECK_EQ(expected[i], sumOfEvenFibsFormula(nValues[i]));
    }
}