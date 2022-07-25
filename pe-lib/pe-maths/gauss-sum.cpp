#include "../../doctest/doctest.h"
#include "gauss-sum.h"

/**
 * Calculates the sum of the first n natural numbers, based on the formula:
 *
 *      {n}Sigma{k=1} k = n * (n + 1) / 2
 *
 * Conversion of large long double types to long long types in this formula can
 * lead to large rounding losses, so integer division by 2 is replaced with a
 * single bitwise right shift, as n >> 1 = n / (2^1).
 */
unsigned long long gaussSum(long long n)
{
    unsigned long long ans = n * (n + 1) >> 1;
    return ans;
}

TEST_CASE("test gaussSum() lower constraints") {
    CHECK_EQ(1, gaussSum(1));
    CHECK_EQ(3, gaussSum(2));
    CHECK_EQ(6, gaussSum(3));
}

TEST_CASE("test gaussSum() mid constraints") {
    CHECK_EQ(1275, gaussSum(50));
    CHECK_EQ(5050, gaussSum(100));
    CHECK_EQ(2'496'495, gaussSum(2234));
}