/*
 * Problem 19: Counting Sundays
 *
 * https://projecteuler.net/problem=19
 *
 * Goal: Find the number of Sundays that fell on the 1st day of the month between 2
 * dates YYYY MM DD inclusive.
 *
 * Constraints: 1900 <= Y1 <= 1e16, Y1 <= Y2 <= Y1 + 1000
 *              1 <= M1, M2 <= 12
 *              1 <= D1, D2 <= 31
 *
 * e.g.: Y1 M1 D1 = 1900 1 1, Y2 M2 D2 = 1910 1 1
 *       num of Sundays on the 1st = 18
 */

#include <tuple>

#include "../../doctest/doctest.h"

#include "pe-strings/utility.h"

inline bool isLeapYear(unsigned long long year)
{
    return year % 4 == 0uLL && (year % 100 != 0uLL || year % 400 == 0uLL);
}

/*
 * Brute search finds weekday on January 1st of year, based on the fact that Jan 1st,
 * 1900 was a Monday.
 *
 * @return integer from 0 to 6 with 0 = Sunday.
 */
unsigned short getJanFirstOfYear(unsigned long long year)
{
    unsigned long long start {1900uLL};
    unsigned short day {1};

    while (start < year) {
        day = isLeapYear(start) ? (day + 2) % 7 : (day + 1) % 7;
        start++;
    }

    return day;
}

/*
 * This solution will not tolerate years > 1e6 well ????
 */
unsigned long countSundayFirsts(unsigned long long startY, unsigned short startM,
                                unsigned short startD, unsigned long long endY,
                                unsigned short endM, unsigned short endD)
{
    const unsigned short daysInMonth[] {31, 28, 31, 30, 31, 30,
                                        31, 31, 30, 31, 30, 31};
    // adjust starting month & year
    unsigned long long currentYear {startY};
    unsigned short currentMonth {startM};
    if (startD > 1) {
        currentMonth = startM % 12 + 1;
        if (currentMonth == 1)
            currentYear++;
    }

    unsigned long sundays {};
    // get weekday that corresponds to Jan 1st of starting year
    auto janFirst = getJanFirstOfYear(currentYear);
    // use above weekday to find first Sunday in January that year
    unsigned short day = janFirst == 0 ? 1 : 8 - janFirst;
    if (currentYear == endY && currentMonth > endM)
        return 0;
    if (day == 1)
        sundays++;
    while (currentYear <= endY) {
        // jump forward a week as only interested in checking every Sunday
        day += 7;
        unsigned short monthDays;
        if (currentMonth == 2 && isLeapYear(currentYear))
            monthDays = 29;
        else
            monthDays = daysInMonth[currentMonth - 1];
        if (day > monthDays) {
            day -= monthDays;
            currentMonth++;
        }
        if (currentYear == endY && currentMonth == endM && day > endD)
            break;
        if (day == 1)
            sundays++;
        if (currentMonth > 12) {
            currentYear++;
            currentMonth = 1;
        }
    }

    return sundays;
}

/*
 * Find the weekday for a date using Zeller's Congruence algorithm.
 *
 * Zeller's Congruence algorithm is based on the formula:
 *
 *      h = (day + 13(month + 1)/5 + K + K/4 + J/4 + 5J) % 7
 *
 * with month & year being adjusted to have January and February as the 13th & 14th
 * months of the preceding year, and (K, J) = (year % 100, year / 100). Note that this
 * only applies to the Gregorian calendar.
 *
 * @return integer from 0 to 6 with 0 = Saturday, 1 = Sunday, ..., 6 = Friday.
 */
