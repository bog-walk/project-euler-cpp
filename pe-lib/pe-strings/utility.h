#ifndef PROJECT_EULER_CPP_UTILITY_H
#define PROJECT_EULER_CPP_UTILITY_H

#include <string>
#include <string_view>
#include <vector>

std::string trim(std::string_view line, std::string_view characters = " \n");

std::vector<std::string> split(std::string_view line, std::string_view delimiter = " ");

unsigned long digitSum(std::string_view number);

#endif //PROJECT_EULER_CPP_UTILITY_H