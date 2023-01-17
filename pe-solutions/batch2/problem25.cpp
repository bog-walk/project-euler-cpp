/*
 * Problem 25: N-digit Fibonacci Number
 *
 * https://projecteuler.net/problem=25
 *
 * Goal: Find the first term in the Fibonacci sequence to have N digits.
 *
 * Constraints: 2 <= N <= 5000
 *
 * e.g.: N = 3
 *       Fibonacci sequence = {0,1,1,2,3,5,8,13,21,34,55,89,144}
 *       first term with 3 digits = F(12)
 */

#include <cmath>

#include "../../doctest/doctest.h"

#include "pe-custom/big-int.h"

namespace fibonacci {
    const double phi = (1 + std::sqrt(5.0)) / 2.0;
}

/*
 * Iterative solution that checks all Fibonacci numbers.
 *
 * @return array containing the first Fibonacci terms to have (index + 2) digits.
 */
unsigned long* nDigitFibTermsBrute(unsigned short maxDigits)
{
    BigInt fNMinus1 {8uLL}, fN {13uLL};

    // Variable length array declaration cannot have 'static' storage duration
    // so must use dynamically allocated array
    auto* terms = new unsigned long[maxDigits-1];
    terms[0] = 7;

    for (unsigned long term {8}, digits {3}; digits <= maxDigits; ++term) {
        // should std::swap() or std::move() be used here?
        auto fNMinus2 {fNMinus1};
        fNMinus1 = fN;
        fN = fNMinus1 + fNMinus2;
        if (fN.length() == digits) {
            terms[digits-2] = term;
            digits++;
        }
    }

    return terms;
}

/*
 * Finds the nth Fibonacci number using Binet's formula.
 *
 * The Golden Ration, Phi, provides an alternative to iteration, based on the
 * closed-form formula:
 *
 *          Fn = (Phi^n - Psi^n) / sqrt(5),
 *          with Phi = (1 + sqrt(5)) / 2 ~= 1.61803... & Psi = -Phi^-1
 *
 * Rounding, using the nearest integer function, reduces the formula to:
 *
 *          Fn = round(Phi^n / sqrt(5)), where n >= 0.
 *
 * Truncation, using the floor function, would result instead in:
 *
 *          Fn = floor((Phi^n / sqrt(5)) + 0.5), where n >= 0.
 */
inline double nthFibUsingGoldenRatio(unsigned long n)
{
    return std::round(std::pow(fibonacci::phi, n) / std::sqrt(5.0));
}

/*
 * Iterative solution uses the Golden Ratio to check all Fibonacci numbers.
 *
 * Original solution compared digit lengths by casting fN to a String. This has been
 * replaced by calling log10(fN) and comparing it to the required digits minus 1.
 *
 * N.B. This solution shows significantly slower execution when N > 10 due to the
 * exponential need to calculate larger Phi^N and resulting OverflowError.
 *
 * @return first Fibonacci term to have N digits.
 */
unsigned long nDigitFibTermGoldenBrute(unsigned short n)
{
    unsigned long term {7};
    double fN {13.0};
    // pattern shows the amount of digits increases every 4th-5th term
    const unsigned short step {4};

    while (std::log10(fN) < n - 1) {
        term += step;
        fN = nthFibUsingGoldenRatio(term);
        while (std::log10(fN) < n - 1) {
            term++;
            fN = nthFibUsingGoldenRatio(term);
        }
    }

    return term;
}

/*
 * O(n) solution based on the inversion of closed-form Binet's formula.
 *
 *          Phi^t / sqrt(5) > 10^(n-1)
 *
 *          Phi^t > 10^(n-1) * sqrt(5)
 *
 *          log(Phi)t > log(10)(n - 1) + log(5)/2
 *
 *          t > (1(n - 1) + log(5)/2) / log(Phi)
 *
 *          t = ceil((n - 1 + log(5)/2) / log(Phi))
 *
 * @return first Fibonacci term to have N digits.
 */
unsigned long nDigitFibTermGoldenFormula(unsigned short n)
{
    auto ans = std::ceil((n - 1 + std::log10(5.0) / 2) / std::log10(fibonacci::phi));

    return static_cast<unsigned long>(ans);
}

TEST_CASE("test nDigitFibTerm") {
    unsigned short digits[] {2, 3, 4, 9, 10, 1000, 5000};
    unsigned long expected[] {7, 12, 17, 40, 45, 4782, 23922};
    unsigned long* allTerms = nDigitFibTermsBrute(5000);

    for (const auto& digit: digits) {
        auto i = &digit - &digits[0];
        CHECK_EQ(expected[i], allTerms[digit-2]);
        CHECK_EQ(expected[i], nDigitFibTermGoldenFormula(digit));
    }

    delete[] allTerms;
}

TEST_CASE("test golden ratio correct for getting Nth fib") {
    unsigned long expected[] {0, 1, 1, 2, 3, 5, 8, 13,
                              21, 34, 55, 89, 144, 233, 377,
                              610, 987, 1597};

    for (int n {0}; n < 18; ++n) {
        CHECK_EQ(expected[n], static_cast<unsigned long>(nthFibUsingGoldenRatio(n)));
    }
}

TEST_CASE("test golden ratio correct for getting N-digit term") {
    unsigned long expected[] {7, 12, 17, 21, 26, 31, 36, 40,
                              45};

    for (int n {2}; n <= 10; ++n) {
        CHECK_EQ(expected[n-2], nDigitFibTermGoldenBrute(n));
    }
}