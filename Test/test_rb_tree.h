#include<gtest/gtest.h>

#include<tree.h>

#include "../Src/tree.h"
#include "../Src/algobase.h"
#include "../Src/functional.h"
namespace my_stl
{
namespace stl_test
{
namespace rbtree_test
{
    
        // 构造函数测试
    TEST(RBTreeTest, Constructor) 
    {
        // 默认构造函数
        my_stl::rb_tree<int, my_stl::less<int>> t1;
        my_stl::rb_tree<int, my_stl::less<int>> t2(t1);
        // // 指定n个元素的构造函数
        // my_stl::vector<int> v2(10);
        // // 指定n个元素以指定值的构造函数
        // my_stl::vector<int> v3(10, 1);
        // // 迭代器范围构造
        // my_stl::vector<int> v4(a, a + 5);
        // // 拷贝构造
        // my_stl::vector<int> v5(v2);
        // // 移动构造
        // my_stl::vector<int> v6(std::move(v2));
        // // 初始化列表构造
        // my_stl::vector<int> v7{ 1,2,3,4,5,6,7,8,9 };
    }

    TEST(RBTreeTest, InsertEqual) 
    {
        // 默认构造函数
        my_stl::rb_tree<int, my_stl::less<int>> t1;

        t1.insert_equal(2);
        EXPECT_EQ(2, *(t1.begin()));
        EXPECT_EQ(2, *(--t1.end()));
        
        my_stl::rb_tree<int, my_stl::less<int>> t2;
        for(int i = 0; i < 100; ++i)
        {
            t2.insert_equal(i);
        }
        auto it = t2.begin();
        for(int i = 0; i < 100; ++i)
        {
            EXPECT_EQ(i, *(it++));
        }

        my_stl::rb_tree<int, my_stl::less<int>> t3;
        for(int i = 100; i < 0; ++i)
        {
            t3.insert_equal(i);
        }
        auto it3 = t3.begin();
        for(int i = 100; i < 0; ++i)
        {
            EXPECT_EQ(i, *(it3++));
        }
    }


}
}
}

