#ifndef BINTREE_TREE_HPP
#define BINTREE_TREE_HPP

#include <functional>
#include <iostream>
#include <list>
#include <deque>

#include <cstdint>



/*
// **************************************
// ************* treenode_t *************
// **************************************
template <typename T>
class treenode_t {
public:
    treenode_t *left        = nullptr;
    treenode_t *right       = nullptr;
    treenode_t *parent      = nullptr;

    T value;

public:
    treenode_t(T value);

    void hookleft(treenode_t *newparent);
    void hookright(treenode_t *newparent);

    void addleft(treenode_t *child);
    void addleft(T val);
    void addright(treenode_t *child);
    void addright(T val);
    uint_t get_height();
    uint_t get_degree();
    int get_balance_factor();
};

typedef enum {
    NODE_ROOT = 0,
    NODE_LEFT,
    NODE_RIGHT
} left_or_right_e;

// *************************************
// ************* bintree_T *************
// *************************************
template <typename T>
class bintree_t {
public:
    using trav_action_t = std::function<void (treenode_t<T> *, uint_t, left_or_right_e)>;
    using node_type = treenode_t<T>;

public:
    treenode_t<T> *root = nullptr;

public:
    bintree_t() = default;
    ~bintree_t();

    void trav_bfs(trav_action_t action);
    void trav_pre(trav_action_t action);
    void trav_in(trav_action_t action);
    void trav_post(trav_action_t action);

    void print_tree();

    void rotate_right(treenode_t<T> *node);
    void rotate_left(treenode_t<T> *node);

#ifdef DEBUG
    treenode_t<T> *search_only(T val);
#endif
};

// *****************************************
// ************* search_tree_t *************
// *****************************************
template <typename T>
class search_tree_t : public bintree_t<T> {
protected:
    std::function<bool (const T &a, const T &b)> comparer
        = [] (const T &a, const T &b) -> bool { return a < b; };
    std::function<bool (const T &a, const T &b)> equaler
        = [] (const T &a, const T &b) -> bool { return a == b; };

public:
    search_tree_t() = default;
    search_tree_t(decltype(comparer) cmp) : comparer(cmp);
    search_tree_t(decltype(comparer) cmp, decltype(equaler) eql) 
        : comparer(cmp), equaler(eql);

    treenode_t<T> *search_value(T val);
    treenode_t<T> *push(T val);
    void remove(T val);
    treenode_t<T> *erase(treenode_t<T> *node);
};

// ************************************
// ************* AVL_tree *************
// ************************************
template <typename T>
class AVL_tree : public search_tree_t<T> {
public:
    AVL_tree() = default;
    template <typename Cmp_T>
    AVL_tree(Cmp_T &&cmp) : search_tree_t<T>(std::forward<Cmp_T>(cmp));
    template <typename Cmp_T, typename Eql_T>
    AVL_tree(Cmp_T &&cmp, Eql_T &&eql) : search_tree_t<T>(std::forward<Cmp_T, Eql_T>(cmp, eql));

    treenode_t<T> *rotate(treenode_t<T> *node);
    treenode_t<T> *push(T val);
    void erase(treenode_t<T> *node);
    void remove(T val);
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
    treenode_t(treenode_t &&node) {
        this->left = node.left;
        this->right = node.right;
        this->parent = node.parent;
        this->value = node.value;

        node.left   = nullptr;
        node.right  = nullptr;
        node.parent = nullptr;
    }

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

    void addleft(T val) {
        this->addleft(new treenode_t(val));
    }

    void addright(treenode_t *child) {
        this->right = child;
        child->parent = this;
    }

    void addright(T val) {
        this->addright(new treenode_t(val));
    }

    uint_t get_height() {
        uint_t height = 0;

        std::function<void (treenode_t*, uint_t)> traversal =
            [&] (treenode_t *node, uint_t level) -> void {
                if (level > height) height = level;
                if (node->left)
                    traversal(node->left, level + 1);
                if (node->right)
                    traversal(node->right, level + 1);
            };
        traversal(this, 0);

        return height;
    }

    uint_t get_degree() {
        if (this->left && this->right)
            return 2;
        else if (this->left || this->right)
            return 1;
        else return 0;
    }

    int get_balance_factor() {
        int left_bfactor = 0;
        int right_bfactor = 0;

        if (this->left)
            left_bfactor = this->left->get_height() + 1;
        if (this->right)
            right_bfactor = this->right->get_height() + 1;

        return left_bfactor - right_bfactor;
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
    using node_type = treenode_t<T>;
    using value_type = T;

public:
    treenode_t<T> *root = nullptr;

public:
    /*
               1
        2               3
    4       5       6       7

    */
    bintree_t() = default;
    ~bintree_t() {
        std::list<treenode_t<T> *> todel;
        this->trav_bfs([&](treenode_t<T> *node, uint_t, left_or_right_e) {
            todel.push_back(node);
        });
    
        for (const auto &each : todel)
            delete each;
    }

    /* 层序遍历 */
    void trav_bfs(trav_action_t action) {
        if (!this->root) return;
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
        if (!this->root) return;
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
        if (!this->root) return;
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
        if (!this->root) return;
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
                std::cout << node->value;
                if (node->parent)
                    std::cout << " |P " << node->parent->value << std::endl;
                else
                    std::cout << " |P NUL" << std::endl;
            }
        );
        std::cout << "Print OK." << std::endl;
    }

    /*
    node:
        左节点 == null
            (不可右旋)
            返回
        左节点 != null
            (可以右旋)
            父节点 == null
                (node是根节点)
                根节点 = 左子

                tmp = 左子->右子
                左子->右子 = node
                node->左子 = tmp

            父节点 != null
                (node不是根节点)
                父->(左/右)子 = 左子

                tmp = 左子->右子
                左子->右子 = node
                node->左子 = tmp

    | node | L | L->L | L->R | R | R->L | R->R | P | YESorNO
        0    *    *      *     *    *      *     *      0
        1    0    *      *     *    *      *     *      0
        1    1
    */
    void rotate_right(treenode_t<T> *node) {
        if (!node) {
            std::cout << "不可对nullptr右旋!" << std::endl;
            return;
        }

        if (!node->left) {
            // 不可右旋
            std::cout << "不可右旋!!!" << std::endl;
            return;
        } else {
            // 可以右旋
            if (!node->parent) {
                // node 是根节点
                this->root = node->left;
                node->left->parent = nullptr;

                treenode_t<T> *tmp = node->left->right;
                node->left->right = node;
                node->parent = node->left;
                node->left = tmp;
                if (tmp)
                    tmp->parent = node;
            } else {
                // node 不是根节点
                if (node->parent->left == node) {
                    node->parent->left = node->left;
                    node->left->parent = node->parent;
                } else {
                    node->parent->right = node->left;
                    node->left->parent = node->parent;
                }

                treenode_t<T> *tmp = node->left->right;
                node->left->right = node;
                node->parent = node->left;
                node->left = tmp;
                if (tmp)
                    tmp->parent = node;
            }
        }
    }

    /*
    node:
        右节点 == null
            (不可左旋)
        右节点 != null
            (可以左旋)
            父节点 == null
                (node是根节点)
                根节点 = 右子

                tmp = 右子->左子
                右子->左子 = node
                右子 = tmp
            父节点 != null
                (node不是根节点)
                父->(左/右)子 = 右子

                tmp = 右子->左子
                右子->左子 = node
                右子 = tmp

    | node | L | L->L | L->R | R | R->L | R->R | P | YESorNO
        0    *    *      *     *    *      *     *      0
        1    *    *      *     0    *      *     *      0
    */
    void rotate_left(treenode_t<T> *node) {
        if (!node) {
            std::cout << "不可对nullptr左旋!" << std::endl;
            return;
        }

        if (!node->right) {
            // 不可左旋
            std::cout << "不可左旋!!!" << std::endl;
            return;
        } else {
            // 可以左旋
            if (!node->parent) {
                // node是根节点
                this->root = node->right;
                node->right->parent = nullptr;

                treenode_t<T> *tmp = node->right->left;
                node->right->left = node;
                node->parent = node->right;
                node->right = tmp;
                if (tmp)
                    tmp->parent = node;
            } else {
                // node不是根节点
                if (node->parent->left == node) {
                    node->parent->left = node->right;
                    node->right->parent = node->parent;
                } else {
                    node->parent->right = node->right;
                    node->right->parent = node->parent;
                }

                treenode_t<T> *tmp = node->right->left;
                node->right->left = node;
                node->parent = node->right;
                node->right = tmp;
                if (tmp)
                    tmp->parent = node;
            }
        }
    }

