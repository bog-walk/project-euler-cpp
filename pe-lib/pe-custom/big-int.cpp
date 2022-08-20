#include "big-int.h"

#include "../../doctest/doctest.h"

// friend modifier negates need for syntax BigInt::operator==()
bool operator==(const BigInt& a, const BigInt& b) { return a.m_digits == b.m_digits; }
bool operator!=(const BigInt& a, const BigInt& b) { return !(a == b); }
bool operator<(const BigInt& a, const BigInt& b)
{
    std::size_t aS = a.length(), bS = b.length();
    if (aS != bS)
        return aS < bS;

    while (aS--) {
        if (a.m_digits[aS] != b.m_digits[aS])
            return a.m_digits[aS] < b.m_digits[aS];
    }

    return false;
}
bool operator>(const BigInt& a, const BigInt& b) { return b < a; }
bool operator<=(const BigInt& a, const BigInt& b) { return !(a > b); }
bool operator>=(const BigInt& a, const BigInt& b) { return !(a < b); }

BigInt& BigInt::operator++()
{
    std::size_t aS = length();
    int i;
    for (i = 0; i < aS && m_digits[i] == '9'; ++i) {
        m_digits[i] = '0';
    }
    if (i == aS)
        m_digits.push_back('1');
    else
        m_digits[i] = (m_digits[i] - '0') + 1 + '0';

    return *this;
}
BigInt& BigInt::operator--()
{
    if (m_digits.empty()) {
        throw std::runtime_error("Negative values not supported");
    }

    std::size_t aS = length();
    int i;
    for (i = 0; m_digits[i] == '0' && i < aS; ++i) {
        m_digits[i] = '9';
    }
    m_digits[i] = (m_digits[i] - '0') - 1 + '0';
    if (aS > 1 && m_digits[aS-1] == '0')
        m_digits.pop_back();


    return *this;
}
BigInt BigInt::operator++(int temp)
{
    BigInt aux {*this};
    ++(*this);

    return aux;
}
BigInt BigInt::operator--(int temp)
{
    BigInt aux {*this};
    --(*this);

    return aux;
}

