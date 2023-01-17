#ifndef PROJECT_EULER_CPP_UTILITY_H
#define PROJECT_EULER_CPP_UTILITY_H

#include <string>
#include <vector>

std::string trim(std::string_view line, std::string_view characters = " \n");

std::vector<std::string> split(std::string_view line, std::string_view delimiter = " ");

unsigned long characterSum(std::string_view str, char normaliser = '0');

#endif //PROJECT_EULER_CPP_UTILITY_H