#ifdef DEBUG
    treenode_t<T> * operator[](T val) {
        treenode_t<T> *target = nullptr;
        this->trav_bfs([&](treenode_t<T> *node, uint_t, left_or_right_e) -> void {
            if (node->value == val) {
                target = node;
                return;
            }
        });
        return target;
    }
#endif
};

template <typename T>
class search_tree_t : public bintree_t<T> {
protected:
    std::function<bool (const T &a, const T &b)> comparer
        = [] (const T &a, const T &b) -> bool { return a < b; };
    std::function<bool (const T &a, const T &b)> equaler
        = [] (const T &a, const T &b) -> bool { return a == b; };

public:
    search_tree_t() = default;

    search_tree_t(decltype(comparer) cmp) : comparer(cmp) {}
    search_tree_t(decltype(comparer) cmp, decltype(equaler) eql) 
        : comparer(cmp), equaler(eql) {}

    treenode_t<T> *search_value(T val) {
        treenode_t<T> *cur = this->root;

        for (;;) {
            if (cur == nullptr)
                return cur;

            if (this->equaler(cur->value, val)) 
                return cur;
            else {
                if (this->comparer(cur->value, val))
                    cur = cur->right;
                else 
                    cur = cur->left;
            }
        }
    }

    treenode_t<T>* push(T val) {
        treenode_t<T> *cur = this->root;

        for (;;) {
            if (this->equaler(cur->value, val)) return nullptr;

            if (this->comparer(cur->value, val)) {
                if (!cur->right) {
                    cur->addright(val);
                    return cur->right;
                }
                cur = cur->right;
            } else {
                if (!cur->left) {
                    cur->addleft(val);
                    return cur->left;
                }
                cur = cur->left;
            }
        }
    }

