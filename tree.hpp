#ifndef BINTREE_TREE_HPP
#define BINTREE_TREE_HPP

#include <functional>
#include <iostream>
#include <list>
#include <deque>

#include <cstdint>



/*

using uint_t = uint32_t;
using int_t  = int32_t;

// **************************************
// ************* treenode_t *************
// **************************************
template <typename T>
class treenode_t {
protected:
    void *_left     = nullptr;
    void *_right    = nullptr;
    void *_parent   = nullptr;

public:
    T value;

    treenode_t<T>*& left();
    treenode_t<T>*& right();
    treenode_t<T>*& parent();

public:
    treenode_t(T value);
    treenode_t(treenode_t &&node);

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
template <typename T, typename node_T = treenode_t<T> >
class bintree_t {
public:
    using trav_action_t = std::function<void (node_T *, uint_t, left_or_right_e)>;
    using node_type = node_T;
    using value_type = T;

public:
    node_T *root = nullptr;

public:
    bintree_t() = default;
    ~bintree_t();

    node_T *get_sibling(node_T *node);

    void trav_bfs(trav_action_t action);
    void trav_pre(trav_action_t action);
    void trav_in(trav_action_t action);
    void trav_post(trav_action_t action);

    void print_tree();

    void rotate_right(node_T *node);
    void rotate_left(node_T *node);

#ifdef DEBUG
    node_T * operator[](T val);
#endif
};

// *****************************************
// ************* search_tree_t *************
// *****************************************
template <typename T, typename node_T = treenode_t<T> >
class search_tree_t : public bintree_t<T, node_T> {
private:
    using base_type = bintree_t<T, node_T>;

public:
    enum { EQ_REPLACE = 0, EQ_KEEP } replace_policy = EQ_REPLACE;
    using comparer_type = std::function<bool (const T &a, const T &b)>;
    using equaler_type  = std::function<bool (const T &a, const T &b)>;

protected:
    comparer_type comparer  = [] (const T &a, const T &b) -> bool { return a < b; };
    equaler_type equaler    = [] (const T &a, const T &b) -> bool { return a == b; };

public:
    search_tree_t() = default;
    search_tree_t(decltype(comparer) cmp);
    search_tree_t(decltype(comparer) cmp, decltype(equaler) eql);

    inline void set_replace_policy(decltype(replace_policy) rp);

    node_T *search_value(T val);
    node_T *push(T val);
    void remove(T val);
    node_T *erase(node_T *node);
};

// **************************************
// ************* AVL_tree_t *************
// **************************************
template <typename T, typename node_T = treenode_t<T> >
class AVL_tree_t : public search_tree_t<T, node_T> {
private:
    using base_type = search_tree_t<T, node_T>;

public:
    AVL_tree_t() = default;

    AVL_tree_t(typename base_type::comparer_type cmp);

    AVL_tree_t(typename base_type::comparer_type cmp,
               typename base_type::equaler_type  eql);

    node_T *rotate(node_T *node);
    node_T *push(T val);
    void erase(node_T *node);
    void remove(T val);
};

// *************************************
// ************* RB_tree_t *************
// *************************************

template <typename T>
class RB_treenode_t : public treenode_t<T> {
public:
    RB_treenode_t(T val);

    RB_treenode_t<T>*& left();
    RB_treenode_t<T>*& right();
    RB_treenode_t<T>*& parent();

    enum { COLOR_BLACK = 0, COLOR_RED } color;
};

template <typename T, typename node_T = RB_treenode_t<T> >
class RB_tree_t : public search_tree_t<T, node_T> {
private:
    using base_type = search_tree_t<T, node_T>;

public:
    RB_tree_t() = default;
    RB_tree_t(typename base_type::comparer_type cmp);
    RB_tree_t(typename base_type::comparer_type cmp,
              typename base_type::equaler_type  eql);

    node_T *rotate(node_T *node);
    node_T *push(T val);
    void erase(node_T *node);
    void remove(T val);
    void print_tree();
};

*/

using uint_t = uint32_t;
using int_t  = int32_t;


// **************************************
// ************* treenode_t *************
// **************************************
template <typename T>
class treenode_t {
protected:
    void *_left     = nullptr;
    void *_right    = nullptr;
    void *_parent   = nullptr;

public:
    T value;

    treenode_t<T>*& left() {
        treenode_t<T> **pp = static_cast<treenode_t<T> **>(
                static_cast<void *>(&this->_left)
        );
        return *pp;
    }

    treenode_t<T>*& right() {
        treenode_t<T> **pp = static_cast<treenode_t<T> **>(
                static_cast<void *>(&this->_right)
        );
        return *pp;
    }

