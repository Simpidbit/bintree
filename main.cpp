#include <iostream>
#include <functional>

#define DEBUG

#include "tree.hpp"

using std::cout;
using std::endl;
using std::cin;


int main()
{
    auto print_action = [] (RB_treenode_t<int> *node, uint_t level, left_or_right_e) {
        cout << node->value << ", ";
    };

    RB_tree_t<int> tree;
    //AVL_tree_t<int> tree([] (const int &a, const int &b) -> bool {return a > b;});
    //tree.set_replace_policy(RB_tree_t<int>::EQ_REPLACE);

    size_t max_count = 0;
    cin >> max_count;

    size_t oper_count = 0;
    for (;; oper_count++) {
        int oper, val;
        cout << "Insert a oper-value: ";
        cin >> oper >> val;
        cout << "oper: " << oper << ", val: " << val << "; oper_count = " << oper_count << endl;
        switch (oper) {
            case 0: 
                    tree.push(val);
                    break;
            case 1:
                    tree.remove(val);
                    break;
        }
        tree.print_tree();
        if (oper_count + 1 == max_count) break;
    }

    return 0;
}
