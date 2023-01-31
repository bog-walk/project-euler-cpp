/*
 * Problem 33: Digit Cancelling Fractions
 *
 * https://projecteuler.net/problem=33
 *
 * Goal: Find every non-trivial fraction where the numerator is less than the
 * denominator (both have N-digits) and the value of the reduced fraction (by
 * cancelling K digits from num. & denom.) is equal to the original fraction.
 *
 * Constraints: 2 <= N <= 4 & 1 <= K <= N-1
 *
 * Non-Trivial Fraction: Satisfies goal's conditions, e.g. 49/98 = 4/8.
 *
 * Trivial Fraction: Fractions with trailing zeroes in both numerator and denominator
 * that allow cancellation, e.g. 30/50 = 3/5.
 *
 *
 * e.g.: N = 2, K = 1
 *       non-trivials = {16/64, 19/95, 26/65, 49/98}
 *       reduced-equivalents = {1/4, 1/5, 2/5, 4/8}
 */

#include <cmath>
#include <set>
#include <tuple>
#include <unordered_set>

#include "../../doctest/doctest.h"

#include "pe-combinatorics/cartesian-product.h"
#include "pe-combinatorics/combinations.h"

/*
 * Naive method checks if a reduced fraction is equivalent to its original fraction.
 */
bool isReducedEquivalent(int digits, int numerator, int denominator, int toCancel)
{
    const int nMod = static_cast<int>(pow(10, toCancel));
    const int dMod = static_cast<int>(pow(10, digits - toCancel));
    if (numerator % nMod == denominator / dMod) {
        auto ogFraction = 1.0 * numerator / denominator;
        auto reduced = 1.0 * (numerator / nMod) / (denominator % dMod);
        return ogFraction == reduced;
    }

    return false;
}

/*
 * Brute iteration through all numerators and denominators with the expected amount of
 * digits, & following constraints specified in problem above.
 *
 * @return vector of pairs(numerator, denominator) sorted by numerator.
 */
std::vector<std::pair<int, int>> findNonTrivialsBrute(int n, int k = 1)
{
    std::vector<std::pair<int, int>> nonTrivials;

    const int minN = static_cast<int>(pow(10, n - 1) + 1);
    const int maxD = static_cast<int>(pow(10, n));
    for (int num {minN}; num < maxD / 2; ++num) {
        for (int denom = num + 1; denom < maxD; ++denom) {
            if (!(denom % 10))
                continue;
            if (isReducedEquivalent(n, num, denom, k))
                nonTrivials.emplace_back(num, denom);
        }
    }

    return nonTrivials;
}

/*
 * Project Euler specific implementation that requires all non-trivial fractions that
 * have 2 digits (pre-cancellation of 1 digit) to be found.
 *
 * @return the denominator of the product of the fractions given in their lowest common
 * terms.
 */
int productOfNonTrivials()
{
    auto nonTrivials = findNonTrivialsBrute(2);
    auto [nProd, dProd] = std::accumulate(
            nonTrivials.cbegin(),
            nonTrivials.cend(),
            std::pair {1, 1},
            [](std::pair<int, int> acc, const auto& pair) {
                return std::pair {acc.first * pair.first, acc.second * pair.second};
            });

    return dProd / std::gcd(nProd, dProd);
}

/*
 * Solution limits loops by pre-cancelling possible digits, rather than checking each
 * iteration to see if cancelled digits match. This pre-reduces all numerators &
 * denominators, which reduces iteration by power of 10.
 *
 * Loop nesting is based on numerator < denominator & cancelled < cancelledMax.
 *
 * This order of solutions is based on the combination equation:
 *
 *      (n10^k + c) / (c10^k + d) = n/d
 *
 * which reduces to:
 *
 *      n(10^k -1)(c - d) = c(d - n)
 *
 * @return vector of pairs(numerator, denominator) sorted by numerator.
 */
