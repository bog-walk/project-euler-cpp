#ifndef PROJECT_EULER_CPP_BIG_INT_H
#define PROJECT_EULER_CPP_BIG_INT_H

#include <algorithm>
#include <cstring>
#include <string>
#include <stdexcept>

/**
 * Class representing data type that stores numbers with a potential for more than 20
 * digits (i.e. exceeding unsigned long long limits).
 *
 * The numbers are non-negative and standard mathematical operations are supported.
 */
class BigInt {
public:
    explicit BigInt(std::string& value)  {
        m_digits = "";
        for (std::size_t i = value.size(); i--;) {
            if (!isdigit(value[i]))
                throw std::invalid_argument("String must represent a number");
            m_digits.push_back(value[i]);
        }
    }
    explicit BigInt(const char *value)  {
        m_digits = "";
        for (std::size_t i = strlen(value); i--;) {
            if (!isdigit(value[i]))
                throw std::invalid_argument("String must represent a number");
            m_digits.push_back(value[i]);
        }
    }
    explicit BigInt(unsigned long long value)  {
        m_digits = "";
        while (value) {
            m_digits.push_back(value % 10 + '0');
            value /= 10;
        }
    }
    BigInt(const BigInt& rhs) {
        m_digits = rhs.m_digits;
    }

    friend BigInt& operator+=(BigInt&, const BigInt&);
    friend BigInt operator+(const BigInt&, const BigInt&);

    std::string toString() const
    {
        //std::string output = m_digits;
        return std::string {m_digits.rbegin(), m_digits.rend()};

        //std::reverse(output.begin(), output.end());

        //return output;
    }

protected:
    std::string m_digits;

    std::size_t length() const { return m_digits.length(); }
};

#endif //PROJECT_EULER_CPP_BIG_INT_H