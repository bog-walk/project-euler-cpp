#ifndef PROJECT_EULER_CPP_PYRAMID_TREE_H
#define PROJECT_EULER_CPP_PYRAMID_TREE_H

#include <string>
#include <vector>

/*
 * A Node for the following PyramidTree class.
 *
 * A Node stores an integer value, as well as a reference to its 2 children, if it is not
 * a leaf node. Its sole method, draw(), allows all Nodes in a PyramidTree to be
 * drawn recursively.
 *
 * This could have been replaced by a structure for default public member access, with an
 * instance provided as an argument to an external draw().
 */
class PyramidNode {
public:
    unsigned short value;
    PyramidNode* leftAdjacent;
    PyramidNode* rightAdjacent;
    explicit PyramidNode(unsigned short val) {
        value = val;
        leftAdjacent = rightAdjacent = nullptr;
    }

    /*
     * Note that shared adjacent nodes will be duplicated in this algorithm, as the
     * PyramidTree class is not a proper binary tree.
     *
     * N.B. One of the tail options should be 4 whitespaces ("    " instead of "----")
     * but there is an issue(?) with CLion buffering output so that any trailing
     * whitespace is ignored when the tree is printing. Replacing these strings
     * produces the expected output, as produced on other external systems/compilers.
     *
     * @return recursive string representation of the node and its children.
     */
    std::string draw(const std::string& prefix, bool isTail, std::string& string) const
    {
        if (rightAdjacent != nullptr)
            rightAdjacent->draw(prefix + (isTail ? "|   " : "----"), false, string);

        string += prefix + (isTail ? "└── " : "┌── ") + std::to_string(value) + "\n";

        if (leftAdjacent != nullptr)
            leftAdjacent->draw(prefix + (isTail ? "----" : "|   "), true, string);

        return string;
    }
};

/*
 * A Tree with each node having 2 children except for leaf nodes.
 *
 * A PyramidTree, more appropriately a graph, has every node that is not along the left or
 * right edges having 2 parents, unlike in a standard BinaryTree.
 *
 * i.e. Internal children are shared by 2 nodes, with nodeX.rightAdjacent ==
 * nodeY.leftAdjacent. Therefore, each level in the tree will have an amount of nodes
 * equivalent to the level number. Also unlike a BinaryTree, nodes in a PyramidTree are
 * not ordered.
 *
 * N.B. Memory Leak & Destructors -> should this class ideally have a user-defined
 * destructor to delete pointers? Attempted binary tree implementation, but only works
 * until a node is encountered that shared a previously deleted pointer (recursion
 * error). Attempted with smart pointers, but still failed. Potentially need to
 * reconsider entire implementation of PyramidTree (maybe the issue lies in the initial
 * local vector storage)?
 */
class PyramidTree {
public:
    PyramidNode* root;
    explicit PyramidTree(unsigned long levels, std::size_t len,
                         unsigned short *elements) {
        std::vector<PyramidNode*> nodes;
        nodes.reserve(len);
        for (int i {0}; i < len; ++i) {
            auto* node = new PyramidNode {elements[i]};
            nodes.push_back(node);
        }
        int index {};
        for (int level {1}; level < levels; ++level) {
            for (int j {1}; j <= level; ++j) {
                nodes[index]->leftAdjacent = nodes[index+level];
                nodes[index]->rightAdjacent = nodes[index+level+1];
                index++;
            }
        }
        root = nodes[0];
    }

    std::string toString() const
    {
        std::string string;
        return root->draw("", true, string);
    }

    /*
     * Post-order traversal visits the left subtree before the right subtree, returning
     * the maximum sum before processing the start node.
     */
    unsigned long maxSumPostOrderTraversal(PyramidNode* node)
    {
        unsigned long left {}, right {};

        if (node->leftAdjacent != nullptr)
            left = maxSumPostOrderTraversal(node->leftAdjacent);
        if (node->rightAdjacent != nullptr)
            right = maxSumPostOrderTraversal(node->rightAdjacent);

        return std::max(left, right) + node->value;
    }
};

#endif //PROJECT_EULER_CPP_PYRAMID_TREE_H