#include <iostream>
#include <utility>

template <typename T>
class node {
  protected:
    T val;

  public:
    node(T &&v) { this->val = std::move(v); }
    node(const T &v) { this->val = v; }
    void print() { std::cout << val.first << ", " << val.second << std::endl; }
};

int main()
{
  std::pair<int, int> p {5, 50};

  node<decltype(p)> *t = new node<decltype(p)>(std::move(p));

  t->print();

  return 0;
}
