#include "queue.h"
#include "deque.h"
#include <iostream>

using namespace std;
using namespace my_stl;

int main()
{
    queue<int, deque<int> > iqueue;
    iqueue.push(1);
    iqueue.push(2);
    iqueue.push(5);
    iqueue.push(7);
    iqueue.push(9);

    cout << iqueue.size() << endl;  // 5
    cout << iqueue.front() << endl;  // 1
    cout << iqueue.back() << endl;  // 9

    iqueue.pop(); 
    cout << iqueue.front() << endl;  // 2
    cout << iqueue.back() << endl;  // 9

    iqueue.pop();
    cout << iqueue.front() << endl;  // 5
    cout << iqueue.back() << endl;   // 9

    iqueue.pop();
    cout << iqueue.front() << endl;  // 7
    cout << iqueue.back() << endl;  // 9

    cout << "use list" << endl;
    queue<int, deque<int> > iqueue2;
    iqueue2.push(1);
    iqueue2.push(2);
    iqueue2.push(5);
    iqueue2.push(7);
    iqueue2.push(9);

    cout << iqueue2.size() << endl;  // 5
    cout << iqueue2.front() << endl;  // 1
    cout << iqueue2.back() << endl;  // 9

    iqueue2.pop(); 
    cout << iqueue2.front() << endl;  // 2
    cout << iqueue2.back() << endl;  // 9

    iqueue2.pop();
    cout << iqueue2.front() << endl;  // 5
    cout << iqueue2.back() << endl;   // 9

    iqueue2.pop();
    cout << iqueue2.front() << endl;  // 7
    cout << iqueue2.back() << endl;  // 9

    return 0;
}