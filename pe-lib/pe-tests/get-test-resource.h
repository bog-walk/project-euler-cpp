#ifndef PROJECT_EULER_CPP_GET_TEST_RESOURCE_H
#define PROJECT_EULER_CPP_GET_TEST_RESOURCE_H

#include <string_view>
#include <vector>

std::string getTestString(std::string_view filePath, std::string_view lineTrim = " \n");

std::vector<std::string> getTestResource(std::string_view filePath,
                                         std::string_view lineTrim = " \n");

#endif //PROJECT_EULER_CPP_GET_TEST_RESOURCE_H