#include "rolling-queue.h"

#include "../../doctest/doctest.h"

TEST_SUITE("test RollingQueue") {
    TEST_CASE("initial state of un-filled instance") {
        std::size_t size {10};
        RollingQueue<int> rq {size};
        for (int i {2}; i < size - 3; ++i) {
            rq.push(i);
        }

        CHECK_EQ(5, rq.size());
        CHECK_EQ(2, rq.front());
        CHECK_EQ("23456", rq.toString());

        // confirm call toString() did not affect underlying data
        CHECK_EQ(5, rq.size());
        CHECK_EQ(2, rq.front());
    }

    TEST_CASE("initial state of filled instance") {
        std::size_t size {5};
        RollingQueue<int> rq {size};
        for (int i {0}; i < size; ++i) {
            rq.push(i);
        }

        CHECK_EQ(size, rq.size());
        CHECK_EQ(0, rq.front());
        CHECK_EQ("01234", rq.toString());
    }

    TEST_CASE("push() beyond capacity") {
        std::size_t size {5};
        RollingQueue<int> rq {size};
        for (int i {0}; i < size + 3; ++i) {
            rq.push(i);
        }

        CHECK_EQ(size, rq.size());
        CHECK_EQ(3, rq.front());
        CHECK_EQ("34567", rq.toString());
    }

    TEST_CASE("reversed()") {
        std::size_t size {5};
        RollingQueue<int> rq {size};
        for (int i {0}; i < size; ++i) {
            rq.push(i);
        }
        rq.reversed();

        CHECK_EQ("43210", rq.toString());
    }
}