BigInt& operator+=(BigInt& a, const BigInt& b)
{
    std::size_t aS = a.length(), bS = b.length();
    if (bS > aS) {  // modified BigInt may be of larger length
        a.m_digits.append(bS - aS, '0');
        aS = a.length();
    }

    unsigned short carryOver {};
    for (int i {0}, sum; i < aS; ++i) {
        if (i < bS) {
            sum = (a.m_digits[i] - '0') + (b.m_digits[i] - '0') + carryOver;
        } else {  // const BigInt may be of smaller length
            sum = (a.m_digits[i] - '0') + carryOver;
        }
        carryOver = sum / 10;
        a.m_digits[i] = (sum % 10) + '0';
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
BigInt& operator-=(BigInt& a, const BigInt& b)
{
    if (a < b) {  // BigInt currently only supports non-negative values
        throw std::invalid_argument("Attempting to subtract BigInt of greater value");
    }

    std::size_t aS = a.length(), bS = b.length();
    for (int i {0}, carryOver {0}, result; i < aS; ++i) {
        if (i < bS) {
            result = (a.m_digits[i] - '0') - (b.m_digits[i] - '0') + carryOver;
        } else {  // const BigInt may be of smaller length
            result = (a.m_digits[i] - '0') + carryOver;
        }
        if (result < 0) {
            result += 10;
            carryOver = -1;
        } else {
            carryOver = 0;
        }
        a.m_digits[i] = result + '0';
    }
    while (aS > 1 && a.m_digits[aS-1] == '0') {
        a.m_digits.pop_back();
        aS--;
    }

    return a;
}
BigInt operator-(const BigInt& a, const BigInt& b)
{
    BigInt temp {a};
    temp -= b;

    return temp;
}

BigInt& operator*=(BigInt& a, const BigInt& b)
{
    auto empty = BigInt::zero();
    if (a == empty || b == empty) {
        a = empty;
        return a;
    }

    std::size_t aS = a.length(), bS = b.length();
    std::vector<unsigned long> cache(aS + bS, 0);
    for (int i {0}; i < aS; ++i) {
        for (int j {0}; j < bS; ++j) {
            cache[i+j] += (a.m_digits[i] - '0') * (b.m_digits[j] - '0');
        }
    }

    aS += bS;
    a.m_digits.resize(cache.size());
    for (int i {0}, carryOver {0}, result; i < aS; ++i) {
        result = carryOver + cache[i];
        cache[i] = result % 10;
        carryOver = result / 10;
        a.m_digits[i] = cache[i] + '0';
    }

    for (auto i = aS - 1; i >= 1 && !cache[i]; --i) {
        a.m_digits.pop_back();
    }

    return a;
}
BigInt operator*(const BigInt& a, const BigInt& b)
{
    BigInt temp {a};
    temp *= b;

    return temp;
}
BigInt& operator/=(BigInt& a, const BigInt& b)
{
    const auto zero = BigInt::zero();
    if (b == zero) {
        throw std::runtime_error("Division by zero");
    }
    if (a < b) {
        a = zero;
        return a;
    }
    const auto one = BigInt::one();
    if (a == b) {
        a = one;
        return a;
    }

    std::size_t aS = a.length(), bS = b.length();
    BigInt carryOver = zero;
    const auto ten = BigInt::ten();
    int i;
    for (i = aS - 1; ; --i) {
        auto ch = BigInt {static_cast<unsigned long long>(a.m_digits[i] - '0')};
        if (carryOver * ten + ch >= b)
            break;
        carryOver *= ten;
        carryOver += ch;
    }

    std::vector<unsigned long> cache(aS, 0);
    unsigned long long cc, lgcat {};
    for (; i >= 0; --i) {
        auto ch = BigInt {static_cast<unsigned long long>(a.m_digits[i] - '0')};
        carryOver = carryOver * ten + ch;
        for (cc = 9uLL; BigInt {cc} * b > carryOver; --cc);
        carryOver -= BigInt {cc} * b;
        cache[lgcat++] = cc;
    }

    a.m_digits.resize(cache.size());
    for (i = 0; i < lgcat; ++i) {
        a.m_digits[i] = cache[lgcat-i-1] + '0';
    }
    a.m_digits.resize(lgcat);

    return a;
}
BigInt operator/(const BigInt& a, const BigInt& b)
{
    BigInt temp {a};
    temp /= b;

    return temp;
}
BigInt& operator%=(BigInt& a, const BigInt& b)
{
    const auto zero = BigInt::zero();
    if (b == zero) {
        throw std::runtime_error("Division by zero");
    }
    const auto one = BigInt::one();
    if (a == b || b == one) {
        a = zero;
        return a;
    }
    if (a < b) {
        return a;
    }

    std::size_t aS = a.length(), bS = b.length();
    BigInt carryOver = zero;
    const auto ten = BigInt::ten();
    int i;
    for (i = aS - 1; ; --i) {
        auto ch = BigInt {static_cast<unsigned long long>(a.m_digits[i] - '0')};
        if (carryOver * ten + ch >= b)
            break;
        carryOver *= ten;
        carryOver += ch;
    }

    std::vector<unsigned long> cache(aS, 0);
    unsigned long long cc, lgcat {};
    for (; i >= 0; --i) {
        auto ch = BigInt {static_cast<unsigned long long>(a.m_digits[i] - '0')};
        carryOver = carryOver * ten + ch;
        for (cc = 9uLL; BigInt {cc} * b > carryOver; --cc);
        carryOver -= BigInt {cc} * b;
        cache[lgcat++] = cc;
    }

    a = carryOver;
    return a;
}
BigInt operator%(const BigInt& a, const BigInt& b)
{
    BigInt temp {a};
    temp %= b;

    return temp;
}

TEST_SUITE("test BigInt") {
    TEST_CASE("consructors and toString()") {
        std::string number {"1234567890"};
        unsigned long long num {1234567890};

        const BigInt a {number};
        const BigInt b {"1234567890"};
        const BigInt c {num};
        const BigInt d {b};

        CHECK_EQ(number, a.toString());
        CHECK_EQ(number, b.toString());
        CHECK_EQ(number, c.toString());
        CHECK_EQ(number, d.toString());
    }

    TEST_CASE("equality comparison") {
        std::string number {"1234567890"};
        unsigned long long num {1234567890};

        const BigInt a {number};
        const BigInt b {num};
        const BigInt c {a};
        const BigInt d {"1234"};

        CHECK_EQ(a, b);
        CHECK_EQ(a, c);
        CHECK_EQ(b, c);

        CHECK_NE(a, d);
        CHECK_NE(b, d);
        CHECK_NE(c, d);
    }

    TEST_CASE("comparison operators") {
        const BigInt a {"10"};
        const BigInt b {"11"};
        const BigInt c {"12"};
        const BigInt d {11uLL};

        CHECK_LT(a, b);
        CHECK_LT(a, c);

        CHECK_GT(b, a);
        CHECK_GT(c, a);

        CHECK_LE(a, b);
        CHECK_LE(b, d);

        CHECK_GE(c, b);
        CHECK_GE(d, b);
    }

    TEST_CASE("increment operators") {
        std::string number {"9999999"};
        BigInt a {number};
        a++;
        const std::string expected {"10000000"};

        CHECK_EQ(expected, a.toString());
        CHECK_EQ(number, (--a).toString());
    }

    TEST_CASE("decrement operators") {
        std::string number {"10000000"};
        BigInt a {number};
        a--;
        const std::string expected {"9999999"};

        CHECK_EQ(expected, a.toString());
        CHECK_EQ(number, (++a).toString());
    }

    TEST_CASE("invalid decrement") {
        BigInt a = BigInt::zero();

        CHECK_THROWS_AS(--a, std::runtime_error);
    }

    TEST_CASE("addition when same length") {
        const BigInt a {"1234567890"};
        const BigInt b {"1234567890"};
        const std::string expected {"2469135780"};

        CHECK_EQ(expected, (a + b).toString());
    }

    TEST_CASE("addition when first number longer") {
        const BigInt a {"1234567890"};
        const BigInt b {"1234"};
        const std::string expected {"1234569124"};

        CHECK_EQ(expected, (a + b).toString());
    }

    TEST_CASE("addition when first number shorter") {
        const BigInt a {"1234"};
        const BigInt b {"1234567890"};
        const std::string expected {"1234569124"};

        CHECK_EQ(expected, (a + b).toString());
    }

    TEST_CASE("addition assignment") {
        BigInt a {"1234567890"};
        const BigInt b {"1234567890"};
        a += b;
        const std::string expected {"2469135780"};

        CHECK_EQ(expected, a.toString());
    }

    TEST_CASE("subtraction when same length") {
        const BigInt a {"1234567890"};
        const BigInt b {"1111111111"};
        const std::string expected {"123456779"};

        CHECK_EQ(expected, (a - b).toString());
    }

    TEST_CASE("subtraction when first number longer") {
        const BigInt a {"1234567890"};
        const BigInt b {"1234"};
        const std::string expected {"1234566656"};

        CHECK_EQ(expected, (a - b).toString());
    }

    TEST_CASE("subtraction when first number shorter") {
        const BigInt a {"1234"};
        const BigInt b {"1234567890"};

        CHECK_THROWS_AS(a - b, std::invalid_argument);
    }

    TEST_CASE("subtraction assignment") {
        BigInt a {"5000"};
        const BigInt b {"999"};
        a -= b;
        const std::string expected {"4001"};

        CHECK_EQ(expected, a.toString());
    }

    TEST_CASE("multiplication when a = 0") {
        const BigInt a = BigInt::zero();
        const BigInt b {"100"};

        CHECK_EQ(a, a * b);
    }

    TEST_CASE("multiplication") {
        const BigInt a {"1000"};
        const BigInt b {"1234"};
        const std::string expected {"1234000"};

        CHECK_EQ(expected, (a * b).toString());
    }

    TEST_CASE("multiplication assignment") {
        BigInt a {"5000"};
        const BigInt b {"999"};
        a *= b;
        const std::string expected {"4995000"};

        CHECK_EQ(expected, a.toString());
    }

    TEST_CASE("division when b = 0") {
        const BigInt a {"123456"};
        const BigInt b = BigInt::zero();

        CHECK_THROWS_AS(a / b, std::runtime_error);
    }

    TEST_CASE("division when a < b") {
        const BigInt a {"1000"};
        const BigInt b {"1234"};

        CHECK_EQ(BigInt::zero(), a / b);
    }

    TEST_CASE("division when a = b") {
        const BigInt a {"1234"};
        const BigInt b {"1234"};

        CHECK_EQ(BigInt::one(), a / b);
    }

    TEST_CASE("division assignment when evenly divisible") {
        BigInt a {"8888"};
        const BigInt b {"2"};
        a /= b;
        const std::string expected {"4444"};

        CHECK_EQ(expected, a.toString());
    }

    TEST_CASE("division assignment when not evenly divisible") {
        BigInt a {"8888"};
        const BigInt b {"3"};
        a /= b;
        const std::string expected {"2962"};

        CHECK_EQ(expected, a.toString());
    }

    TEST_CASE("modulo when b = 0") {
        const BigInt a {"123456"};
        const BigInt b = BigInt::zero();

        CHECK_THROWS_AS(a % b, std::runtime_error);
    }

    TEST_CASE("modulo when a = b or b = 1") {
        const BigInt a {"1234"};
        const BigInt b {"1234"};
        const BigInt c = BigInt::one();

        CHECK_EQ(BigInt::zero(), a % b);
        CHECK_EQ(BigInt::zero(), a % c);
    }

    TEST_CASE("modulo when a < b") {
        const BigInt a {"1000"};
        const BigInt b {"1234"};

        CHECK_EQ(a, a % b);
    }

    TEST_CASE("modulo assignment when evenly divisible") {
        BigInt a {"25"};
        const BigInt b {"5"};
        a %= b;

        CHECK_EQ("0", a.toString());
    }

    TEST_CASE("modulo assignment when not evenly divisible") {
        BigInt a {"12345678"};
        const BigInt b {"13"};
        a %= b;
        const std::string expected {"7"};

        CHECK_EQ(expected, a.toString());
    }
}