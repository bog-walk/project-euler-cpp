/**
 * Problem 7: The 10001st Prime
 *
 * https://projecteuler.net/problem=7
 *
 * Goal: Find the Nth prime number.
 *
 * Constraints: 1 <= N <= 10'001
 *
 * e.g.: N = 6
 *       primes = {2,3,5,7,11,13,...}
 *       6th prime = 13
 */

#include "../../doctest/doctest.h"
#include <vector>
#include "pe-maths/is-prime.h"

// Solution iterates over all numbers & checks for primality using an
// optimised helper function, until the [n]th prime is found.
// Odd numbers are not excluded from this iteration (do speed test!!!)
unsigned long getNthPrime(unsigned short n)
{
    unsigned long number {1};
    while (n > 0) {
        if (isPrime(++number))
            n--;
    }
    return number;
}

// Solution generates a list of primes of size [count] for quick-draw access.
// The dynamic list itself is used to test primality of every number based on
// the prime factorisation principle.
// The same rules for primes apply, namely that every prime after 2 is odd
// and the only factor greater than sqrt(x) would be x itself if x is prime.
// @return list of primes with the nth prime at index n-1.
std::vector<unsigned long>& getAllPrimes(unsigned short count)
{
    static std::vector<unsigned long> primes;
    primes.reserve(count);
    primes.push_back(2);
    unsigned long number {1};
    while (primes.size() < count) {
        number += 2;
        bool isPrime = true;
        for (unsigned long p : primes) {
            if (p * p > number)
                break;
            if (number % p == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
            primes.push_back(number);
    }
    return primes;
}

// Nest method for tests to share resources?
// TEXT_FIXTURE & SUBCASE have getAllPrimes() being called for every set
// TEST_SUITE only calls it once
TEST_SUITE("both P7 solutions") {
    // static storage duration warning?
    std::vector<unsigned long> allPrimes = getAllPrimes(10001);

    TEST_CASE("test lower constraints") {
        std::vector<unsigned short> nValues{1, 2, 3, 4, 5, 6};
        std::vector<unsigned long> expected{2, 3, 5, 7, 11, 13};

        for (int i = 0; i < nValues.size(); i++) {
            CHECK_EQ(expected[i], getNthPrime(nValues[i]));
            CHECK_EQ(expected[i], allPrimes[nValues[i] - 1]);
        }
    }

    TEST_CASE("test lower mid constraints") {
        std::vector<unsigned short> nValues{ 20, 62, 99, 101, 173 };
        std::vector<unsigned long> expected{ 71, 293, 523, 547, 1031 };

        for (int i = 0; i < nValues.size(); i++) {
            CHECK_EQ(expected[i], getNthPrime(nValues[i]));
            CHECK_EQ(expected[i], allPrimes[nValues[i] - 1]);
        }
    }

    TEST_CASE("test upper mid constraints") {
        std::vector<unsigned short> nValues{ 250, 289, 919, 1000, 1284 };
        std::vector<unsigned long> expected{ 1583, 1879, 7193, 7919,
                                             10499 };

        for (int i = 0; i < nValues.size(); i++) {
            CHECK_EQ(expected[i], getNthPrime(nValues[i]));
            CHECK_EQ(expected[i], allPrimes[nValues[i] - 1]);
        }
    }

    TEST_CASE("test upper constraints") {
        std::vector<unsigned short> nValues{5000, 7777, 10'000, 10'001};
        std::vector<unsigned long> expected{48611, 79357, 104'729,
                                            104'743};

        for (int i = 0; i < nValues.size(); i++) {
            CHECK_EQ(expected[i], getNthPrime(nValues[i]));
            CHECK_EQ(expected[i], allPrimes[nValues[i] - 1]);
        }
    }
}