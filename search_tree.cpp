#include "tree.h"

/*
                      8
         4                        12
    2         6            10            14
  1   3     5   7        9    11      13    15 

*/
search_tree_t::search_tree_t(bool init)
{
    if (init) {
        this->root = new treenode_t(8);

        this->root->addleft(4);
        this->root->addright(12);

        this->root->left->addleft(2);
        this->root->left->addright(6);
        this->root->right->addleft(10);
        this->root->right->addright(14);

        this->root->left->left->addleft(1);
        this->root->left->left->addright(3);
        this->root->left->right->addleft(5);
        this->root->left->right->addright(7);
        this->root->right->left->addleft(9);
        this->root->right->left->addright(11);
        this->root->right->right->addleft(13);
        this->root->right->right->addright(15);
    }
}

treenode_t *
search_tree_t::search_value(int val)
{
    treenode_t *cur = this->root;

    for (;;) {
        if (cur == nullptr)
            return cur;

        if (cur->value == val) 
            return cur;
        else {
            if (cur->value < val)
                cur = cur->right;
            else 
                cur = cur->left;
        }
    }
}

void
search_tree_t::push(int val)
{
    treenode_t *cur = this->root;

    for (;;) {
        if (cur->value == val) return;
        if (val > cur->value) {
            if (!cur->right) {
                cur->addright(val);
                return;
            }
            cur = cur->right;
        } else {
            if (!cur->left) {
                cur->addleft(val);
                return;
            }
            cur = cur->left;
        }
    }
}

void
search_tree_t::remove(int val)
{
    treenode_t *node = this->search_value(val);
    if (node)
        this->erase(node);
}

void
search_tree_t::erase(treenode_t *node)
{
    // 先确定此节点的度
    int degree = node->get_degree();
    if (degree == 0) {
        // 直接删
        if (!node->parent) // 根节点
            ;
        else if (node->parent->left == node)
            node->parent->left = nullptr;
        else 
            node->parent->right = nullptr;
        delete node;
    } else if (degree == 1) {
        // 让子节点上来
        if (node->left) {
            node->value = node->left->value;
            delete node->left;
            node->left = nullptr;
        } else {
            node->value = node->right->value;
            delete node->right;
            node->right = nullptr;
        }
    } else {
        // 左子树的最大节点或者右子树的最小节点
        if (node->left) {
            // 左子树的最大节点
            treenode_t *left_max = node->left;
            for (;;) {
                if (left_max->right)
                    left_max = left_max->right;
                else break;
            }
            node->value = left_max->value;
            if (left_max->parent->left == left_max)
                left_max->parent->left = nullptr;
            else
                left_max->parent->right = nullptr;
            delete left_max;
        } else {
            // 右子树的最小节点
            treenode_t *right_min = node->right;
            for (;;) {
                if (right_min->left)
                    right_min = right_min->left;
                else break;
            }
            node->value = right_min->value;
            if (right_min->parent->left == right_min)
                right_min->parent->left = nullptr;
            else
                right_min->parent->right = nullptr;
            delete right_min;
        }
    }
}
