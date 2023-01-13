#ifndef PROJECT_EULER_CPP_PRIME_FACTORS_H
#define PROJECT_EULER_CPP_PRIME_FACTORS_H

#include <map>
#include <vector>

using pfMap = std::map<unsigned long long, unsigned long>;

pfMap primeFactors(unsigned long long n);

std::vector<unsigned long long> primeFactorsFlattened(const pfMap& factors);

#endif //PROJECT_EULER_CPP_PRIME_FACTORS_H