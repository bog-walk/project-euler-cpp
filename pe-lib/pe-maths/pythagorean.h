#ifndef PROJECT_EULER_CPP_PYTHAGOREAN_H
#define PROJECT_EULER_CPP_PYTHAGOREAN_H

#include <numeric>
#include <tuple>

using triple = std::tuple<unsigned long, unsigned long, unsigned long>;

/*
 * Two integers are co-prime (relatively/mutually prime) if the only positive integer
 * that is a divisor of both of them is 1.
 */
inline bool isCoPrime(unsigned long x, unsigned long y)
{
    return std::gcd(x, y) == 1;
}

triple pythagoreanTriplet(unsigned long m, unsigned long n, unsigned long d);

#endif //PROJECT_EULER_CPP_PYTHAGOREAN_H