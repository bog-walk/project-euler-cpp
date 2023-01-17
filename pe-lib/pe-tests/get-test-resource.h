#ifndef PROJECT_EULER_CPP_GET_TEST_RESOURCE_H
#define PROJECT_EULER_CPP_GET_TEST_RESOURCE_H

#include <fstream>
#include <functional>

#include "../pe-strings/utility.cpp"

/*
 * Retrieves content of a test resource file as a single std::string.
 *
 * @param lineTrim characters to remove from the left and right of each file line.
 */
std::string getTestString(const std::string& filePath,
                          std::string_view lineTrim = " \n")
{
    std::string resource;
    std::ifstream resFile(filePath);

    if (resFile.is_open()) {
        std::string line {};
        while (std::getline(resFile, line)) {
            // technically, end-of-line is extracted but not stored by getline()
            // so no need to trim '\n'?
            resource.append(trim(line, lineTrim));
        }
        resFile.close();
    }

    return resource;
}

/*
 * Retrieves content of a test resource file, with each line returned as a trimmed, but
 * otherwise unaltered std::string.
 *
 * @param lineTrim characters to remove from the left and right of each file line.
 */
std::vector<std::string> getTestResource(const std::string& filePath,
                                         const std::string_view lineTrim = " \n")
{
    std::vector<std::string> resource;
    std::ifstream resFile(filePath);

    if (resFile.is_open()) {
        std::string line {};
        while (std::getline(resFile, line)) {
            resource.push_back(trim(line, lineTrim));
        }
        resFile.close();
    }

    return resource;
}

/*
 * Retrieves content of a test resource file, with each line transformed into a nested
 * std::vector.
 *
 * @param lineTrim characters to remove from the left and right of each file line.
 * @param lineSplit characters to use as the delimiter when splitting a line.
 * @param transformation function that takes either an entire line as an argument or,
 * if split, individual elements in a line.
 */
template <typename T>
std::vector<std::vector<T>> getTestResource(const std::string& filePath,
                                      const std::function<T(std::string)>& transformation,
                                            std::string_view lineSplit = " ",
                                            std::string_view lineTrim = " \n")
{
    // Implementation of non-specialised templates must be visible in header for compiler.
    // Implementation could be extracted to a separate header file if linked after
    // declaration?
    std::vector<std::vector<T>> resource;
    std::ifstream resFile(filePath);

    if (resFile.is_open()) {
        std::string line {};
        // getLine has delimiter arg but only for single characters
        while (std::getline(resFile, line)) {
            auto elements = split(trim(line, lineTrim), lineSplit);

            std::vector<T> transformed;
            transformed.reserve(elements.size());
            // transform() does not guarantee in-order application of unary_op
            std::for_each(
                    elements.cbegin(),
                    elements.cend(),
                    [&transformation, &transformed](const std::string& str) {
                        transformed.push_back(transformation(str));
                    });

            resource.push_back(transformed);
        }
        resFile.close();
    }

    return resource;
}

/*
 * Transforms content of a test resource file into a 2D array with elements of provided
 * type T.
 *
 * @param lineSplit characters to use as the delimiter when splitting a line.
 */
template <typename T>
T** getTestGrid(const std::string& filePath, unsigned short gridSize,
                std::string_view lineSplit = " ")
{
    std::function<T(std::string)> op;
    if (typeid(T) == typeid(unsigned short)) {
        op = [](const std::string& s) {
            return static_cast<unsigned short>(std::stoi(s));
        };
    }
    else if (typeid(T) == typeid(unsigned long)) {
        op = [](const std::string& s) {
            return std::stoul(s);
        };
    }

    const auto resource = getTestResource<T>(filePath, op, lineSplit);

    auto* arr = new T*[gridSize];
    for (int row {0}; row < gridSize; ++row) {
        arr[row] = new T[gridSize];
        for (int col {0}; col < gridSize; ++col) {
            arr[row][col] = resource[row][col];
        }
    }

    return arr;
}

#endif //PROJECT_EULER_CPP_GET_TEST_RESOURCE_H