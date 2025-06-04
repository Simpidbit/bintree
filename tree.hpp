#ifndef BINTREE_TREE_HPP
#define BINTREE_TREE_HPP

#include <functional>
#include <iostream>
#include <list>
#include <deque>

#include <cstdint>
#include <cassert>



using uint_t = uint32_t;
using int_t  = int32_t;


// **************************************
// ************* treenode_t *************
// **************************************

/**
 * @brief 树节点
 */
template <typename T>
class treenode_t {
protected:
  void *_left   = nullptr;
  void *_right  = nullptr;
  void *_parent = nullptr;

public:
  T value;

  constexpr inline treenode_t<T>*& left() noexcept {
    return *static_cast<treenode_t<T> **>(static_cast<void *>(&this->_left));
  }

  constexpr inline treenode_t<T>*& right() noexcept {
    return *static_cast<treenode_t<T> **>(static_cast<void *>(&this->_right));
  }

  constexpr inline treenode_t<T>*& parent() noexcept {
    return *static_cast<treenode_t<T> **>(static_cast<void *>(&this->_parent));
  }

  inline treenode_t<T>*& which_son_is(treenode_t<T> *node) noexcept {
    if (this->right() == node) return this->right();
    else if (this->left() == node) return this->left();
    else assert(false);
  }

public:
  treenode_t(T&& value) {
    std::cout << "in treenode_t(T&& value)" << std::endl;
    this->value = std::move(value);
  }

  treenode_t(const T &value) {
    this->value = value;
  }

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

  void addleft(const T& val) {
    this->addleft(new treenode_t(val));
  }

  void addleft(T &&val) {
    this->addleft(new treenode_t(val));
  }

  void addright(treenode_t *child) {
    this->right() = child;
    child->parent() = this;
  }

  void addright(const T& val) {
    std::cout << "in const T& addright()" << std::endl;
    this->addright(new treenode_t(val));
  }

