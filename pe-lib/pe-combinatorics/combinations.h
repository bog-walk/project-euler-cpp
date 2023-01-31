#ifndef PROJECT_EULER_CPP_COMBINATIONS_H
#define PROJECT_EULER_CPP_COMBINATIONS_H

#include <algorithm>
#include <numeric>
#include <string>

/*
 * Mimics the Python itertools module function and returns r-length subLists of elements.
 *
 * If elements is sorted, combinations will be yielded in lexicographic order.
 *
 * If elements contains only unique values, there will be no repeat values in each
 * combination.
 *
 * e.g. elements = "ABCD", r = 2 -> AB AC AD BC BD CD
 *
 * The number of yielded combinations, if n = the amount of elements, is:
 *
 * n!/r!/(n - r)!
 */
template <typename T>
class Combinations {
public:
    explicit Combinations(int r, const std::string& elements) :
            m_input(elements.cbegin(), elements.cend()),
            m_index {static_cast<int>(elements.length() - r)},
            m_length {r} {
        m_indices.resize(r);
        std::iota(m_indices.begin(), m_indices.end(), 0);
    }
    explicit Combinations(int r, std::vector<T>& elements) :
            m_input(elements.cbegin(), elements.cend()),
            m_index {static_cast<int>(elements.size() - r)},
            m_length {r} {
        m_indices.resize(r);
        std::iota(m_indices.begin(), m_indices.end(), 0);
    }

    // Don't support copy and move operations
    Combinations(const Combinations&) = delete;
    Combinations(Combinations&&) = delete;
    Combinations& operator=(const Combinations&) = delete;
    Combinations& operator=(Combinations&&) = delete;

    bool hasNext()
    {
        if (m_firstYield) {
            m_firstYield = false;
            // don't allow combo generation if r == 0 || r > elements.size();
            return m_length && m_index >= 0;
        }

        auto i = m_length - 1;
        while (i >= 0) {
            if (m_indices[i] != i + m_index)
                break;
            if (i-- == 0)
                return false;
        }
        m_indices[i]++;
        for (int j = i + 1; j < m_length; ++j) {
            m_indices[j] = m_indices[j-1] + 1;
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

    std::vector<std::vector<T>> toList()
    {
        std::vector<std::vector<T>> output;

        while (hasNext()) {
            output.push_back(next());
        }

        return output;
    }

private:
    const std::vector<T> m_input;
    const int m_index, m_length;
    std::vector<int> m_indices;
    bool m_firstYield {true};
};

#endif //PROJECT_EULER_CPP_COMBINATIONS_H