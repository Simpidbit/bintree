#include <iostream>
#include <utility>

template <typename T>
class tt {
  protected:
    T val;

  public:
    tt(T &&v) {
      this->val = std::move(v);
    }

    tt(const T &v) {
      this->val = v;
    }

    void print() { std::cout << val.first << ", " << val.second << std::endl; }
};

int main()
{
  std::pair<int, int> p {5, 50};

  tt<decltype(p)> *t = new tt<decltype(p)>(std::move(p));

  t->print();

  return 0;
}
