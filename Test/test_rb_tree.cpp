#include "stl_tree.h"
#include <iostream>

using namespace std;
using namespace my_stl;

int main()
{
    rb_tree_node<int> node;
    node.left = nullptr;
    node.right = nullptr;
    node.parent = nullptr;
    node.color = rb_tree_color::red;
    node.value = 3;

    int* val = node.valptr();
    std::cout<< *val << std::endl;

    return 0;
}