    treenode_t<T>*& parent() {
        treenode_t<T> **pp = static_cast<treenode_t<T> **>(
                static_cast<void *>(&this->_parent)
        );
        return *pp;
    }

public:
    treenode_t(T value) : value(value) {}
    treenode_t(treenode_t &&node) {
        this->_left = node._left;
        this->_right = node._right;
        this->_parent = node._parent;
        this->value = node.value;

        node._left   = nullptr;
        node._right  = nullptr;
        node._parent = nullptr;
    }

    void hookleft(treenode_t *newparent) {
        newparent->left() = this;
        this->parent() = newparent;
    }

    void hookright(treenode_t *newparent) {
        newparent->right() = this;
        this->parent() = newparent;
    }
    
    void addleft(treenode_t *child) {
        this->left() = child;
        child->parent() = this;
    }

    void addleft(T val) {
        this->addleft(new treenode_t(val));
    }

    void addright(treenode_t *child) {
        this->right() = child;
        child->parent() = this;
    }

    void addright(T val) {
        this->addright(new treenode_t(val));
    }

    uint_t get_height() {
        uint_t height = 0;

        std::function<void (treenode_t*, uint_t)> traversal =
            [&] (treenode_t *node, uint_t level) -> void {
                if (level > height) height = level;
                if (node->left())
                    traversal(node->left(), level + 1);
                if (node->right())
                    traversal(node->right(), level + 1);
            };
        traversal(this, 0);

        return height;
    }

    uint_t get_degree() {
        if (this->left() && this->right())
            return 2;
        else if (this->left() || this->right())
            return 1;
        else return 0;
    }

    int_t get_balance_factor() {
        int_t left_bfactor = 0;
        int_t right_bfactor = 0;

        if (this->left())
            left_bfactor = this->left()->get_height() + 1;
        if (this->right())
            right_bfactor = this->right()->get_height() + 1;

        return left_bfactor - right_bfactor;
    }
};

typedef enum {
    NODE_ROOT = 0,
    NODE_LEFT,
    NODE_RIGHT
} left_or_right_e;

// *************************************
// ************* bintree_T *************
// *************************************
template <typename T, typename node_T = treenode_t<T> >
class bintree_t {
public:
    using trav_action_t = std::function<void (node_T *, uint_t, left_or_right_e)>;
    using node_type = node_T;
    using value_type = T;

public:
    node_T *root = nullptr;

public:
    /*
               1
        2               3
    4       5       6       7

    */
    bintree_t() = default;
    ~bintree_t() {
        std::list<node_T *> todel;
        this->trav_bfs([&](node_T *node, uint_t, left_or_right_e) {
            todel.push_back(node);
        });
    
        for (const auto &each : todel)
            delete each;
    }

    node_T *get_sibling(node_T *node) {
        if (node->parent()) {
            if (node->parent()->left() == node)
                return node->parent()->right();
            else
                return node->parent()->left();
        } else return nullptr;
    }

    /* 层序遍历 */
    void trav_bfs(trav_action_t action) {
        if (!this->root) return;
        using std::deque;
        deque<node_T *> dq;

        dq.push_back(this->root);
        while (!dq.empty()) {
            node_T *node = dq.front();
            dq.pop_front();

            action(node, 0, NODE_ROOT);

            if (node->left())
                dq.push_back(node->left());
            if (node->right())
                dq.push_back(node->right());
        }
    }

    /* 前序遍历 */
    void trav_pre(trav_action_t action) {
        if (!this->root) return;
        std::function<void (node_T *, trav_action_t, uint_t, left_or_right_e)> recur_trav = 
            [&] (node_T *node, trav_action_t action, uint_t level, left_or_right_e lorr) -> void {
                action(node, level, lorr);

                if (node->left())
                    recur_trav(node->left(), action, level + 1, NODE_LEFT);
                if (node->right())
                    recur_trav(node->right(), action, level + 1, NODE_RIGHT);
            };
        recur_trav(this->root, action, 0, NODE_ROOT);
    }

    /* 中序遍历 */
    void trav_in(trav_action_t action) {
        if (!this->root) return;
        std::function<void (node_T *, trav_action_t, uint_t, left_or_right_e)> recur_trav = 
            [&] (node_T *node, trav_action_t action, uint_t level, left_or_right_e lorr) -> void {
                if (node->left())
                    recur_trav(node->left(), action, level + 1, NODE_LEFT);

                action(node, level, lorr);

                if (node->right())
                    recur_trav(node->right(), action, level + 1, NODE_RIGHT);
            };
        recur_trav(this->root, action, 0, NODE_ROOT);
    }

