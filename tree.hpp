#ifndef BINTREE_TREE_HPP
#define BINTREE_TREE_HPP

#include <functional>
#include <iostream>
#include <list>
#include <deque>

#include <cstdint>


/*
template <typename T>
class treenode_t {
public:
    treenode_t *left        = nullptr;
    treenode_t *right       = nullptr;
    treenode_t *parent      = nullptr;

    T value;

public:
    treenode_t() {}
    treenode_t(T value) : value(value) {}

    void hangleft(treenode_t *newparent);
    void hangright(treenode_t *newparent);

    void addleft(treenode_t *child);
    void addleft(T val);
    void addright(treenode_t *child);
    void addright(T val);
    int get_degree();
};
*/

using uint_t = uint64_t;

template <typename T>
class treenode_t {
public:
    treenode_t<T> *left     = nullptr;
    treenode_t<T> *right    = nullptr;
    treenode_t<T> *parent   = nullptr;

    T value;

public:
    treenode_t(T value) : value(value) {}

    void hookleft(treenode_t *newparent) {
        newparent->left = this;
        this->parent = newparent;
    }

    void hookright(treenode_t *newparent) {
        newparent->right = this;
        this->parent = newparent;
    }
    
    void addleft(treenode_t *child) {
        this->left = child;
        child->parent = this;
    }

    void addright(treenode_t *child) {
        this->right = child;
        child->parent = this;
    }

    void addleft(T val) {
        this->addleft(new treenode_t(val));
    }

    void addright(T val) {
        this->addright(new treenode_t(val));
    }

    uint_t get_degree() {
        uint_t degree_max = 0;

        std::function<void (treenode_t*, uint_t)> traversal =
            [&] (treenode_t *node, uint_t level) {
                if (level > degree_max) degree_max = level;
                if (node->left)
                    traversal(node->left, ++level);
                if (node->right)
                    traversal(node->right, ++level);
            };
        traversal(this, 0);

        return degree_max;
    }
};

typedef enum {
    NODE_ROOT = 0,
    NODE_LEFT,
    NODE_RIGHT
} left_or_right_e;

template <typename T>
class bintree_t {
public:
    using trav_action_t = std::function<void (treenode_t<T> *, uint_t, left_or_right_e)>;

public:
    treenode_t<T> *root = nullptr;

public:
    /*
               1
        2               3
    4       5       6       7

    */
    bintree_t(bool init = false) {
        if (init) {
            this->root = new treenode_t<T>(1);
            this->root->addleft(2);
            this->root->addright(3);
            this->root->left->addleft(4);
            this->root->left->addright(5);
            this->root->right->addleft(6);
            this->root->right->addright(7);
        }
    }

    ~bintree_t() {
        std::list<treenode_t<T> *> todel;
        this->iter_bfs([&](treenode_t<T> *node, int) {
            todel.push_back(node);
        });
    
        for (const auto &each : todel)
            delete each;
    }

    /* 层序遍历 */
    void trav_bfs(trav_action_t action) {
        using std::deque;
        deque<treenode_t<T> *> dq;

        dq.push_back(this->root);
        while (!dq.empty()) {
            treenode_t<T> *node = dq.front();
            dq.pop_front();

            action(node, 0, NODE_ROOT);

            if (node->left)
                dq.push_back(node->left);
            if (node->right)
                dq.push_back(node->right);
        }
    }

    /* 前序遍历 */
    void trav_pre(trav_action_t action) {
        std::function<void (treenode_t<T> *, trav_action_t, uint_t, left_or_right_e)> recur_trav = 
            [&] (treenode_t<T> *node, trav_action_t action, uint_t level, left_or_right_e lorr) -> void {
                action(node, level, lorr);

                if (node->left)
                    recur_trav(node->left, action, level + 1, NODE_LEFT);
                if (node->right)
                    recur_trav(node->right, action, level + 1, NODE_RIGHT);
            };
        recur_trav(this->root, action, 0, NODE_ROOT);
    }

    /* 中序遍历 */
    void trav_in(trav_action_t action) {
        std::function<void (treenode_t<T> *, trav_action_t, uint_t, left_or_right_e)> recur_trav = 
            [&] (treenode_t<T> *node, trav_action_t action, uint_t level, left_or_right_e lorr) -> void {
                if (node->left)
                    recur_trav(node->left, action, level + 1, NODE_LEFT);

                action(node, level, lorr);

                if (node->right)
                    recur_trav(node->right, action, level + 1, NODE_RIGHT);
            };
        recur_trav(this->root, action, 0, NODE_ROOT);
    }

    /* 后序遍历 */
    void trav_post(trav_action_t action) {
        std::function<void (treenode_t<T> *, trav_action_t, uint_t, left_or_right_e)> recur_trav = 
            [&] (treenode_t<T> *node, trav_action_t action, uint_t level, left_or_right_e lorr) -> void {
                if (node->left)
                    recur_trav(node->left, action, level + 1, NODE_LEFT);
                if (node->right)
                    recur_trav(node->right, action, level + 1, NODE_RIGHT);

                action(node, level, lorr);
            };
        recur_trav(this->root, action, 0, NODE_ROOT);
    }

    /* 打印树 */
    void print_tree() {
        this->trav_pre(
            [] (treenode_t<T> *node, uint_t level, left_or_right_e lorr) {
                for (int i = 0; i < level; i++)
                    std::cout << "======|";
                std::cout << "\b";
                switch (lorr) {
                case NODE_ROOT:
                    std::cout << "=========== print_tree ==========\nRT ";
                    break;
                case NODE_LEFT:
                    std::cout << "[L] ";
                    break;
                case NODE_RIGHT:
                    std::cout << "[R] ";
                    break;
                default:;
                }
                std::cout << node->value << std::endl;
            }
        );
        std::cout << "OK." << std::endl;
    }
};

template <typename T,
          typename Compare_T = std::less<T>,
          typename Equal_T = std::equal_to<T> >
class search_tree_t : public bintree_t<T> {
public:
    search_tree_t(bool init) {
        if (init) {
            this->root = new treenode_t<T>(8);

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

    treenode_t<T> *search_value(T val) {
        treenode_t<T> *cur = this->root;

        for (;;) {
            if (cur == nullptr)
                return cur;

            if (Equal_T{}(cur->value, val)) 
                return cur;
            else {
                if (Compare_T{}(cur->value, val))
                    cur = cur->right;
                else 
                    cur = cur->left;
            }
        }
    }

    void push(T val) {
        treenode_t<T> *cur = this->root;

        for (;;) {
            if (Equal_T{}(cur->value, val)) return;
            if (Compare_T{}(cur->value, val)) {
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

    void remove(T val) {
        treenode_t<T> *node = this->search_value(val);
        if (node)
            this->erase(node);
    }

    void erase(treenode_t<T> *node) {
        // 先确定此节点的度
        uint_t degree = node->get_degree();
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
                treenode_t<T> *left_max = node->left;
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
                treenode_t<T> *right_min = node->right;
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
};

#endif  // BINTREE_TREE_HPP