std::vector<std::pair<int, int>> findNonTrivials(int n, int k = 1)
{
    std::vector<std::pair<int, int>> nonTrivials;

    const int cancelledMin = static_cast<int>(pow(10, k - 1));
    const int cancelledMax = static_cast<int>(pow(10, k));
    const int reducedMin = static_cast<int>(pow(10, n - k - 1));
    const int reducedMax = static_cast<int>(pow(10, n - k));
    for (int cancelled {cancelledMin}; cancelled < cancelledMax; ++cancelled) {
        for (int d2 = reducedMin + 1; d2 < reducedMax; ++d2) {
            for (int n2 {reducedMin}; n2 < d2; ++n2) {
                auto numerator = n2 * cancelledMax + cancelled;
                auto denominator = cancelled * reducedMax + d2;
                if (n2 * denominator == numerator * d2)
                    nonTrivials.emplace_back(numerator, denominator);
            }
        }
    }

    std::sort(
            nonTrivials.begin(),
            nonTrivials.end(),
            [](std::pair<int, int> pair1, std::pair<int, int> pair2) {
                return pair1.first < pair2.first;
            });
    return nonTrivials;
}

/*
 * Finds all combinations for digits cancelled from a number based on the indices of
 * the digits to be cancelled. Ensures no combinations have duplicate digits or
 * duplicate integer outputs.
 *
 * @return set of post-cancellation integers.
 */
std::set<int> getCancelledCombos(std::string num, std::vector<char>& combo)
{
    auto k = combo.size();
    std::set<int> original;
    for (int i {0}; i < num.size(); ++i) {
        original.insert(i);
    }
    // e.g. num = "9919" with cancelCombo = ('9','9')
    // indices = ((0,1,3), (0,1,3))
    std::vector<std::vector<int>> indices;
    for (int ch {0}; ch < k; ++ch) {
        std::vector<int> toAdd;
        for (auto& i : original) {
            if (num[i] == combo[ch])
                toAdd.push_back(i);
        }
        indices.push_back(toAdd);
    }

    auto products = product<int>(indices);
    std::vector<std::set<int>> productSets;
    // products to set returns {0,1}, {0,3}, {1,0}, {1, 3}... with {0,0},etc
    // reduced to {0}
    for (auto& prod : products) {
        std::set<int> prodS(prod.cbegin(), prod.cend());
        productSets.push_back(prodS);
    }

    std::set<int> results;
    for (const auto& comboSet : productSets) {
        // ignore combos reduced due to duplicate indices
        if (comboSet.size() == k) {
            // e.g. {0,1,2,3} - {0,1} = {2,3}
            std::set<int> diff;
            std::set_difference(original.begin(), original.end(),
                                comboSet.begin(), comboSet.end(),
                                std::inserter(diff, diff.begin()));
            std::string s;
            for (auto& ch : diff) {
                s += num[ch];
            }
            results.insert(std::stoi(s));
        }
    }

    return results;
}

/*
 * HackerRank specific implementation that includes extra restrictions that are not
 * clearly specified on the problem page:
 *
 *      - The digits cancelled from the numerator and denominator can be in any order.
 *      e.g. 1306/6530 == 10/50 and 6483/8644 == 3/5.
 *
 *      - Zeroes should not be cancelled, but leading zeroes are allowed as they will
 *      be read as if removed.
 *      e.g. 4808/8414 == 08/14 == 8/14 and 490/980 == 40/80.
 *
 *      - Pre-cancelled fractions must only be counted once, even if the cancelled
 *      digits can be removed in different ways with the same output.
 *      e.g. 1616/6464 == 161/644 == 116/464.
 *
 * N.B. Solution does not tolerate K > 1 well.
 *
 * @return pair of (sum of numerators, sum of denominators).
 */
