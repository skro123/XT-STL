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
        my_stl::rb_tree<int, my_stl::identity<int>, my_stl::less<int>> t1;
        my_stl::rb_tree<int, my_stl::identity<int>, my_stl::less<int>> t2(t1);
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

    // 重复插入测试
    TEST(RBTreeTest, InsertEqual) 
    {
        // 默认构造函数
        my_stl::rb_tree<int, my_stl::identity<int>, my_stl::less<int>> t1;

        t1.insert_equal(2);
        EXPECT_EQ(2, *(t1.begin()));
        EXPECT_EQ(2, *(--t1.end()));
        
        my_stl::rb_tree<int, my_stl::identity<int>, my_stl::less<int>> t2;
        for(int i = 0; i < 100; ++i)
        {
            t2.insert_equal(i);
        }
        auto it = t2.begin();
        for(int i = 0; i < 100; ++i)
        {
            EXPECT_EQ(i, *(it++));
        }

        my_stl::rb_tree<int, my_stl::identity<int>, my_stl::less<int>> t3;
        for(int i = 100; i < 0; ++i)
        {
            t3.insert_equal(i);
        }
        auto it3 = t3.begin();
        for(int i = 100; i < 0; ++i)
        {
            EXPECT_EQ(i, *(it3++));
        }

        my_stl::rb_tree<int, my_stl::identity<int>, my_stl::less<int>> t4;
        for(int i = 0; i < 100; ++i)
        {
            t4.insert_equal(i);
            t4.insert_equal(i);
        }
        auto it4 = t4.begin();
        for(int i = 0; i < 100; ++i)
        {
            EXPECT_EQ(i, *(it4++));
            EXPECT_EQ(i, *(it4++));
        }
    }

    // 不重复插入测试
    TEST(RBTreeTest, InsertUnique) 
    {
        // 默认构造函数
        my_stl::rb_tree<int, my_stl::identity<int>, my_stl::less<int>> t1;

        t1.insert_unique(2);
        EXPECT_EQ(2, *(t1.begin()));
        EXPECT_EQ(2, *(--t1.end()));
        
        my_stl::rb_tree<int, my_stl::identity<int>, my_stl::less<int>> t2;
        for(int i = 0; i < 100; ++i)
        {
            t2.insert_unique(i);
        }
        auto it = t2.begin();
        for(int i = 0; i < 100; ++i)
        {
            EXPECT_EQ(i, *(it++));
        }

        my_stl::rb_tree<int, my_stl::identity<int>, my_stl::less<int>> t3;
        for(int i = 100; i < 0; ++i)
        {
            t3.insert_unique(i);
        }
        auto it3 = t3.begin();
        for(int i = 100; i < 0; ++i)
        {
            EXPECT_EQ(i, *(it3++));
        }

        my_stl::rb_tree<int, my_stl::identity<int>, my_stl::less<int>> t4;
        for(int i = 0; i < 100; ++i)
        {
            t4.insert_unique(i);
            t4.insert_unique(i);
        }
        auto it4 = t4.begin();
        for(int i = 0; i < 100; ++i)
        {
            // EXPECT_EQ(i, *(it4++));
            EXPECT_EQ(i, *(it4++));
        }
    }

    // 删除操作测试
    TEST(RBTreeTest, Erase) 
    {
        my_stl::rb_tree<int, my_stl::identity<int>, my_stl::less<int>> t1;
        for(int i = 0; i < 100; ++i)
        {
            t1.insert_equal(i);
        }

        // 删除单个位置
        auto it1 = t1.erase(t1.begin());
        EXPECT_EQ(*it1, 1);
        EXPECT_EQ(1, *(t1.begin()));
        t1.erase(++t1.begin());
        EXPECT_EQ(3, *(++t1.begin()));

        t1.erase((--t1.end()));
        EXPECT_EQ(98, *(--t1.end()));

        t1.erase((--(--t1.end())));
        EXPECT_EQ(96, *((--(--t1.end()))));

        // 删除指定键值的
        t1.erase(10);
        auto res = t1.find(10);
        EXPECT_EQ(t1.end(), res);

        // 迭代器范围删除
        auto first = t1.find(11);
        auto last = t1.find(20);
        t1.erase(first, last);
        EXPECT_EQ(t1.end(), t1.find(15));
        EXPECT_NE(t1.end(), t1.find(20));

    }
}
}
}

