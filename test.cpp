#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::cin;

class testype {
public:
    int a = 10;

    testype(int x) : a(x) {}
    testype(const testype &t) {
        cout << "拷贝构造!" << endl;
        this->a = t.a;
    }
};


int main()
{
    testype t2(10);
    testype t1(20);

    t1 = testype(t2);

    cout << t1.a << endl;

    return 0;
}