    void remove(T val) {
        treenode_t<T> *node = this->search_value(val);
        if (!node) return;
        if (node)
            this->erase(node);
    }

    treenode_t<T> *erase(treenode_t<T> *node) {
        // 先确定此节点的度
        uint_t degree = node->get_degree();
        if (degree == 0) {
            // 直接删
            if (!node->parent) // 根节点
                this->root = nullptr;
            else if (node->parent->left == node)
                node->parent->left = nullptr;
            else 
                node->parent->right = nullptr;
            delete node;
            return nullptr;
        } else if (degree == 1) {
            // 让子节点上来
            if (node->left) {
                node->value = std::move(node->left->value);

                delete node->left;
                node->left = nullptr;
                return node;
            } else {
                node->value = std::move(node->right->value);

                delete node->right;
                node->right = nullptr;
                return node;
            }
        } else {
            /*
                以左子树的最大节点(left_max)替换此被删除的节点
                ml存在:
                    mp继承ml
                mr存在:
                    不可能，因为m不会存在r
            */
            treenode_t<T> *left_max = node->left;
            for (;;) {
                if (left_max->right)
                    left_max = left_max->right;
                else break;
            }
            if (left_max->parent->left == left_max) {
                // 出现这种情况，只可能是left_max是node->left
                // left_max没有right
                node->left = left_max->left;
                if (left_max->left)
                    left_max->left->parent = node;
            } else  {
                // 是父节点右子
                // left_max没有right
                left_max->parent->right = left_max->left;
                if (left_max->left)
                    left_max->left->parent = left_max->parent;
            }
            node->value = std::move(left_max->value);
            delete left_max;
            return node;
        }
    }
};


template <typename T>
class AVL_tree : public search_tree_t<T> {
public:
    AVL_tree() = default;

    template <typename Cmp_T>
    AVL_tree(Cmp_T &&cmp) : search_tree_t<T>(std::forward<Cmp_T>(cmp)) {}

    template <typename Cmp_T, typename Eql_T>
    AVL_tree(Cmp_T &&cmp, Eql_T &&eql) : search_tree_t<T>(std::forward<Cmp_T, Eql_T>(cmp, eql)) {}

    treenode_t<T> *rotate(treenode_t<T> *node) {
        int balance_factor = node->get_balance_factor();
        int child_balance_factor;
        if (balance_factor > 1) {
            // 左偏树
            child_balance_factor = node->left->get_balance_factor();
            if (child_balance_factor < 0) {
                // 先左旋，后右旋
                this->rotate_left(node->left);
                this->rotate_right(node);
            } else {
                // 直接右旋
                this->rotate_right(node);
            }
            return node->parent;
        } else if (balance_factor < -1) {
            // 右偏树
            child_balance_factor = node->right->get_balance_factor();
            if (child_balance_factor > 0) {
                // 先右旋，后左旋
                this->rotate_right(node->right);
                this->rotate_left(node);
            } else {
                // 直接左旋
                this->rotate_left(node);
            }
            return node->parent;
        } else {
            return node;
        }
    }

    treenode_t<T> *push(T val) {
        if (this->root == nullptr) {
            this->root = new treenode_t<T>(val);
            return this->root;
        }

        treenode_t<T> *newnode = dynamic_cast<search_tree_t<T> *>(this)->push(val);
        if (!newnode) return nullptr;

        // 自底向上旋转
        treenode_t<T> *cur = newnode;
        for (;;) {
            if (cur == nullptr) break;
            this->rotate(cur);
            cur = cur->parent;
        }
        return newnode;
    }

    void erase(treenode_t<T> *node) {
        if (!node) return;
        treenode_t<T> *delnode = dynamic_cast<search_tree_t<T> *>(this)->erase(node);
        treenode_t<T> *cur = delnode;
        for (;;) {
            if (cur == nullptr) break;
            this->rotate(cur);
            cur = cur->parent;
        }
    }

    void remove(T val) {
        this->erase(this->search_value(val));
    }
};

#endif  // BINTREE_TREE_HPP
