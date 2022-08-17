#ifndef PROJECT_EULER_CPP_GET_TEST_RESOURCE_H
#define PROJECT_EULER_CPP_GET_TEST_RESOURCE_H

#include <fstream>
#include <functional>
#include <string_view>
#include <vector>

#include "../pe-strings/utility.h"

std::string getTestString(std::string_view filePath, std::string_view lineTrim = " \n");

std::vector<std::string> getTestResource(std::string_view filePath,
                                         std::string_view lineTrim = " \n");

// implementation of non-specialised templates must be visible in header for compiler.
// implementation could be extracted to a separate header file if linked after
// declaration.
/**
 * Retrieves content of a test resource file, with each line transformed into a nested
 * std::vector.
 *
 * @param lineTrim characters to remove from the left and right of each file line.
 * @param lineSplit characters to use as the delimiter when splitting a line.
 * @param transformation function that takes either an entire line as an argument or,
 * if split, individual elements in a line.
 */
template <typename T>
std::vector<std::vector<T>> getTestResource(
        std::string_view filePath,
        std::function<T(std::string)> transformation,
        std::string_view lineSplit = " ",
        std::string_view lineTrim = " \n")
{
    std::vector<std::vector<T>> resource {};
    std::ifstream myFile(static_cast<std::string>(filePath));

    if (myFile.is_open()) {
        std::string line {};
        // getLine has delimiter arg but only for single characters :(
        while (std::getline(myFile, line)) {
            auto elements = split(trim(line, lineTrim), lineSplit);
            std::vector<T> transformed {};
            std::for_each(
                    elements.begin(),
                    elements.end(),
                    [&transformation, &transformed](const std::string& str) {
                        transformed.push_back(transformation(str));
                    });
            resource.push_back(transformed);
        }
        myFile.close();
    }

    return resource;
}

/**
 * Transforms content of a test resource file into a 2D array with elements of provided
 * type T.
 *
 * @param lineSplit characters to use as the delimiter when splitting a line.
 */
template <typename T>
T** getTestGrid(std::string_view filePath,
                unsigned short gridSize,
                std::string_view lineSplit = " ")
{
    std::function<T(std::string)> op;
    if (typeid(T) == typeid(unsigned short)) {
        op = [](const std::string& s) {
            return static_cast<unsigned short>(std::stoi(s));
        };
    } else if (typeid(T) == typeid(unsigned long)) {
        op = [](const std::string& s) {
            return std::stoul(s);
        };
    }

    const auto input = getTestResource<T>(filePath, op, lineSplit);

    auto *arr = new T*[gridSize];
    for (int row {0}; row < gridSize; ++row) {
        arr[row] = new T[gridSize];
        for (int col {0}; col < gridSize; ++col) {
            arr[row][col] = input[row][col];
        }
    }

    return arr;
}

#endif //PROJECT_EULER_CPP_GET_TEST_RESOURCE_H