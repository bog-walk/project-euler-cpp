#ifndef PROJECT_EULER_CPP_BIG_INT_H
#define PROJECT_EULER_CPP_BIG_INT_H

#include <algorithm>
#include <cstring>
#include <string>
#include <stdexcept>

/*
 * Class representing data type that stores numbers with a potential for more than 20
 * digits (i.e. exceeding unsigned long long limits).
 *
 * The numbers are non-negative and standard mathematical operations are supported.
 */
class BigInt {
public:
    // digits are stored in reverse order
    explicit BigInt(std::string& value)  {
        m_digits.reserve(value.size());
        for (std::size_t i = value.size(); i--;) {
            if (!isdigit(value[i]))
                throw std::invalid_argument("String must represent a number");
            m_digits.push_back(value[i]);
        }
    }
    explicit BigInt(const char* value)  {
        m_digits.reserve(strlen(value));
        for (std::size_t i = strlen(value); i--;) {
            if (!isdigit(value[i]))
                throw std::invalid_argument("String must represent a number");
            m_digits.push_back(value[i]);
        }
    }
    explicit BigInt(unsigned long long value)  {
         do {
            m_digits.push_back(value % 10 + '0');
            value /= 10;
         } while (value);
    }
    BigInt(const BigInt& rhs) {
        m_digits = rhs.m_digits;
    }
    BigInt() {
        m_digits.push_back('0');
    }

    // way to create global namespace constants?
    static BigInt zero() { return BigInt {0uLL}; }
    static BigInt one() { return BigInt {1uLL}; }
    static BigInt ten() { return BigInt {10uLL}; }

    friend bool operator==(const BigInt&, const BigInt&);
    friend bool operator!=(const BigInt&, const BigInt&);
    friend bool operator<(const BigInt&, const BigInt&);
    friend bool operator>(const BigInt&, const BigInt&);
    friend bool operator<=(const BigInt&, const BigInt&);
    friend bool operator>=(const BigInt&, const BigInt&);

    // Prefix operators
    BigInt& operator++();
    BigInt& operator--();
    // Postfix operators
    // Clang-Tidy: Overloaded 'operator++' returns a non-constant object
    // instead of a constant object type?
    BigInt operator++(int temp);
    BigInt operator--(int temp);

    friend BigInt& operator+=(BigInt&, const BigInt&);
    friend BigInt operator+(const BigInt&, const BigInt&);
    friend BigInt& operator-=(BigInt&, const BigInt&);
    friend BigInt operator-(const BigInt&, const BigInt&);

    friend BigInt& operator*=(BigInt&, const BigInt&);
    friend BigInt operator*(const BigInt&, const BigInt&);
    friend BigInt& operator/=(BigInt&, const BigInt&);
    friend BigInt operator/(const BigInt&, const BigInt&);
    friend BigInt& operator%=(BigInt&, const BigInt&);
    friend BigInt operator%(const BigInt&, const BigInt&);

    BigInt pow(const BigInt& exp) const
    {
        if (exp == BigInt::zero())
            return BigInt::one();

        BigInt base {*this}, exponent {exp}, result {1uLL};

        while (!(exponent.length() == 1 && exponent.m_digits[0] == '0')) {
            if (exponent.m_digits[0] - '0' & 1)
                result *= base;
            base *= base;
            int add {};
            for (int i = exponent.length() - 1; i >= 0; --i) {
                auto digit = ((exponent.m_digits[i] - '0') >> 1) + add;
                add = (exponent.m_digits[i] - '0' & 1) * 5;
                exponent.m_digits[i] = digit + '0';
            }
            while (exponent.length() > 1 && !(exponent.m_digits.back() - '0')) {
                exponent.m_digits.pop_back();
            }
        }

        return result;
    }
    BigInt modPow(const BigInt& exp, const BigInt& mod) const
    {
        if (exp == BigInt::zero())
            return BigInt::one() % mod;
        if (exp == BigInt::one())
            return *this % mod;

        BigInt base {*this}, exponent {exp}, result {1uLL};
        // no need to invoke pow() block if base is evenly divisible by mod
        base %= mod;
        if (base == BigInt::zero())
            return base;

        while (!(exponent.length() == 1 && exponent.m_digits[0] == '0')) {
            if (exponent.m_digits[0] - '0' & 1)
                result = result * base % mod;
            base = base * base % mod;
            int add {};
            for (int i = exponent.length() - 1; i >= 0; --i) {
                auto digit = ((exponent.m_digits[i] - '0') >> 1) + add;
                add = (exponent.m_digits[i] - '0' & 1) * 5;
                exponent.m_digits[i] = digit + '0';
            }
            while (exponent.length() > 1 && !(exponent.m_digits.back() - '0')) {
                exponent.m_digits.pop_back();
            }
        }

        return result;
    }

    std::size_t length() const { return m_digits.size(); }

    std::string toString() const
    {
        return std::string {m_digits.rbegin(), m_digits.rend()};
    }
    /*
     * @throws std::out_of_range if converted value would overflow unsigned long range.
     */
    unsigned long toULong() const { return std::stoul(this -> toString()); }
    /*
     * @throws std::out_of_range if converted value would overflow unsigned long long
     * range.
     */
    unsigned long long toULLong() const { return std::stoull(this -> toString()); }

protected:
    std::vector<unsigned char> m_digits;
};

#endif //PROJECT_EULER_CPP_BIG_INT_H