#include <iostream>
#include "stl_list.h"

using namespace std;
using namespace my_stl;

int main() 
{
    int i;
    list<int> ilist;
    cout << "size = " << ilist.size() << endl;

    ilist.push_back(0);
    ilist.push_back(1);
    ilist.push_back(2);
    ilist.push_back(3);
    ilist.push_back(4);
    cout << "size = " << ilist.size() << endl;

    list<int>::iterator it;
    for(it = ilist.begin(); it != ilist.end(); ++it) {
        cout<< *it << " ";
    }
    cout<<endl;

    it = ilist.begin();
    ilist.insert(++it, 23);
    cout<< "size=" << ilist.size() << endl;
    cout<< *it << endl;
    for(it = ilist.begin(); it != ilist.end(); ++it) {
        cout<< *it << " ";
    }
    cout<<endl;

    if (it == ilist.end())
        cout<< "==" << endl;
    it = ++(ilist.begin());
    cout << "cur:" << *it << endl;
    cout << "erase:" << *(ilist.erase(it)) << endl;
    for(it = ilist.begin(); it != ilist.end(); ++it) {
        cout<< *it << " ";
    }
    cout<<endl;

    ilist.pop_back();
    ilist.pop_frnot();
    for(it = ilist.begin(); it != ilist.end(); ++it) {
        cout<< *it << " ";
    }
    cout<<endl;

    ilist.remove(2);
    for(it = ilist.begin(); it != ilist.end(); ++it) {
        cout<< *it << " ";
    }
    cout<<endl;

    cout << "test unique" << endl;
    ilist.push_back(4);
    ilist.push_back(2);
    ilist.push_back(4);
    ilist.push_back(4);
    ilist.push_back(4);
    for(it = ilist.begin(); it != ilist.end(); ++it) {
        cout<< *it << " ";
    }
    cout<<endl;
    ilist.unique();
    for(it = ilist.begin(); it != ilist.end(); ++it) {
        cout<< *it << " ";
    }
    cout<<endl;

    cout << "test transfer" << endl;
    list<int> jlist;
    jlist.push_back(1);
    jlist.push_back(2);
    jlist.push_back(3);
    jlist.push_back(4);
    jlist.push_back(5);
    jlist.push_back(6);

    // cout << *(++ilist.begin()) << endl;
    // cout<< *(++jlist.begin()) << endl;
    // cout << *(--jlist.end()) << endl;
    // ilist.transfer((++ilist.begin()), (++jlist.begin()), (--jlist.end()));
    // for(it = ilist.begin(); it != ilist.end(); ++it) {
    //         cout<< *it << " ";
    //     }
    // cout<<endl;

    // cout << *(ilist.begin()) << endl;
    // cout<< *(jlist.begin()) << endl;
    // cout << *(--jlist.end()) << endl;
    // ilist.transfer((ilist.begin()), (jlist.begin()), (jlist.end()));
    // for(it = ilist.begin(); it != ilist.end(); ++it) {
    //         cout<< *it << " ";
    //     }
    // cout<<endl;

    cout << *(ilist.end()) << endl;
    cout<< *(jlist.begin()) << endl;
    cout << *(--jlist.end()) << endl;
    ilist.transfer((ilist.end()), (jlist.begin()), (jlist.end()));
    for(it = ilist.begin(); it != ilist.end(); ++it) {
            cout<< *it << " ";
        }
    cout<<endl;

    list<int> klist = ilist;
    ilist.pop_back();
    for(it = klist.begin(); it != klist.end(); ++it) {
            cout<< *it << " ";
        }
    cout<<endl;

    


    return 0;
}