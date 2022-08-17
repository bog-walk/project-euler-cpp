#ifndef PROJECT_EULER_CPP_UTILITY_H
#define PROJECT_EULER_CPP_UTILITY_H

#include <string>
#include <string_view>
#include <vector>

std::string trim(std::string_view line, std::string_view characters);

std::vector<std::string> split(std::string_view line, std::string_view delimiter = " ");

#endif //PROJECT_EULER_CPP_UTILITY_H