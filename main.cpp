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
    bintree_t<int> *tree = new bintree_t<int>(true);

    auto print_action = [] (treenode_t<int> *node, uint_t level, left_or_right_e) {
        cout << node->value << ", ";
    };

    cout << "层序遍历:" << endl;
    tree->trav_bfs(print_action);
    cout << endl;

    cout << "前序遍历:" << endl;
    tree->trav_pre(print_action);
    cout << endl;

    cout << "中序遍历:" << endl;
    tree->trav_in(print_action);
    cout << endl;

    cout << "后序遍历:" << endl;
    tree->trav_post(print_action);
    cout << endl;

    delete tree;

    cout << "二叉搜索树 ===================" << endl;

    search_tree_t<int> *search_tree = new search_tree_t<int>(true);

    cout << "层序遍历:" << endl;
    search_tree->trav_bfs(print_action);
    cout << endl;

    treenode_t<int> *searched_node = search_tree->search_value(88);
    cout << "搜索元素: "
         << searched_node << endl;
    searched_node = search_tree->search_value(3);
    cout << "搜索元素: "
         << searched_node << " = " << searched_node->value << endl;

    cout << "添加元素后打印: " << endl;
    search_tree->push(19);
    search_tree->push(18);
    search_tree->push(20);
    search_tree->print_tree();

    auto ptr = search_tree->search_value(15);
    if (ptr)
        cout << "ptr bf: " << ptr->get_balance_factor() << endl;

    cout << "右旋测试 ==========" << endl;
    cout << "输入右旋节点: " << endl;
    int target = 0;
    cin >> target;
    search_tree->rotate_right(search_tree->search_value(target));
    search_tree->print_tree();

    return 0;
}
