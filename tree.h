#ifndef BINTREE_TREE_H
#define BINTREE_TREE_H

#include <functional>
#include <iostream>
#include <list>

using std::list;
using std::cout;
using std::cin;
using std::endl;

class treenode_t {
public:
    treenode_t *left        = nullptr;
    treenode_t *right       = nullptr;
    treenode_t *parent      = nullptr;

    int value = 0;

public:
    treenode_t() {}
    treenode_t(int value) : value(value) {}

    void hangleft(treenode_t *newparent);
    void hangright(treenode_t *newparent);

    void addleft(treenode_t *child);
    void addleft(int val);
    void addright(treenode_t *child);
    void addright(int val);
    int get_degree();
};

using iter_fun_t = std::function<void (treenode_t *, int)>;
static iter_fun_t print_action = [] (treenode_t *node, int level = 0) {
    cout << node->value << ", ";
};

class bintree_t {
public:
    treenode_t *root = nullptr;

public:
    bintree_t(bool init = false);
    ~bintree_t();


/* 层序遍历 */
    void iter_bfs(iter_fun_t action = ::print_action);

/* 前序遍历 */
    void iter_pre(iter_fun_t action = ::print_action);

/* 中序遍历 */
    void iter_in(iter_fun_t action = ::print_action);

/* 后序遍历 */
    void iter_post(iter_fun_t action = ::print_action);

/* 打印树 */
    void print_tree();

};

class search_tree_t : public bintree_t {
public:
    search_tree_t(bool init = false);

    treenode_t *search_value(int val);
    void push(int val);
    void remove(int val);
    void erase(treenode_t *node);
};

#endif