std::pair<int, int> sumOfNonTrivialsBrute(int n, int k)
{
    int nSum {}, dSum {};
    const int minNumerator = static_cast<int>(pow(10, n - 1) + 2);
    const int maxDenominator = static_cast<int>(pow(10, n));

    for (int numerator {minNumerator}; numerator <= maxDenominator - 2; ++numerator) {
        const std::string nS = std::to_string(numerator);
        std::string nSWithoutZero;
        std::copy_if(nS.cbegin(), nS.cend(),
                     std::back_inserter(nSWithoutZero),
                     [](const auto& ch) { return ch != '0'; });
        auto cancelCombos = Combinations<char>(k, nSWithoutZero).toList();
        for (int denom = numerator + 1; denom < maxDenominator; ++denom) {
            bool found {false};
            auto ogFraction = 1.0 * numerator / denom;
            for (auto& combo : cancelCombos) {
                auto nPost = getCancelledCombos(nS, combo);
                auto dPost = getCancelledCombos(std::to_string(denom), combo);
                // denominator did not contain all digits to cancel
                if (dPost.empty())
                    continue;
                for (auto& n2 : nPost) {
                    if (!n2)
                        continue;
                    for (auto& d2 : dPost) {
                        // avoid division by zero error
                        if (!d2)
                            continue;
                        if (ogFraction == 1.0 * n2 / d2) {
                            nSum += numerator;
                            dSum += denom;
                            // avoid duplicating numerator with this denominator
                            found = true;
                            break;
                        }
                    }
                    if (found)
                        break;
                }
                if (found)
                    break;
            }
        }
    }

    return {nSum, dSum};
}

/*
 * HackerRank specific implementation with extra restrictions, as detailed in the above
 * brute force solution.
 *
 * This solution has been optimised by only looping through possible numerators & the
 * cancellation combos they allow. Rather than loop through denominators, std::gcd() is
 * used to assess reductive equivalence based on the following:
 *
 *      n_og / d_og = n_r / d_r, and
 *
 *      n_r = n_og / gcd(n_og, d_og)
 *
 *      d_r = d_og / gcd(n_og, d_og)
 *
 * @return pair of (sum of numerators, sum of denominators).
 */
std::pair<int, int> sumOfNonTrivialsGCD(int n, int k)
{
    int nSum {}, dSum {};
    const auto minNumerator = static_cast<int>(pow(10, n - 1));
    const auto maxDenominator = static_cast<int>(pow(10, n));
    const auto maxReduced = static_cast<int>(pow(10, n - k));

    for (int numer {minNumerator}; numer <= maxDenominator - 2; ++numer) {
        const std::string nS = std::to_string(numer);
        std::string nSWithoutZero;
        std::copy_if(nS.cbegin(), nS.cend(),
                     std::back_inserter(nSWithoutZero),
                     [](const auto& ch) { return ch != '0'; });
        auto cancelCombos = Combinations<char>(k, nSWithoutZero).toList();
        // avoid denominator duplicates with same numer
        std::unordered_set<int> denominatorsUsed;
        for (auto& combo : cancelCombos) {
            // get all integers possible post-cancellation of k digits
            auto nPost = getCancelledCombos(nS, combo);
            for (auto& n2 : nPost) {
                if (!n2)
                    continue;
                int d {numer}, nr {n2}, i {1};
                while (true) {
                    i++;
                    auto g = std::gcd(d, nr);
                    d = d / g * i;
                    nr = nr / g * i;
                    if (d <= numer)
                        continue;
                    if (nr >= maxReduced || d >= maxDenominator)
                        break;
                    auto dPost = getCancelledCombos(std::to_string(d), combo);
                    // denominator did not contain all digits to cancel
                    if (dPost.empty())
                        continue;
                    for (auto& d2: dPost) {
                        if (nr == d2 && denominatorsUsed.count(d) == 0) {
                            nSum += numer;
                            dSum += d;
                            denominatorsUsed.insert(d);
                        }
                    }
                }
            }
        }
    }

    return {nSum, dSum};
}

TEST_SUITE("test isReducedEquivalent()") {
    TEST_CASE("with valid fractions") {
        std::pair<int, int> fractions[] {{16, 64}, {26, 65}, {1249, 9992},
                                         {4999, 9998}};
        int k {1};

        for (const auto& [num, denom] : fractions) {
            auto digits = std::to_string(num).length();
            CHECK(isReducedEquivalent(digits, num, denom, k));
        }
    }

    TEST_CASE("with invalid fractions") {
        std::pair<int, int> fractions[] {{11, 19}, {47, 71}, {328, 859},
                                         {8777, 7743}};
        int k {1};

        for (const auto& [num, denom] : fractions) {
            auto digits = std::to_string(num).length();
            CHECK_FALSE(isReducedEquivalent(digits, num, denom, k));
        }
    }

    TEST_CASE("with different k") {
        std::pair<int, int> fractions[] {{16, 64}, {166, 664}, {1666, 6664}};

        for (const auto& [num, denom] : fractions) {
            auto digits = std::to_string(num).length();
            for (int k {1}; k <= 3; ++k) {
                if (k >= digits)
                    break;
                CHECK(isReducedEquivalent(digits, num, denom, k));
            }
        }
    }
}

