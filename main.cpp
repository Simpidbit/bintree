#include <iostream>
#include "tree.hpp"

/*
               1
        2               3
    4       5       6       7

*/

using std::cout;
using std::endl;
using std::cin;

int main()
{
    bintree_t<int> tree;

    auto print_action = [] (treenode_t<int> *node, uint_t level, left_or_right_e) {
        cout << node->value << ", ";
    };

    tree.root = new decltype(tree)::node_type(10);
    tree.root->addleft(2);
    tree.root->addright(30);
    tree.print_tree();

    tree.trav_bfs(print_action);
    cout << endl;
    tree.trav_pre(print_action);
    cout << endl;
    tree.trav_in(print_action);
    cout << endl;
    tree.trav_post(print_action);
    cout << endl;

    tree.rotate_right(tree.root);
    tree.print_tree();
    tree.rotate_left(tree.root);
    tree.print_tree();
    tree.rotate_left(tree.root);
    tree.print_tree();


    return 0;
}
