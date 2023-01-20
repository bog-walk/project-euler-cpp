#ifndef PROJECT_EULER_CPP_COMBINATIONS_WITH_REPL_H
#define PROJECT_EULER_CPP_COMBINATIONS_WITH_REPL_H

#include <algorithm>
#include <string>
#include <vector>

/*
 * Mimics the Python itertools module function and returns r-length subLists of elements
 * allowing individual elements to be repeated more than once.
 *
 * If elements is sorted, combinations will be yielded in lexicographic order.
 *
 * If elements contains only unique values, combinations will also be unique.
 *
 * e.g. elements = "ABCD", r = 2 -> AA AB AC AD BB BC BD CC CD DD
 *
 * The number of yielded combinations, if n = the amount of elements, is:
 *
 * (n + r - 1)!/r!/(n - 1)!
 */
template <typename T>
class CombinationsWithRepl {
public:
    explicit CombinationsWithRepl(int r, const std::string& elements) :
        m_input(elements.cbegin(), elements.cend()),
        m_lastIndex {static_cast<int>(elements.length()) - 1},
        m_length {r} { m_indices.resize(m_length, 0); }
    explicit CombinationsWithRepl(int r, std::vector<T>& elements) :
        m_input(elements.cbegin(), elements.cend()),
        m_lastIndex {static_cast<int>(elements.size()) - 1},
        m_length {r} { m_indices.resize(m_length, 0); }

    // Don't support copy and move operations
    CombinationsWithRepl(const CombinationsWithRepl&) = delete;
    CombinationsWithRepl(CombinationsWithRepl&&) = delete;
    CombinationsWithRepl& operator=(const CombinationsWithRepl&) = delete;
    CombinationsWithRepl& operator=(CombinationsWithRepl&&) = delete;

    bool hasNext()
    {
        if (m_firstYield) {
            m_firstYield = false;
            // don't allow combo generation if r == 0 || elements.size() == 0;
            return m_length && m_lastIndex != -1;
        }

        auto i = m_length - 1;
        while (i >= 0) {
            if (m_indices[i] != m_lastIndex)
                break;
            if (i-- == 0)
                return false;
        }
        auto index = m_indices[i] + 1;
        for (int j {i}; j < m_length; ++j) {
            m_indices[j] = index;
        }

        return true;
    }

    std::vector<T> next()
    {
        std::vector<T> toYield(m_length);
        int i {};
        std::generate_n(toYield.begin(), m_length, [&i, this]{
            return m_input[m_indices[i++]];
        });

        return toYield;
    }

private:
    const std::vector<T> m_input;
    const int m_lastIndex, m_length;
    std::vector<int> m_indices;
    bool m_firstYield {true};
};

#endif //PROJECT_EULER_CPP_COMBINATIONS_WITH_REPL_H
