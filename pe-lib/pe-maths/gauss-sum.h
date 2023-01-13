#ifndef PROJECT_EULER_CPP_GAUSS_SUM_H
#define PROJECT_EULER_CPP_GAUSS_SUM_H

/*
 * Calculates the sum of the first n natural numbers, based on the formula:
 *
 *      {n}Sigma{k=1} k = n * (n + 1) / 2
 *
 * Conversion of large long double types to long long types in this formula can lead to
 * large rounding losses, so integer division by 2 is replaced with a single bitwise
 * right shift, as n >> 1 = n / (2^1).
 */
inline unsigned long long gaussSum(unsigned long long n) { return n * (n + 1) >> 1; }

#endif //PROJECT_EULER_CPP_GAUSS_SUM_H