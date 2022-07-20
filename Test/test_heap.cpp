
/*
* 测试Heap算法
* API   make_heap     Pass
* API   push_heap     Pass
* API   pop_heap      Pass
* API   sort_heap     Pass
* 
* made by XT
*/

// #include "stl_vector.h"
#include <vector>
#include "stl_heap.h"
#include <iostream>

using namespace std;
using namespace my_stl;

int main()
{
    int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
    vector<int> ivec(ia, ia + 9);

    cout << "Make heap" << endl;
    make_heap(ivec.begin(), ivec.end());
    for (vector<int>::iterator it = ivec.begin(); it != ivec.end(); ++it)
    {
        cout << *it << " ";  // 9 5 8 3 4 0 2 3 1
    }
    cout << endl;

    cout << "Push heap" << endl;
    ivec.push_back(7);
    push_heap(ivec.begin(), ivec.end());    
    for (vector<int>::iterator it = ivec.begin(); it != ivec.end(); ++it)
    {
        cout << *it << " ";  // 9 7 8 3 5 0 2 3 1 4
    }
    cout << endl;

    cout << "Pop heap" << endl;
    pop_heap(ivec.begin(), ivec.end());
    cout << ivec.back() << endl;
    ivec.pop_back();
    for (vector<int>::iterator it = ivec.begin(); it != ivec.end(); ++it)
    {
        cout << *it << " ";  // 8 7 4 3 5 0 2 3 1
    }
    cout << endl;

    cout << "Sort heap" << endl;
    sort_heap(ivec.begin(), ivec.end());
    for (vector<int>::iterator it = ivec.begin(); it != ivec.end(); ++it)
    {
        cout << *it << " ";  // 0 1 2 3 3 4 5 7 8
    }
    cout << endl;

    return 0;
}