TEST_SUITE("test findNonTrivials()") {
    TEST_CASE("when K = 1") {
        std::vector<std::pair<int, int>> expected[] {
            {{16, 64}, {19, 95}, {26, 65}, {49, 98}},
            {{166, 664}, {199, 995}, {217, 775}, {249, 996},
             {266, 665}, {499, 998}},
             {{1249, 9992}, {1666, 6664}, {1999, 9995}, {2177, 7775},
              {2499, 9996}, {2666, 6665}, {4999, 9998}}};

        for (int n {2}; n <= 4; ++n) {
            CHECK_EQ(expected[n-2], findNonTrivialsBrute(n));
            CHECK_EQ(expected[n-2], findNonTrivials(n));
        }
    }

    TEST_CASE("when K = 2") {
        int n {3}, k {2};
        std::vector<std::pair<int, int>> expected {{166, 664}, {199, 995}, {266, 665},
                                                   {484, 847}, {499, 998}};

        CHECK_EQ(expected, findNonTrivialsBrute(n, k));
        CHECK_EQ(expected, findNonTrivials(n, k));
    }

    TEST_CASE("when N = 4") {
        // sums of numerators & denominators
        int n {4}, expectedNSum {17255}, expectedDSum {61085};

        auto bruteActual = findNonTrivialsBrute(n);
        auto [actualBNSum, actualBDSum] = std::accumulate(
                bruteActual.cbegin(),
                bruteActual.cend(),
                std::pair {0, 0},
                [](std::pair<int, int> acc, const std::pair<int, int>& pair) {
            return std::pair {acc.first + pair.first, acc.second + pair.second};
        });

        auto actual = findNonTrivials(n);
        auto [actualNSum, actualDSum] = std::accumulate(
                actual.cbegin(),
                actual.cend(),
                std::pair {0, 0},
                [](std::pair<int, int> acc, const std::pair<int, int>& pair) {
                    return std::pair {acc.first + pair.first, acc.second + pair.second};
                });

        CHECK_EQ(expectedNSum, actualBNSum);
        CHECK_EQ(expectedNSum, actualNSum);
        CHECK_EQ(expectedDSum, actualBDSum);
        CHECK_EQ(expectedDSum, actualDSum);
    }
}

TEST_CASE("test PE problem") {
    CHECK_EQ(100, productOfNonTrivials());
}

TEST_CASE("test getCancelledCombos()") {
    std::string nums[] {"9919", "1233", "1051", "5959"};
    std::vector<char> toCancel[] {{'9', '9'}, {'1', '2', '3'},
                                  {'1', '5'}, {'9'}};
    std::set<int> expected[] {{19, 91}, {3}, {1, 10},
                                        {559, 595}};

    for (const auto& n: nums) {
        auto i = &n - &nums[0];
        CHECK_EQ(expected[i], getCancelledCombos(n, toCancel[i]));
    }
}

TEST_SUITE("test HR problem") {
    TEST_CASE("when K = 1") {
        int k {1};
        std::pair<int, int> expected[] {{110, 322}, {77262, 163'829}};

        for (int n {2}; n <= 3; ++n) {
            CHECK_EQ(expected[n-2], sumOfNonTrivialsBrute(n, k));
            CHECK_EQ(expected[n-2], sumOfNonTrivialsGCD(n, k));
        }
    }

    TEST_CASE("when K = 2") {
        int k {2}, n {3};
        std::pair<int, int> expected {7429, 17305 };

        CHECK_EQ(expected, sumOfNonTrivialsBrute(n, k));
        CHECK_EQ(expected, sumOfNonTrivialsGCD(n, k));
    }

    TEST_CASE("when N = 4") {
        int n {4};
        std::pair<int, int> expected[] {{12'999'936, 28'131'911},
                                        {3'571'225, 7'153'900}, {255'983, 467'405}};

        for (int k {1}; k <= 3; ++k) {
            CHECK_EQ(expected[k-1], sumOfNonTrivialsGCD(n, k));
        }
    }
}