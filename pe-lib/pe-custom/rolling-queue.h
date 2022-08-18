#ifndef PROJECT_EULER_CPP_ROLLING_QUEUE_H
#define PROJECT_EULER_CPP_ROLLING_QUEUE_H

#include <queue>
#include <sstream>

/**
 * A classic circular buffer implementation of a queue for FIFO management, which uses
 * a bounded-blocking queue backed by a fixed-sized std::queue Container.
 *
 * This is a specific-use implementation and would need to consider the use of
 * conditional variables and std::mutex to be truly blocking.
 */
template <typename T>
class RollingQueue {
public:
    explicit RollingQueue(std::size_t capacity) : m_capacity {capacity} {};

    /**
     * Inserts element at the back of this queue without exceeding m_capacity, by
     * auto-removing the front if full. The action will never block or throw if queue
     * is full.
     */
    void push(const T& value)
    {
        if (size() == m_capacity) {
            m_data.pop();
        }
        m_data.push(value);
    }

    T front() { return m_data.front(); }

    void reversed()
    {
        if (m_data.empty())
            return;

        T value = m_data.front();
        m_data.pop();

        reversed();
        // no need to use override version
        m_data.push(value);
    }

    // Pointless to make constant as class objects most likely never const?
    std::size_t size() const { return m_data.size(); }

    std::string toString() const
    {
        std::stringstream output;

        std::queue<T> copy = m_data;
        while (!copy.empty()) {
            output << copy.front();
            copy.pop();
        }

        return output.str();
    }

private:
    std::queue<T> m_data;
    const std::size_t m_capacity;
};

#endif //PROJECT_EULER_CPP_ROLLING_QUEUE_H