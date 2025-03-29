#include <iostream>

#define DEBUG

#include "tree.hpp"

using std::cout;
using std::endl;
using std::cin;

int main()
{
    bintree_t<int> tree;

    auto print_action = [] (treenode_t<int> *node, uint_t level, left_or_right_e) {
        cout << node->value << ", ";
    };

#define NEWNODE(val) \
    treenode_t<int> *node##val = new treenode_t<int>(val);

    NEWNODE(1)
    NEWNODE(2)
    NEWNODE(3)
    NEWNODE(4)
    NEWNODE(5)
    NEWNODE(6)
    NEWNODE(7)
    NEWNODE(8)

    return 0;
}
