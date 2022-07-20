
#include "stack.h"
#include "deque.h"
#include "list.h"
#include <iostream>

using namespace std;
using namespace my_stl;

int main()
{
    stack<int, deque<int> > istack;
    istack.push(1);
    istack.push(2);
    istack.push(5);
    istack.push(7);

    cout << istack.size() << endl;  // 4
    cout << istack.top() << endl;   // 7

    istack.pop();
    cout << istack.top() << endl;   // 5

    istack.pop();
    cout << istack.top() << endl;   // 2

    istack.pop();
    cout << istack.top() << endl;   // 1

    cout << istack.size() << endl;  // 1

    cout << "use list" << endl;
    stack<int, list<int> > istack2;
    istack2.push(1);
    istack2.push(2);
    istack2.push(5);
    istack2.push(7);

    cout << istack2.size() << endl;  // 4
    cout << istack2.top() << endl;   // 7

    istack2.pop();
    cout << istack2.top() << endl;   // 5

    istack2.pop();
    cout << istack2.top() << endl;   // 2

    istack2.pop();
    cout << istack2.top() << endl;   // 1

    cout << istack2.size() << endl;  // 1
    return 0;
}