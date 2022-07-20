
/*
* 测试Heap算法
* API   init    Pass
* API   empty   Pass
* API   size    Pass
* API   top     Pass
* API   push    Pass
* API   pop     Pass
* 
* todo: 使用的是标准库的vector 自己实现的vector有问题
* 
* made by XT
*/

#include <algorithm>
#include "queue.h"
#include <iostream>

using namespace std;
using namespace my_stl;
int main()
{
    int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
    priority_queue<int> ipq(ia, ia + 9);
    cout << "size = " << ipq.size() << endl;  // size = 9 

    for(int i = 0; i < ipq.size(); ++i)
    {
        cout << ipq.top() << " ";  // 9 9 9 9 9 9 9 9 9
    }
    cout << endl;

    ipq.push(10);
    ipq.push(6);

    while(!ipq.empty())
    {
        cout << ipq.top() << " ";  // 10 9 8 5 4 3 3 2 1 0
        ipq.pop();
    }


    cout << endl;

    return 0;
}