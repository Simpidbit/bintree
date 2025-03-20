#include "tree.h"

void
treenode_t::hangleft(treenode_t *newparent) 
{
    newparent->left = this;
    this->parent = newparent;
}

void
treenode_t::hangright(treenode_t *newparent) {
    newparent->right = this;
    this->parent = newparent;
}


void
treenode_t::addleft(treenode_t *child) {
    this->left = child;
    child->parent = this;
}
void 
treenode_t::addleft(int val) { this->addleft(new treenode_t(val)); }

void
treenode_t::addright(treenode_t *child) {
    this->right = child;
    child->parent = this;
}
void 
treenode_t::addright(int val) { this->addright(new treenode_t(val)); }

int
treenode_t::get_degree()
{
    bintree_t *tree = new bintree_t;
    tree->root = this;

    int degree_max = 0;

    tree->iter_pre([&](treenode_t *node, int level) {
        if (level > degree_max) degree_max = level;
    });
    free(tree);

    return degree_max;
}
