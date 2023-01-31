/*
 * Problem 31: Coin Sums
 *
 * https://projecteuler.net/problem=31
 *
 * Goal: Count the number of ways (mod 1e9 + 7) that N pence can be made using any
 * combination of English coins.
 *
 * Constraints: 1 <= N <= 1e5
 *
 * English currency: There are 8 types of coins in circulation ->
 * {1p, 2p, 5p, 10p, 20p, 50p, 1 pound (= 100p), 2 pound (= 200p)}.
 *
 * e.g.: N = 5 (i.e. goal is 5 pence in coins)
 *       combos = [{5p}, {2p, 2p, 1p}, {2p, 1p, 1p, 1p}, {1p, 1p, 1p, 1p, 1p}]
 *       count = 4
 */

#include "../../doctest/doctest.h"

#include "pe-custom/big-int.h"

namespace coinSum {
    const BigInt zero {BigInt::zero()}, one {BigInt::one()};
    const unsigned long long modulus {1'000'000'007uLL};
    const int coins[] {1, 2, 5, 10, 20, 50, 100, 200};
}

/*
 * Repeatedly subtract each coin value from the target value & sum combos previously
 * calculated for smaller targets.
 */
BigInt recursiveCombos(int n, int coin, std::vector<std::vector<BigInt>>& cache)
{
    if (coin < 1)
        return coinSum::one;
    if (cache[n][coin] > coinSum::zero)
        return cache[n][coin];

    int target {n};
    BigInt combos {coinSum::zero};
    while (target >= 0) {
        combos += recursiveCombos(target, coin - 1, cache);
        target -= coinSum::coins[coin];
    }
    cache[n][coin] = combos;

    return combos;
}

/*
 * Recursive solution uses helper function to allow memoization using top-down, thereby
 * optimising this top-down approach.
 *
 * @param [n] total amount that needs to be achieved by all combinations.
 * @param [coin] index of coin value from array coins. Default is the largest coin
 * available (2 pounds). This parameter allows flexibility in the method purpose. e.g.
 * Count combos for making 10p using 2p (& lower coins) = 6, instead of making 10p
 * using all possible coins = 11 combos.
 */
unsigned long countCoinCombosRecursive(int n, int coin = 7)
{
    const BigInt modulus {coinSum::modulus};
    std::vector<std::vector<BigInt>> recursiveMemo(
            100'001,
            {coinSum::zero, coinSum::zero, coinSum::zero, coinSum::zero, coinSum::zero,
             coinSum::zero, coinSum::zero, coinSum::zero}
            );

    auto ans = recursiveCombos(n, coin, recursiveMemo);
    return (ans % modulus).toULong();
}

/*
 * Solution uses bottom-up approach that determines a target's combo based on:
 *
 *      - The previous combo calculated for the coin with a smaller target, &
 *
 *      - The previous combo calculated for a coin of lesser value.
 */
unsigned long countCoinCombos(int n)
{
        // index 0 exists for when 0p is needed
        std::vector<unsigned long long> combosByCoin(n + 1);
        combosByCoin[0] = 1uLL;

        for (auto& coin : coinSum::coins) {
            for (int i {coin}; i <= n; ++i) {
                combosByCoin[i] += combosByCoin[i-coin];
                combosByCoin[i] %= coinSum::modulus;
            }
        }

        return combosByCoin[n];
}

TEST_CASE("test lower constraints") {
    unsigned long expected[] {1, 2, 2, 3, 4, 5, 6, 7, 8, 11};

    for (int n {1}; n <= 10; ++n) {
        CHECK_EQ(expected[n-1], countCoinCombos(n));
        CHECK_EQ(expected[n-1], countCoinCombosRecursive(n));
    }
}

TEST_CASE("test mid constraints") {
    int nValues[] {15, 20, 50, 200, 500, 1000, 10'000};
    unsigned long expected[] {22, 41, 451, 73682, 6'295'434, 321'335'886,
                              296'710'490};

    for (const auto& n: nValues) {
        auto i = &n - &nValues[0];
        CHECK_EQ(expected[i], countCoinCombos(n));
        CHECK_EQ(expected[i], countCoinCombosRecursive(n));
    }
}

TEST_CASE("test upper constraints") {
    int n {100'000};
    unsigned long expected {836'633'026};

    CHECK_EQ(expected, countCoinCombos(n));
}