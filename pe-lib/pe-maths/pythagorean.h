#ifndef PROJECT_EULER_CPP_PYTHAGOREAN_H
#define PROJECT_EULER_CPP_PYTHAGOREAN_H

#include <tuple>

using triple = std::tuple<unsigned long, unsigned long, unsigned long>;

bool isCoPrime(unsigned long x, unsigned long y);

triple pythagoreanTriplet(unsigned long m, unsigned long n, unsigned long d);

#endif //PROJECT_EULER_CPP_PYTHAGOREAN_H