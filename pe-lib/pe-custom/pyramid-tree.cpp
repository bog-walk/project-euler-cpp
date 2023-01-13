#include "pyramid-tree.h"

#include <iostream>

#include "../../doctest/doctest.h"

TEST_SUITE("test PyramidTree") {
    TEST_CASE("with only a root element") {
        unsigned short elements[] {10};
        // could just use std::size(container)?
        auto len = sizeof(elements) / sizeof(elements[0]);
        PyramidTree tree {1, len, elements};

        std::cout << tree.toString() << std::endl;

        CHECK_EQ(elements[0], tree.root->value);
        CHECK((tree.root->leftAdjacent == nullptr));
        CHECK((tree.root->rightAdjacent == nullptr));
        CHECK_EQ(elements[0], tree.maxSumPostOrderTraversal(tree.root));
    }

    TEST_CASE("with only 2 levels") {
        unsigned short elements[] {1, 2, 3};
        auto len = sizeof(elements) / sizeof(elements[0]);
        PyramidTree tree {2, len, elements};

        std::cout << tree.toString() << std::endl;

        CHECK_EQ(2, tree.root->leftAdjacent->value);
        CHECK_EQ(3, tree.root->rightAdjacent->value);
        CHECK_EQ(4, tree.maxSumPostOrderTraversal(tree.root));
    }

    TEST_CASE("with 3 levels") {
        unsigned short elements[] {1, 2, 3, 4, 5, 6};
        auto len = sizeof(elements) / sizeof(elements[0]);
        PyramidTree tree {3, len, elements};

        std::cout << tree.toString() << std::endl;

        CHECK_EQ(10, tree.maxSumPostOrderTraversal(tree.root));
    }

    TEST_CASE("finds sole route through max-sized tree") {
        std::size_t len {120};
        unsigned short elements[len];
        for (int i {0}; i < len; ++i) {
            elements[i] = i == 100 ? 99 : 0;
        }
        PyramidTree tree {15, len, elements};

        CHECK_EQ(99, tree.maxSumPostOrderTraversal(tree.root));
    }

    TEST_CASE("finds max route through max-sized tree") {
        std::size_t len {120};
        unsigned short elements[len];
        for (int i {0}; i < len; ++i) {
            elements[i] = i == 108 ? 11 : (i == 116 ? 99 : 1);
        }
        PyramidTree tree {15, len, elements};

        CHECK_EQ(113, tree.maxSumPostOrderTraversal(tree.root));
    }
}