
#include "stl_deque.h"

using namespace std;
using namespace my_stl;

template <class iterator>
void
print_elem(iterator begin, iterator end) {
    for(; begin != end; ++begin) {
        cout << *begin << " ";
    }
    cout << endl;
}
int main() {
    deque<int> dq(1,0);

    // 增
    for (int i = 0; i < 10; ++i)
    {
        dq.push_back(i + 1);
    }
    print_elem(dq.begin(), dq.end());
    for (int i = 0; i < 10; ++i)
    {
        dq.push_front(-(i + 1));
    }
    print_elem(dq.begin(), dq.end());

    // 随机增
    cout << "test iterator" << endl;
    cout << " + " << *(dq.begin() + 1) << endl;
    cout << " - " << *(dq.end() - 2) << endl;
    deque<int>::iterator it = dq.begin();
    it += 5;
    cout << " += " << *it << endl;
    it = dq.end();
    ptrdiff_t index = it - dq.begin();
    cout << "index:" << index << endl;
    --it;
    --it;
    it = dq.begin();
    cout<< *it << endl;
    ++it;
    cout << *it << endl;
    ++it;
    cout << *it << endl;


    dq.insert(dq.begin() + 5, 99);
    print_elem(dq.begin(), dq.end());
    deque<int>::iterator it2 = dq.end() - 5;
    cout << *it2 << endl;
    dq.insert(it2, 99);
    print_elem(dq.begin(), dq.end());
    cout << *it2 << endl;

    // 删除
    cout << "earse" << endl;
    dq.earse(it2);
    print_elem(dq.begin(), dq.end());

    // pop
    cout << "pop back" << endl;
    dq.pop_back();
    print_elem(dq.begin(), dq.end());

    cout << "pop front" << endl;
    dq.pop_front();
    print_elem(dq.begin(), dq.end());

    cout << "clear" << endl;
    dq.clear();
    print_elem(dq.begin(), dq.end());

    return 0;

}