  void addright(T&& val) {
    std::cout << "in addright" << std::endl;
    std::cout << "val.first = " << val.first << std::endl;
    std::cout << "val.second = " << val.second << std::endl;
    treenode_t *child = new treenode_t(std::move(val));
    std::cout << "child make ok" << std::endl;
    this->addright(child);
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

/**
 * @brief 标示节点是父节点的左子节点还是右子节点，还是本身就是根节点.
 */
typedef enum {
  NODE_ROOT = 0,
  NODE_LEFT,
  NODE_RIGHT
} left_or_right_e;

// *************************************
// ************* bintree_T *************
// *************************************

/**
 * @brief 最普通的二叉树，是其它二叉树的祖先类.
 */
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
  ~bintree_t() {
    std::list<node_T *> todel;
    this->trav_bfs([&](node_T *node, uint_t, left_or_right_e) {
      todel.push_back(node);
    });
  
    for (const auto &each : todel)
      delete each;
  }

  /**
   * @brief 获取某个节点的兄弟节点.
   * @param node 目标节点指针.
   * @return 目标节点 node 的兄弟节点指针.
   */
  node_T *get_sibling(node_T *node) {
    if (node->parent()) {
      if (node->parent()->left() == node)
        return node->parent()->right();
      else
        return node->parent()->left();
    } else return nullptr;
  }

  /**
   * @brief 层序遍历二叉树.
   */
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

  /**
   * @brief 前序遍历二叉树.
   */
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

  /**
   * @brief 中序遍历二叉树
   */
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

  /**
   * @brief 后序遍历二叉树
   */
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
  enum { EQ_REPLACE = 0, EQ_KEEP } replace_policy = EQ_KEEP;
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

  node_T *search_value(T val) const {
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

  node_T* push(const T& val) {
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
            cur->value = val;
            return cur;
          default: return nullptr;  // Shouldn't be here
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

  node_T* push(T&& val) {
    if (!this->root) {
      this->root = new node_T(std::move(val));
      return this->root;
    }

    node_T *cur = this->root;


    for (;;) {
      std::cout << "Push for loop..." << std::endl;
      if (this->equaler(cur->value, val)) {
        std::cout << "in if" << std::endl;
        switch (this->replace_policy) {
          case EQ_KEEP:
            return nullptr;
          case EQ_REPLACE:
            cur->value = std::move(val);
            return cur;
          default: return nullptr;  // Shouldn't be here
        }
      }
      std::cout << "out if" << std::endl;

      if (this->comparer(cur->value, val)) {
        std::cout << "in 2 if " << std::endl;
        if (!cur->right()) {
          std::cout << "in 2 if if" << std::endl;
          cur->addright(std::move(val));
          std::cout << "addright OK" << std::endl;
          return cur->right();
        }
        std::cout << "out 2 if if" << std::endl;
        cur = cur->right();
      } else {
        std::cout << "in else" << std::endl;
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

  constexpr inline RB_treenode_t<T>*& left() noexcept {
    return *static_cast<RB_treenode_t<T> **> (static_cast<void *>(&this->_left));
  }

  constexpr inline RB_treenode_t<T>*& right() noexcept {
    return *static_cast<RB_treenode_t<T> **> (static_cast<void *>(&this->_right));
  }

  constexpr inline RB_treenode_t<T>*& parent() noexcept {
    return *static_cast<RB_treenode_t<T> **> (static_cast<void *>(&this->_parent));
  }

  inline RB_treenode_t<T>*& which_son_is(RB_treenode_t<T> *node) noexcept {
    return *static_cast<RB_treenode_t<T> **> (static_cast<void *>(&dynamic_cast<treenode_t<T> *>(this)->which_son_is(node)));
  }

  enum { COLOR_BLACK = 0, COLOR_RED } color;
};

template <typename T, typename node_T = RB_treenode_t<T> >
class RB_tree_t : public search_tree_t<T, node_T> {
protected:
  using base_type = search_tree_t<T, node_T>;

  /**
   * @brief 取前驱
   */
  node_T* get_front_node(node_T *node) {
    node_T *fn = node->left();
    if (!fn) return nullptr;
    while (fn->right()) fn = fn->right();
    return fn;
  }

  /**
   * @brief 取后继
   */
  node_T* get_back_node(node_T *node) {
    node_T *bn = node->right();
    if (!bn) return nullptr;
    while (bn->left()) bn = bn->left();
    return bn;
  }

  void rebel(node_T *son) {
    assert(son->parent() != nullptr);

    node_T *oson = son;
    node_T *onode = son->parent();
    node_T *oparent = onode->parent();

    if (oparent) {
      oparent->which_son_is(onode) = oson;
      oson->parent() = oparent;
    } else {
      this->root = oson;
      oson->parent() = nullptr;
    }

    if (onode->left() == son) {
      oson->right() = onode->right();
      if (onode->right())
        onode->right()->parent() = oson;
    } else if (onode->right() == son) {
      oson->left() = onode->left();
      if (onode->left())
        onode->left()->parent() = oson;
    } else assert(false);
  }

  void maintain(node_T *node) {
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
        if (node == node->parent()->left()
         && node->parent() == grandparent->left()) {

          node->color = node_T::COLOR_RED;
          node->parent()->color = node_T::COLOR_BLACK;
          grandparent->color = node_T::COLOR_RED;

          this->rotate_right(grandparent);
        } else if (node == node->parent()->right()
            && node->parent() == grandparent->left()) {

          node->color = node_T::COLOR_BLACK;
          node->parent()->color = node_T::COLOR_RED;
          grandparent->color = node_T::COLOR_RED;

          this->rotate_left(node->parent());
          this->rotate_right(grandparent);
        } else if (node == node->parent()->left()
            && node->parent() == grandparent->right()) {

          node->color = node_T::COLOR_BLACK;
          node->parent()->color = node_T::COLOR_RED;
          grandparent->color = node_T::COLOR_RED;

          this->rotate_right(node->parent());
          this->rotate_left(grandparent);
        } else {
          node->color = node_T::COLOR_RED;
          node->parent()->color = node_T::COLOR_BLACK;
          grandparent->color = node_T::COLOR_RED;

          this->rotate_left(grandparent);
        }
      } else {
        // 叔节点是红色
        node->color = node_T::COLOR_RED;
        node->parent()->color = node_T::COLOR_BLACK;
        uncle->color = node_T::COLOR_BLACK;
        this->maintain(grandparent);
      }
    }
  }

  void recursive_delete(node_T *node) {
    node_T *front_node = this->get_front_node(node);
    node_T *back_node = this->get_back_node(node);

    assert(front_node != node->left() || back_node != node->right());

    if (front_node != node->left()) {
      // 和前驱互换
      T tmp = std::move(node->value);
      node->value = std::move(front_node->value);
      front_node->value = std::move(tmp);

      this->erase(front_node);
    } else {
      // 和后继互换
      T tmp = std::move(node->value);
      node->value = std::move(back_node->value);
      back_node->value = std::move(tmp);

      this->erase(back_node);
    }
  }

  void recursive_balance_maintain_m1(node_T *node) {
    if (node == this->root) return;
    if (node->parent()) {
      if (node->parent()->left() == node)
        balance_maintain(node->parent(), false);
      else
        balance_maintain(node->parent(), true);
    }
  }

  void balance_maintain_c0(node_T *node, bool side) {

    if (side) { // 右低
      if (!node->left()->right())
        this->rotate_right(node);
      else {
        if (node->left()->left()) {
          node->color = node_T::COLOR_BLACK;
          node->left()->color = node_T::COLOR_RED;
          node->left()->left()->color = node_T::COLOR_BLACK;
          this->rotate_right(node);
        } else {
          node->left()->color = node_T::COLOR_RED;
          node->left()->right()->color = node_T::COLOR_BLACK;
          this->rotate_left(node->left());
          this->rotate_right(node);
        }
      }
    } else {    // 左低
      if (!node->right()->left())
        this->rotate_left(node);
      else {
        if (node->right()->right()) {
          node->color = node_T::COLOR_BLACK;
          node->right()->color = node_T::COLOR_RED;
          node->right()->right()->color = node_T::COLOR_BLACK;
          this->rotate_left(node);
        } else {
          node->right()->color = node_T::COLOR_RED;
          node->right()->left()->color = node_T::COLOR_BLACK;
          this->rotate_right(node->right());
          this->rotate_left(node);
        }
      }
    }
  }

  void balance_maintain_c1(node_T *node, bool side) {

    auto color_of = [] (node_T *node) -> auto {
      if (!node) return node_T::COLOR_BLACK;
      else return node->color;
    };

    if (node->color == node_T::COLOR_RED) {
      balance_maintain_c0(node, side);
      return;
    }

    if (node->get_height() == 1) {
      if (side) { // 右低
        node->left()->color = node_T::COLOR_RED;
      } else {    // 左低
        node->right()->color = node_T::COLOR_RED;
      }
      recursive_balance_maintain_m1(node);
    } else {
      if (side) { // 右低
        if (node->left()->color == node_T::COLOR_RED) {
          if (!node->left()->right()->left() && !node->left()->right()->right()) {
            node->left()->color = node_T::COLOR_BLACK;
            node->left()->right()->color = node_T::COLOR_RED;
            this->rotate_right(node);
          } else {
            node->left()->color = node_T::COLOR_BLACK;
            if (node->left()->right()->left()) {
              node->left()->right()->color = node_T::COLOR_RED;
              node->left()->right()->left()->color = node_T::COLOR_BLACK;
              this->rotate_right(node);
              this->rotate_right(node);
            } else {
              this->rotate_right(node);
              this->rotate_left(node->left());
              this->rotate_right(node);
            }
          }
          return;
        }

        if (color_of(node->left()->left()) != node_T::COLOR_BLACK)
          node->left()->left()->color = node_T::COLOR_BLACK;
        if (node->left()->left() && !node->left()->right())
          this->rotate_right(node);
        else if (!node->left()->left() && node->left()->right()) {
          node->left()->right()->color = node_T::COLOR_BLACK;
          this->rotate_left(node->left());
          this->rotate_right(node);
        } else
          this->rotate_right(node);
      } else {    // 左低
        if (node->right()->color == node_T::COLOR_RED) {
          if (!node->right()->left()->right() && !node->right()->left()->left()) {
            node->right()->color = node_T::COLOR_BLACK;
            node->right()->left()->color = node_T::COLOR_RED;
            this->rotate_left(node);
          } else {
            node->right()->color = node_T::COLOR_BLACK;
            if (node->right()->left()->right()) {
              node->right()->left()->color = node_T::COLOR_RED;
              node->right()->left()->right()->color = node_T::COLOR_BLACK;
              this->rotate_left(node);
              this->rotate_left(node);
            } else {
              this->rotate_left(node);
              this->rotate_right(node->right());
              this->rotate_left(node);
            }
          }
          return;
        }

        if (color_of(node->right()->right()) != node_T::COLOR_BLACK)
          node->right()->right()->color = node_T::COLOR_BLACK;
        if (node->right()->right() && !node->right()->left())
          this->rotate_left(node);
        else if (!node->right()->right() && node->right()->left()) {
          node->right()->left()->color = node_T::COLOR_BLACK;
          this->rotate_right(node->right());
          this->rotate_left(node);
        } else
          this->rotate_left(node);
      }
    }
  }

  void balance_maintain_c2(node_T *node, bool side) {
    if (!side) { // 左低
      node->left()->color = node_T::COLOR_BLACK;
    } else {    // 右低
      node->right()->color = node_T::COLOR_BLACK;
    }
  }

  void balance_maintain_c3(node_T *node, bool side) {
    if (!side) { // 左低
      node->left()->color = node_T::COLOR_BLACK;
    } else {    // 右低
      node->right()->color = node_T::COLOR_BLACK;
    }
  }

  void balance_maintain_c4(node_T *node, bool side) {
    if (!side) { // 左低
      this->rotate_left(node);
    } else {    // 右低
      this->rotate_right(node);
    }
    node->color = node_T::COLOR_RED;

    recursive_balance_maintain_m1(node->parent());
  }

  void balance_maintain_c5(node_T *node, bool side) {
    if (!side) { // 左低
      this->rotate_right(node->right());
      this->rotate_left(node);
      if (node->parent() == this->root)
        node->parent()->color = node_T::COLOR_BLACK;
    } else {    // 右低
      this->rotate_left(node->left());
      this->rotate_right(node);
      if (node->parent() == this->root)
        node->parent()->color = node_T::COLOR_BLACK;
    }
    recursive_balance_maintain_m1(node->parent());
  }

  void balance_maintain_c6(node_T *node, bool side) {
    if (!side) { // 左低
      node->right()->left()->color = node_T::COLOR_BLACK;
      this->rotate_right(node->right());
      this->rotate_left(node);
    } else {    // 右低
      node->left()->right()->color = node_T::COLOR_BLACK;
      this->rotate_left(node->left());
      this->rotate_right(node);
    }
  }

  void balance_maintain_c7(node_T *node, bool side) {
    if (!side)  this->rotate_left(node);
    else        this->rotate_right(node);
  }

  void balance_maintain_c8(node_T *node, bool side) {
    if (!side) { // 左低
      node->color = node_T::COLOR_BLACK;
      this->rotate_right(node->right());
      this->rotate_left(node);
    } else {    // 右低
      node->color = node_T::COLOR_BLACK;
      this->rotate_left(node->left());
      this->rotate_right(node);
    }
  }

  void balance_maintain_c9(node_T *node, bool side) {
    if (!side) { // 左低
      node->color = node_T::COLOR_RED;
      node->right()->color = node_T::COLOR_BLACK;
      this->rotate_left(node);
      this->rotate_left(node);
    } else {    // 右低
      node->color = node_T::COLOR_RED;
      node->left()->color = node_T::COLOR_BLACK;
      this->rotate_right(node);
      this->rotate_right(node);
    }
  }

  void balance_maintain_c10(node_T *node, bool side) {
    if (!side) { // 左低
      node->right()->left()->left()->color = node_T::COLOR_BLACK;
      this->rotate_left(node);
      this->rotate_left(node);
      this->rotate_left(node);
      this->rotate_right(node->parent()->parent());
      this->rotate_right(node->parent()->parent());
    } else {    // 右低
      node->left()->right()->right()->color = node_T::COLOR_BLACK;
      this->rotate_right(node);
      this->rotate_right(node);
      this->rotate_right(node);
      this->rotate_left(node->parent()->parent());
      this->rotate_left(node->parent()->parent());
    }
  }

  /** 
   * @brief 平衡维护程序.
   * @param oparent 待维护的子树根.
   * @param left_bh oparent 左子的黑高.
   * @param right_bh oparent 右子的黑高.
   */
  void balance_maintain(node_T *node, bool side /* 左低: false, 右低: true */) {
    auto color_of = [] (node_T *node) -> auto {
      if (!node) return node_T::COLOR_BLACK;
      else return node->color;
    };


    if (!node->left() || !node->right()) {   // node 只有 1 子，且无孙
      balance_maintain_c1(node, side);
    } else {                                // 2 子情况
      if (node->left()->color != node->right()->color) {
        if (side && color_of(node->right()) == node_T::COLOR_RED)
          balance_maintain_c2(node, side);
        else if (!side && color_of(node->left()) == node_T::COLOR_RED)
          balance_maintain_c2(node, side);
        else {
          if (!side) {
            if (color_of(node->right()->left()->left()) == node_T::COLOR_BLACK)
              balance_maintain_c9(node, side);
            else
              balance_maintain_c10(node, side);
          } else {
            if (color_of(node->left()->right()->right()) == node_T::COLOR_BLACK)
              balance_maintain_c9(node, side);
            else
              balance_maintain_c10(node, side);
          }
        }
      }
      else if (node->left()->color == node_T::COLOR_RED &&
               node->right()->color == node_T::COLOR_RED)
        balance_maintain_c3(node, side);
      else if (node->left()->color == node_T::COLOR_BLACK &&
               node->right()->color == node_T::COLOR_BLACK) {
        if (node->color == node_T::COLOR_BLACK) { // 3 黑
          if (!side) { // 左低
            if (color_of(node->right()->left()) == node_T::COLOR_BLACK)
              balance_maintain_c4(node, side);
            else if (color_of(node->parent()) == node_T::COLOR_BLACK)
              balance_maintain_c5(node, side);
            else
              balance_maintain_c6(node, side);
          } else {    // 右低
            if (color_of(node->left()->right()) == node_T::COLOR_BLACK)
              balance_maintain_c4(node, side);
            else if (color_of(node->parent()) == node_T::COLOR_BLACK)
              balance_maintain_c5(node, side);
            else
              balance_maintain_c6(node, side);
          }
        } else { // node 红子黑
          if (!side) {
            if (color_of(node->right()->left()) == node_T::COLOR_BLACK)
              balance_maintain_c7(node, side);
            else
              balance_maintain_c8(node, side);
          } else {
            if (color_of(node->left()->right()) == node_T::COLOR_BLACK)
              balance_maintain_c7(node, side);
            else
              balance_maintain_c8(node, side);
          }
        }
      } else assert(false);
    }
  }

  void delete_root() {
    node_T *oroot = this->root;
    auto degree = this->root->get_degree();
    if (degree == 0) {
      this->root = nullptr;
      delete oroot;
    } else if (degree == 1) {
      if (oroot->left())
        this->root = oroot->left();
      else
        this->root = oroot->right();
      this->root->color = node_T::COLOR_BLACK;
      delete oroot;
      this->root->parent() = nullptr;
    } else {
      // root 有两子
      node_T *front_node = this->get_front_node(oroot);
      node_T *back_node = this->get_back_node(oroot);
      if (front_node == oroot->left() && back_node == oroot->right()) {
        if (oroot->left()->left()) {
          this->rebel(front_node);    // 把前驱抬上来
          if (this->root->right()->color == node_T::COLOR_BLACK)
            this->root->left()->color = node_T::COLOR_BLACK;
        } else {
          this->rebel(back_node);
          if (this->root->left()->color == node_T::COLOR_BLACK) {
            if (this->root->right())
              this->root->right()->color = node_T::COLOR_BLACK;
            else
              this->root->left()->color = node_T::COLOR_RED;
          }
        }
        this->root->color = node_T::COLOR_BLACK;
      } else recursive_delete(oroot);
    }
    return;
  }

  // node 无子.
  void delete_degree_0(node_T *node) {
    node_T *oparent = node->parent();

    if (node->color == node_T::COLOR_BLACK) {
      // 如果是黑色, 移除后还要启动平衡维护程序.
      if (node->parent()->left() == node) {
        node->parent()->left() = nullptr;
        delete node;
        balance_maintain(oparent, false);
      } else {
        node->parent()->right() = nullptr;
        delete node;
        balance_maintain(oparent, true);
      }
    } else {
      // 红色，直接移除
      node->parent()->which_son_is(node) = nullptr;
      delete node;
    }
    return;
  }

  // node 有 1 子.
  void delete_degree_1(node_T *node) {
    assert(node->color == node_T::COLOR_BLACK);

    node_T *sibling = this->get_sibling(node);
    node_T *son = node->left() ? node->left() : node->right();
    this->rebel(son);
    son->color = node_T::COLOR_BLACK;
  }

  // node 有 2 子.
  void delete_degree_2(node_T *node) {
    node_T *sibling = this->get_sibling(node);

    // node 左子有子.
    bool condition_node_left_has_son = 
      node->left()->left() || node->left()->right();

    // node 右子有子.
    bool condition_node_right_has_son =
      node->right()->left() || node->right()->right();

    node_T *front_node = this->get_front_node(node),
           *back_node  = this->get_back_node(node);

    if (front_node == node->left() &&
        back_node == node->right()) {             // 直线形子树
      node_T *onode = node;
      node_T *t1 = node->left();
      node_T *t2 = node->right();
      node_T *t1l = node->left()->left();
      node_T *t2r = node->right()->right();
      if (node->color == node_T::COLOR_RED) {     // node 是红色.

        if (t1l) {
          this->rebel(t1);
          t1->color = node_T::COLOR_RED;
          t1l->color = node_T::COLOR_BLACK;
        } else if (t2r) {
          this->rebel(t2);
          t2->color = node_T::COLOR_RED;
          t2r->color = node_T::COLOR_BLACK;
        } else {
          this->rebel(t1);
          t2->color = node_T::COLOR_RED;
        }
      } else {                                    // node 是黑色.
        // 此时 node 的左子和右子颜色必相同
        assert(node->left()->color == node->right()->color);
        if (node->left()->color == node_T::COLOR_BLACK) { // node 左子右子都是黑色.
          if (t1l) {
            this->rebel(t1);
            t1->color = node_T::COLOR_BLACK;
            t1l->color = node_T::COLOR_BLACK;
          } else if (t2r) {
            this->rebel(t2);
            t2->color = node_T::COLOR_BLACK;
            t2r->color = node_T::COLOR_BLACK;
          } else {
            this->rebel(t1);
            balance_maintain(t1, false);
          }
        } else {                                          // node 左子右子都是红色.
          this->rebel(t1);
          t1->color = node_T::COLOR_BLACK;
        }
      }
    } else recursive_delete(node);
  }


public:
  RB_tree_t() = default;
  RB_tree_t(typename base_type::comparer_type cmp)
    : base_type(cmp) {}
  RB_tree_t(typename base_type::comparer_type cmp,
        typename base_type::equaler_type  eql)
    : base_type(cmp, eql) {}

  node_T *push(const T& val) {
    node_T *newnode = dynamic_cast<base_type *>(this)->push(val);
    if (!newnode) return nullptr;

    this->maintain(newnode);

    return newnode;
  }

  node_T *push(T &&val) {
    std::cout << "RB_tree_t::push(T &&val) begin" << std::endl;

    node_T *newnode = dynamic_cast<base_type *>(this)->push(std::move(val));

    std::cout << "RB_tree_t::push() OK, now will maintain...." << std::endl;

    if (!newnode) return nullptr;

    std::cout << "Will maintain..." << std::endl;
    this->maintain(newnode);

    std::cout << "Maintain OK." << std::endl;

    return newnode;
  }

  bool erase(node_T *node) {
    if (node == nullptr) return false;
    if (node == this->root) {
      this->delete_root();
      return true;
    }

    if (node->get_degree() == 0)        this->delete_degree_0(node);
    else if (node->get_degree() == 1)   this->delete_degree_1(node);
    else                                this->delete_degree_2(node);
    return true;
  }
  
  bool remove(T val) {
    node_T *node = this->search_value(val);
    return this->erase(node);
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
