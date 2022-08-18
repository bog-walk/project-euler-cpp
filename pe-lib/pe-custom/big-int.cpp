#include "big-int.h"

#include "../../doctest/doctest.h"

// friend modifier negates need for syntax BigInt::operator+-()
BigInt& operator+=(BigInt& a, const BigInt& b) {
    std::size_t aS = a.length(), bS = b.length();
    if (bS > aS) {  // modified BigInt may be of larger length
        a.m_digits.append(bS - aS, '0');
        aS = a.length();
    }

    unsigned short carryOver {0}, sum;
    for (int i {0}; i < aS; ++i) {
        if (i < bS) {
            sum = a.m_digits[i] - '0' + b.m_digits[i] - '0' + carryOver;
        } else {  // const BigInt may be of smaller length
            sum = a.m_digits[i] - '0' + carryOver;
        }
        carryOver = sum / 10;
        a.m_digits[i] = sum % 10 + '0';
    }
    if (carryOver)
        a.m_digits.push_back(carryOver + '0');
    return a;
}

BigInt operator+(const BigInt& a, const BigInt& b)
{
    BigInt temp {a};
    temp += b;
    return temp;
}

TEST_SUITE("test BigInt") {
    TEST_CASE("consructors correct") {
        std::string number {"1234567890"};
        unsigned long long num {1234567890};

        const BigInt a {number};
        BigInt b {"1234567890"};
        BigInt c {num};
        BigInt d {b};

        CHECK_EQ(number, a.toString());
        CHECK_EQ(number, b.toString());
        CHECK_EQ(number, c.toString());
        CHECK_EQ(number, d.toString());
    }

    TEST_CASE("addition when same length") {
        BigInt a {"1234567890"};
        BigInt b {"1234567890"};
        std::string expected {"2469135780"};

        CHECK_EQ(expected, (a + b).toString());
    }

    TEST_CASE("addition when first number longer") {
        BigInt a {"1234567890"};
        BigInt b {"1234"};
        std::string expected {"1234569124"};

        CHECK_EQ(expected, (a + b).toString());
    }

    TEST_CASE("addition when first number shorter") {
        BigInt a {"1234"};
        BigInt b {"1234567890"};
        std::string expected {"1234569124"};

        CHECK_EQ(expected, (a + b).toString());
    }

    TEST_CASE("addition assignment") {
        BigInt a {"1234567890"};
        BigInt b {"1234567890"};
        a += b;
        std::string expected {"2469135780"};

        CHECK_EQ(expected, a.toString());
    }
}