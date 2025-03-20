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

    cout << "层序遍历:" << endl;
    tree->trav_bfs();

    cout << "前序遍历:" << endl;
    tree->trav_pre();

    cout << "中序遍历:" << endl;
    tree->trav_in();

    cout << "后序遍历:" << endl;
    tree->trav_post();

    delete tree;

    cout << "二叉搜索树 ===================" << endl;

    search_tree_t<int> *search_tree = new search_tree_t<int>(true);

    cout << "层序遍历:" << endl;
    search_tree->trav_bfs();

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

    search_tree->remove(19);
    search_tree->remove(18);
    search_tree->remove(20);
    search_tree->print_tree();
    search_tree->remove(1);
    search_tree->remove(2);
    search_tree->remove(4);
    search_tree->print_tree();

    return 0;
}