inline unsigned short getWeekday(unsigned short day, unsigned short month,
                                 unsigned long long year)
{
    if (month < 3) {
        month += 12;
        year--;
    }
    auto k = year % 100, j = year / 100;

    return (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;

}

/*
 * Solution using Zeller's Congruence algorithm helper function.
 */
unsigned long countSundayFirstsZeller(unsigned long long startY, unsigned short startM,
                                      unsigned short startD, unsigned long long endY,
                                      unsigned short endM)
{
    // adjust starting month forward
    // as only the weekday on the first of the month matters
    unsigned long long currentYear {startY};
    unsigned short currentMonth {startM};
    if (startD > 1) {
        currentMonth = startM % 12 + 1;
        // adjust starting year forward if date has rolled over
        // e.g. Dec 2, 2020 would become Jan 1, 2021
        if (currentMonth == 1)
            currentYear++;
    }

    unsigned long sundays {};
    // end loop when end month & year exceeded, as end day is not relevant
    while (currentYear <= endY) {
        if (currentYear == endY && currentMonth > endM)
            break;
        // check if first of month is Sunday
        auto weekday = getWeekday(1, currentMonth, currentYear);
        if (weekday == 1)
            sundays++;
        // move forward to next month
        currentMonth = currentMonth % 12 + 1;
        if (currentMonth == 1)
            currentYear++;
    }

    return sundays;
}


TEST_CASE("test isLeapYear() with leap years") {
    unsigned long long years[] {2016, 2020, 2000, 1980, 2396, 1944};

    for (const auto& year: years) {
        CHECK(isLeapYear(year));
    }
}

TEST_CASE("test isLeapYear() with non-leap years") {
    unsigned long long years[] {2100, 2200, 1900, 1986, 2379};

    for (const auto& year: years) {
        CHECK_FALSE(isLeapYear(year));
    }
}

TEST_CASE("test getJanFirstOfYear()") {
    unsigned long long years[] {1900, 1901, 1920, 1986, 2000, 2020};
    unsigned short expected[] {1, 2, 4, 3, 6, 3};

    for (const auto& year: years) {
        auto i = &year - &years[0];
        CHECK_EQ(expected[i], getJanFirstOfYear(year));
    }
}

TEST_CASE("test getWeekday()") {
    unsigned short days[] {1, 17, 24, 25};
    unsigned short months[] {1, 10, 8, 12};
    unsigned long long years[] {1900, 2021, 2000, 1982};
    unsigned short expected[] {2, 1, 5, 0};

    for (const auto& day: days) {
        auto i = &day - &days[0];
        CHECK_EQ(expected[i], getWeekday(day, months[i], years[i]));
    }
}

TEST_CASE("test countSundayFirsts() with lower constraints") {
    std::tuple<std::string, std::string, unsigned long> parameters[] {
        // start date exceeds end date
        {"1925 6 16", "1924 6 6", 0},
        // start date == end date
        {"1905 1 1", "1905 1 1", 1}, {"2022 2 10", "2022 2 10", 0},
        // one day between dates
        {"1999 12 31", "2000 1 1", 0}, {"2022 12 31", "2023 1 1", 1},
        // months between dates in same year
        {"2020 5 10", "2020 11 29", 1},
        // months between dates over different years
        {"1988 3 25", "1989 7 13", 2},
        // years between dates
        {"1924 6 6", "1925 6 16", 2}, {"1995 12 6", "1998 4 2", 5},
        // decade between dates
        {"1900 1 1", "1910 1 1", 18}, {"2000 1 1", "2020 1 1", 35},
        // century between dates
        {"1901 1 1", "2000 12 31", 171},
        // adjusted start date exceeds end date
        {"1900 1 4", "1900 1 5", 0}, {"1999 12 20", "1999 12 31", 0},
        // future cases
        {"4699 12 12", "4710 1 1", 18}
    };

    for (const auto& [start, end, expected]: parameters) {
        auto startDates = split(start);
        auto endDates = split(end);
        CHECK_EQ(expected, countSundayFirsts(std::stoull(startDates[0]),
                                             std::stoi(startDates[1]),
                                             std::stoi(startDates[2]),
                                             std::stoull(endDates[0]),
                                             std::stoi(endDates[1]),
                                             std::stoi(endDates[2])));
        CHECK_EQ(expected,countSundayFirstsZeller(std::stoull(startDates[0]),
                                                  std::stoi(startDates[1]),
                                                  std::stoi(startDates[2]),
                                                  std::stoull(endDates[0]),
                                                  std::stoi(endDates[1])));
    }
}

TEST_CASE("test countSundayFirsts() with mid constraints") {
    unsigned long long startY {static_cast<unsigned long long>(1e6)};
    auto endY = startY + 1000uLL;
    unsigned short month = 1, day = 1;
    unsigned long expected {1720};

    CHECK_EQ(expected, countSundayFirsts(startY, month, day, endY, month, day));
    CHECK_EQ(expected, countSundayFirstsZeller(startY, month, day, endY, month));
}

TEST_CASE("test countSundayFirsts() with upper constraints") {
    unsigned long long startY {static_cast<unsigned long long>(1e12)};
    auto endY = startY + 1000uLL;
    unsigned short startM = 2, day = 2;
    unsigned short endM = startM + 1;
    unsigned long expected {1720};

    CHECK_EQ(expected, countSundayFirstsZeller(startY, startM, day, endY, endM));
}