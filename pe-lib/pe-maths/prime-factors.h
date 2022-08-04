#ifndef PROJECT_EULER_CPP_PRIME_FACTORS_H
#define PROJECT_EULER_CPP_PRIME_FACTORS_H

#include <map>

using pfMap = std::map<unsigned long long, unsigned long>;

pfMap primeFactors(unsigned long long n);

#endif //PROJECT_EULER_CPP_PRIME_FACTORS_H