    /* 后序遍历 */
    void trav_post(trav_action_t action) {
        if (!this->root) return;
        std::function<void (node_T *, trav_action_t, uint_t, left_or_right_e)> recur_trav = 
            [&] (node_T *node, trav_action_t action, uint_t level, left_or_right_e lorr) -> void {
                if (node->left())
                    recur_trav(node->left(), action, level + 1, NODE_LEFT);
                if (node->right())
                    recur_trav(node->right(), action, level + 1, NODE_RIGHT);

                action(node, level, lorr);
            };
        recur_trav(this->root, action, 0, NODE_ROOT);
    }

    /* 打印树 */
    void print_tree() {
        this->trav_pre(
            [] (node_T *node, uint_t level, left_or_right_e lorr) {
                for (uint_t i = 0; i < level; i++)
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
                if (node->parent())
                    std::cout << " |P " << node->parent()->value << std::endl;
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
    void rotate_right(node_T *node) {
        if (!node) {
            std::cout << "不可对nullptr右旋!" << std::endl;
            return;
        }

        if (!node->left()) {
            // 不可右旋
            std::cout << "不可右旋!!!" << std::endl;
            return;
        } else {
            // 可以右旋
            if (!node->parent()) {
                // node 是根节点
                this->root = node->left();
                node->left()->parent() = nullptr;

                node_T *tmp = node->left()->right();
                node->left()->right() = node;
                node->parent() = node->left();
                node->left() = tmp;
                if (tmp)
                    tmp->parent() = node;
            } else {
                // node 不是根节点
                if (node->parent()->left() == node) {
                    node->parent()->left() = node->left();
                    node->left()->parent() = node->parent();
                } else {
                    node->parent()->right() = node->left();
                    node->left()->parent() = node->parent();
                }

                node_T *tmp = node->left()->right();
                node->left()->right() = node;
                node->parent() = node->left();
                node->left() = tmp;
                if (tmp)
                    tmp->parent() = node;
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
    void rotate_left(node_T *node) {
        if (!node) {
            std::cout << "不可对nullptr左旋!" << std::endl;
            return;
        }

        if (!node->right()) {
            // 不可左旋
            std::cout << "不可左旋!!!" << std::endl;
            return;
        } else {
            // 可以左旋
            if (!node->parent()) {
                // node是根节点
                this->root = node->right();
                node->right()->parent() = nullptr;

                node_T *tmp = node->right()->left();
                node->right()->left() = node;
                node->parent() = node->right();
                node->right() = tmp;
                if (tmp)
                    tmp->parent() = node;
            } else {
                // node不是根节点
                if (node->parent()->left() == node) {
                    node->parent()->left() = node->right();
                    node->right()->parent() = node->parent();
                } else {
                    node->parent()->right() = node->right();
                    node->right()->parent() = node->parent();
                }

                node_T *tmp = node->right()->left();
                node->right()->left() = node;
                node->parent() = node->right();
                node->right() = tmp;
                if (tmp)
                    tmp->parent() = node;
            }
        }
    }

#ifdef DEBUG
    node_T * operator[](T val) {
        node_T *target = nullptr;
        this->trav_bfs([&](node_T *node, uint_t, left_or_right_e) -> void {
            if (node->value == val) {
                target = node;
                return;
            }
        });
        return target;
    }
#endif
};

// *****************************************
// ************* search_tree_t *************
// *****************************************
template <typename T, typename node_T = treenode_t<T> >
class search_tree_t : public bintree_t<T, node_T> {
private:
    using base_type = bintree_t<T, node_T>;

public:
    enum { EQ_REPLACE = 0, EQ_KEEP } replace_policy = EQ_REPLACE;
    using comparer_type = std::function<bool (const T &a, const T &b)>;
    using equaler_type  = std::function<bool (const T &a, const T &b)>;

protected:
    comparer_type comparer  = [] (const T &a, const T &b) -> bool { return a < b; };
    equaler_type equaler    = [] (const T &a, const T &b) -> bool { return a == b; };

public:
    search_tree_t() = default;

    search_tree_t(decltype(comparer) cmp) : comparer(cmp) {}
    search_tree_t(decltype(comparer) cmp, decltype(equaler) eql)
        : comparer(cmp), equaler(eql) {}

    inline void set_replace_policy(decltype(replace_policy) rp) {
        this->replace_policy = rp;
    }

    node_T *search_value(T val) {
        node_T *cur = this->root;

        for (;;) {
            if (cur == nullptr)
                return cur;

            if (this->equaler(cur->value, val)) 
                return cur;
            else {
                if (this->comparer(cur->value, val))
                    cur = cur->right();
                else 
                    cur = cur->left();
            }
        }
    }

    node_T* push(T val) {
        if (!this->root) {
            this->root = new node_T(val);
            return this->root;
        }

        node_T *cur = this->root;

        for (;;) {
            if (this->equaler(cur->value, val)) {
                switch (this->replace_policy) {
                    case EQ_KEEP: 
                        return nullptr;
                    case EQ_REPLACE:
                        cur->value = std::forward<T>(val);
                        return cur;
                    default: return nullptr;    // Shouldn't be here
                }
            }

            if (this->comparer(cur->value, val)) {
                if (!cur->right()) {
                    cur->addright(val);
                    return cur->right();
                }
                cur = cur->right();
            } else {
                if (!cur->left()) {
                    cur->addleft(val);
                    return cur->left();
                }
                cur = cur->left();
            }
        }
    }

    void remove(T val) {
        node_T *node = this->search_value(val);
        if (!node) return;
        if (node)
            this->erase(node);
    }

    node_T *erase(node_T *node) {
        // 先确定此节点的度
        uint_t degree = node->get_degree();
        if (degree == 0) {
            // 直接删
            if (!node->parent()) // 根节点
                this->root = nullptr;
            else if (node->parent()->left() == node)
                node->parent()->left() = nullptr;
            else 
                node->parent()->right() = nullptr;
            delete node;
            return nullptr;
        } else if (degree == 1) {
            // 让子节点上来
            if (node->left()) {
                node->value = std::move(node->left()->value);

                delete node->left();
                node->left() = nullptr;
                return node;
            } else {
                node->value = std::move(node->right()->value);

                delete node->right();
                node->right() = nullptr;
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
            node_T *left_max = node->left();
            for (;;) {
                if (left_max->right())
                    left_max = left_max->right();
                else break;
            }
            if (left_max->parent()->left() == left_max) {
                // 出现这种情况，只可能是left_max是node->left()
                // left_max没有right
                node->left() = left_max->left();
                if (left_max->left())
                    left_max->left()->parent() = node;
            } else  {
                // 是父节点右子
                // left_max没有right
                left_max->parent()->right() = left_max->left();
                if (left_max->left())
                    left_max->left()->parent() = left_max->parent();
            }
            node->value = std::move(left_max->value);
            delete left_max;
            return node;
        }
    }
};


// **************************************
// ************* AVL_tree_t *************
// **************************************
template <typename T, typename node_T = treenode_t<T> >
class AVL_tree_t : public search_tree_t<T, node_T> {
private:
    using base_type = search_tree_t<T, node_T>;

public:
    AVL_tree_t() = default;

    AVL_tree_t(typename base_type::comparer_type cmp)
        : base_type(cmp) {}

    AVL_tree_t(typename base_type::comparer_type cmp,
               typename base_type::equaler_type  eql)
        : base_type(cmp, eql) {}

    node_T *rotate(node_T *node) {
        int_t balance_factor = node->get_balance_factor();
        int_t child_balance_factor;
        if (balance_factor > 1) {
            // 左偏树
            child_balance_factor = node->left()->get_balance_factor();
            if (child_balance_factor < 0) {
                // 先左旋，后右旋
                this->rotate_left(node->left());
                this->rotate_right(node);
            } else {
                // 直接右旋
                this->rotate_right(node);
            }
            return node->parent();
        } else if (balance_factor < -1) {
            // 右偏树
            child_balance_factor = node->right()->get_balance_factor();
            if (child_balance_factor > 0) {
                // 先右旋，后左旋
                this->rotate_right(node->right());
                this->rotate_left(node);
            } else {
                // 直接左旋
                this->rotate_left(node);
            }
            return node->parent();
        } else {
            return node;
        }
    }

    node_T *push(T val) {
        if (this->root == nullptr) {
            this->root = new node_T(val);
            return this->root;
        }

        node_T *newnode = dynamic_cast<base_type *>(this)->push(val);
        if (!newnode) return nullptr;

        // 自底向上旋转
        node_T *cur = newnode;
        for (;;) {
            if (cur == nullptr) break;
            this->rotate(cur);
            cur = cur->parent();
        }
        return newnode;
    }

    void erase(node_T *node) {
        if (!node) return;
        node_T *delnode = dynamic_cast<base_type *>(this)->erase(node);
        node_T *cur = delnode;
        for (;;) {
            if (cur == nullptr) break;
            this->rotate(cur);
            cur = cur->parent();
        }
    }

    void remove(T val) {
        this->erase(this->search_value(val));
    }
};

// *************************************
// ************* RB_tree_t *************
// *************************************

template <typename T>
class RB_treenode_t : public treenode_t<T> {
public:
    RB_treenode_t(T val) : treenode_t<T>(val) {}

    RB_treenode_t<T>*& left() {
        RB_treenode_t<T> **pp = static_cast<RB_treenode_t<T> **>(
                static_cast<void *>(&this->_left)
        );
        return *pp;
    }

    RB_treenode_t<T>*& right() {
        RB_treenode_t<T> **pp = static_cast<RB_treenode_t<T> **>(
                static_cast<void *>(&this->_right)
        );
        return *pp;
    }

    RB_treenode_t<T>*& parent() {
        RB_treenode_t<T> **pp = static_cast<RB_treenode_t<T> **>(
                static_cast<void *>(&this->_parent)
        );
        return *pp;
    }

    enum { COLOR_BLACK = 0, COLOR_RED } color;
};

template <typename T, typename node_T = RB_treenode_t<T> >
class RB_tree_t : public search_tree_t<T, node_T> {
private:
    using base_type = search_tree_t<T, node_T>;

public:
    RB_tree_t() = default;
    RB_tree_t(typename base_type::comparer_type cmp)
        : base_type(cmp) {}
    RB_tree_t(typename base_type::comparer_type cmp,
              typename base_type::equaler_type  eql)
        : base_type(cmp, eql) {}

    [[noreturn]]
    node_T *rotate(node_T *node) {
    }

    node_T *push(T val) {
        std::function<void (node_T *)> maintain = [&](node_T *node) {
            if (node == this->root) {
                node->color = node_T::COLOR_BLACK;
            } else if (node->parent()->color == node_T::COLOR_BLACK) {
                node->color = node_T::COLOR_RED;
            } else {// 父节点是红色，一定有黑色祖父节点
                node_T *uncle = this->get_sibling(node->parent());
                node_T *grandparent = node->parent()->parent();
                bool is_uncle_black = false;
                if (!uncle)
                    is_uncle_black = true;
                else if (uncle->color == node_T::COLOR_BLACK) 
                    is_uncle_black = true;

                if (is_uncle_black) {
                    // 叔节点是黑色 NIL
                    // 我红，父黑，祖红，旋转祖
                    node->color = node_T::COLOR_RED;
                    node->parent()->color = node_T::COLOR_BLACK;
                    grandparent->color = node_T::COLOR_RED;

                    grandparent->get_height();

                    if (node == node->parent()->left()
                     && node->parent() == grandparent->left()) {
                        this->rotate_right(grandparent);
                    } else if (node == node->parent()->right()
                            && node->parent() == grandparent->left()) {
                        this->rotate_left(node->parent());
                        this->rotate_right(grandparent);
                    } else if (node == node->parent()->left()
                            && node->parent() == grandparent->right()) {
                        this->rotate_right(node->parent());
                        this->rotate_left(grandparent);
                    } else if (node == node->parent()->right()
                            && node->parent() == grandparent->right()) {
                        this->rotate_left(grandparent);
                    }
                } else {
                    // 叔节点是红色
                    node->color = node_T::COLOR_RED;
                    node->parent()->color = node_T::COLOR_BLACK;
                    uncle->color = node_T::COLOR_BLACK;
                    maintain(grandparent);
                }
            }
        };

        node_T *newnode = dynamic_cast<base_type *>(this)->push(val);
        maintain(newnode);

        return newnode;
    }

    void erase(node_T *node) {
    }

    void remove(T val) {
        node_T *node = this->search_value(val);
        this->erase(node);
    }

    void print_tree() {
        this->trav_pre(
            [] (node_T *node, uint_t level, left_or_right_e lorr) {
                for (uint_t i = 0; i < level; i++)
                    std::cout << "======|";
                std::cout << "\b";
                switch (lorr) {
                case NODE_ROOT:
                    std::cout << "=========== print_tree ==========\nRT";
                    break;
                case NODE_LEFT:
                    std::cout << "[L]";
                    break;
                case NODE_RIGHT:
                    std::cout << "[R]";
                    break;
                default:;
                }
                if (node->color == node_T::COLOR_RED)
                    std::cout << " Red ";
                else
                    std::cout << " Black ";

                std::cout << node->value;
                if (node->parent())
                    std::cout << " |P " << node->parent()->value << std::endl;
                else
                    std::cout << " |P NUL" << std::endl;
            }
        );
        std::cout << "Print OK." << std::endl;
    }
};


#endif  // BINTREE_TREE_HPP
