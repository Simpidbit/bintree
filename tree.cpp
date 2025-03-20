#include "tree.h"

#include <cstdlib>

#include <deque>

/*
               1
        2               3
    4       5       6       7

*/
bintree_t::bintree_t(bool init)
{
    if (init) {
        this->root = new treenode_t(1);
        this->root->addleft(new treenode_t(2));
        this->root->addright(new treenode_t(3));
        this->root->left->addleft(new treenode_t(4));
        this->root->left->addright(new treenode_t(5));
        this->root->right->addleft(new treenode_t(6));
        this->root->right->addright(new treenode_t(7));
    }
}

bintree_t::~bintree_t()
{
    list<treenode_t *> todel;
    this->iter_bfs([&](treenode_t *node, int) {
        todel.push_back(node);
    });

    for (const auto &each : todel)
        delete each;
}

void
bintree_t::iter_bfs(iter_fun_t action)
{ using std::deque;
    deque <treenode_t *> dq;

    dq.push_back(this->root);
    while (!dq.empty()) {
        treenode_t *node = dq.front();
        dq.pop_front();

        action(node, 0);

        if (node->left)
            dq.push_back(node->left);
        if (node->right) 
            dq.push_back(node->right);
    }
}

void
bintree_t::iter_pre(iter_fun_t action)
{
    std::function<void (treenode_t *, iter_fun_t, int)> recur_iter_pre = 
        [&] (treenode_t *node, iter_fun_t action, int level) -> void {
            action(node, level);

            if (node->left)
                recur_iter_pre(node->left, action, level + 1);
            if (node->right)
                recur_iter_pre(node->right, action, level + 1);
        };
    recur_iter_pre(this->root, action, 0);
    cout << endl;
}

void
bintree_t::iter_in(iter_fun_t action)
{
    std::function<void (treenode_t *, iter_fun_t, int)> recur_iter_in = 
        [&] (treenode_t *node, iter_fun_t action, int level) -> void {
            if (node->left)
                recur_iter_in(node->left, action, level + 1);

            action(node, level);

            if (node->right)
                recur_iter_in(node->right, action, level + 1);
        };
    recur_iter_in(this->root, action, 0);
    cout << endl;
}

void
bintree_t::iter_post(iter_fun_t action)
{
    std::function<void (treenode_t *, iter_fun_t, int)> recur_iter_post = 
        [&] (treenode_t *node, iter_fun_t action, int level) -> void {
            if (node->left)
                recur_iter_post(node->left, action, level + 1);
            if (node->right)
                recur_iter_post(node->right, action, level + 1);

            action(node, level);
        };
    recur_iter_post(this->root, action, 0);
    cout << endl;
}

void bintree_t::print_tree()
{
    this->iter_pre(
        [] (treenode_t *node, int level) {
            for (int i = 0; i < level; i++)
                cout << "======|";
            cout << "\b";
            treenode_t *parent = node->parent;
            if (parent) {
                if (parent->left == node)
                    cout << "[L] ";
                else
                    cout << "[R] ";
            } else
                cout << "=========== print_tree ==========\nRT ";
            cout << node->value << endl;
        }
    );
    cout << "OK." << endl;
}
