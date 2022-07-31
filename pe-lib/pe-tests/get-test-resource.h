#ifndef PROJECT_EULER_CPP_GET_TEST_RESOURCE_H
#define PROJECT_EULER_CPP_GET_TEST_RESOURCE_H

#endif //PROJECT_EULER_CPP_GET_TEST_RESOURCE_H

#include <string>
#include <vector>

std::string getTestString(
        const std::string& filePath,
        const std::string& lineTrim = "[ \n]"
        );

std::vector<std::string> getTestResource(
        const std::string& filePath,
        const std::string& lineTrim = "[ \n]"
        );