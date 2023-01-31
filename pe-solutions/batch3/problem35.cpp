/*
 * Problem 35: Circular Primes
 *
 * https://projecteuler.net/problem=35
 *
 * Goal: Find the sum of all circular primes less than N, with rotations allowed to
 * exceed N, & the rotations themselves allowed as duplicates if below N.
 *
 * Constraints: 10 <= N <= 1e6
 *
 * Circular Prime: All rotations of the number's digits are themselves prime.
 * e.g. 197 -> {197, 719, 971}.
 *
 * e.g.: N = 100
 *       circular primes = {2,3,5,7,11,13,17,31,37,71,73,79,97}
 *       sum = 446
 */

#include <algorithm>
#include <numeric>
#include <string>
#include <unordered_set>

#include "../../doctest/doctest.h"

#include "pe-maths/is-prime.h"
#include "pe-maths/primes.h"

std::unordered_set<unsigned long> getRotations(unsigned long num)
{
    std::unordered_set<unsigned long> rotations {num};

    std::string rotation {std::to_string(num)};
    // number of possible rotations minus original already listed
    unsigned short r = rotation.length() - 1;
    // a more elegant way to perform fixed no-conditional repeats?
    for (unsigned short i {0}; i < r; ++i) {
        rotation = rotation.substr(r) + rotation.substr(0, r);
        rotations.insert(std::stoul(rotation));
    }

    return rotations;
}

/*
 * Solution is optimised by filtering out primes with any even digits as an even digit
 * means at least 1 rotation will be even and therefore not prime.
 *
 * The primes list is also searched using a binary search algorithm.
 *
 * @return unsorted list of circular primes < n.
 */
std::vector<unsigned long> getCircularPrimes(unsigned long n)
{
    auto primes = primeNumbers(n - 1);
    std::string evenDigits {"02468"};
    // lambda checks if any prime greater than 2 has even digits
    auto filterOut = [&evenDigits](const unsigned long& p) {
        std::string pS {std::to_string(p)};
        return p > 2 && std::any_of(pS.cbegin(), pS.cend(),
                                    [&evenDigits](const auto& ch) {
                    return evenDigits.find(ch) != std::string::npos;
        });
    };
    // in-place filter a vector with the erase-remove idiom
    primes.erase(std::remove_if(primes.begin(), primes.end(), filterOut),
                 primes.end());
    if (n == 10)
        return primes;

    std::vector<unsigned long> circularPrimes;
    auto isDuplicate = [&circularPrimes](const unsigned long& r) {
        return std::find(
                circularPrimes.cbegin(), circularPrimes.cend(), r
                ) != circularPrimes.cend();
    };
    auto primeNotFound = [&primes](const unsigned long& r) {
        return !std::binary_search(primes.cbegin(), primes.cend(), r);
    };

    for (const auto& p : primes) {
        if (p < 10) {
            circularPrimes.push_back(p);
        }
        else {
            auto pRotated = getRotations(p);
            // avoid duplicates and non-primes
            if (std::none_of(pRotated.cbegin(), pRotated.cend(),
                            [&isDuplicate, &primeNotFound, &n](const auto& r) {
                return isDuplicate(r) || r < n && primeNotFound(r) || !isPrime(r);
            })) {
                for (const auto& r : pRotated) {
                    if (r < n)
                        circularPrimes.push_back(r);
                }
            }
        }
    }

    return circularPrimes;
}

/*
 * Increase this solution's efficiency by only using Sieve of Eratosthenes once to
 * calculate all primes less than the upper constraint.
 */
unsigned long sumOfCircularPrimes(unsigned long n)
{
    auto result = getCircularPrimes(n);
    return std::accumulate(result.cbegin(), result.cend(), 0uL);
}

TEST_CASE("test getCircularPrimes() with lower constraint") {
    unsigned long n {100};
    std::vector<unsigned long> expected {2, 3, 5, 7, 11, 13, 17, 31, 37, 71, 73, 79, 97};
    auto actual = getCircularPrimes(n);
    std::sort(actual.begin(), actual.end());

    CHECK_EQ(expected, actual);
}

TEST_CASE("test getCircularPrimes() with upper constraint") {
    unsigned long n {1'000'000};
    std::size_t expectedLength {55};
    auto actual = getCircularPrimes(n);

    CHECK_EQ(expectedLength, actual.size());
}

TEST_CASE("test sumOfCircularPrimes() with lower constraints") {
    unsigned long nValues[] {10, 50, 100, 200};
    unsigned long expected[] {17, 126, 446, 1086};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], sumOfCircularPrimes(n));
    }
}

TEST_CASE("test sumOfCircularPrimes() with upper constraint") {
    unsigned long n {1'000'000};
    unsigned long expected {8'184'200};

    CHECK_EQ(expected, sumOfCircularPrimes(n));
}