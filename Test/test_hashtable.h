#include<gtest/gtest.h>

#include <bits/hashtable.h>

#include "../Src/hashtable.h"
#include "../Src/algobase.h"
#include "../Src/functional.h"
namespace my_stl
{
namespace stl_test
{
namespace hashtable_test
{
    
    // 构造函数测试
    TEST(hashtableTest, Constructor) 
    {
        // 默认构造函数
        my_stl::hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc> t1(2);
        my_stl::hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc> t2(t1);
    }

    // 重复插入测试
    TEST(hashtableTest, InsertEqual) 
    {
        // 默认构造函数
        my_stl::hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc> t1(1);
        // 测试不可重复插入
        for(int i = 0; i < 100; ++i)
        {
            t1.insert_unique(i);
        }
        // 重复插入一遍
        for(int i = 0; i < 100; ++i)
        {
            t1.insert_unique(i);
        }
        // 每个键值个数应该为1
        for(int i = 0; i < 100; ++i)
        {
            EXPECT_EQ(t1.count(i), 1);
        }   
    }

    // 不重复插入测试
    TEST(hashtableTest, InsertUnique) 
    {
        // 测试可重复插入
        my_stl::hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc> t2(1);
        for(int i = 0; i < 100; ++i)
        {
            t2.insert_equal(i);
        }
        for(int i = 0; i < 100; ++i)
        {
            t2.insert_equal(i);
        }
        // 每个键值个数应该为2
        for(int i = 0; i < 100; ++i)
        {
            EXPECT_EQ(t2.count(i), 2);
        }
        // 再插入一遍应该是3 
        for(int i = 0; i < 100; ++i)
        {
            t2.insert_equal(i);
            EXPECT_EQ(t2.count(i), 3);
        }
    }

    // 查找操作测试
    TEST(hashtableTest, Find) 
    {
        my_stl::hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc> t1(1);
        for(int i = 0; i < 100; ++i)
        {
            t1.insert_unique(i);
        }
        for(int i = 0; i < 100; ++i)
        {
            t1.find(i);
        }
        // 查找不存在的元素应该返回类似于end()的迭代器
        t1.find(101);
        // 指向的节点是空的
        EXPECT_EQ(t1.find(101)._M_cur, nullptr);

    }
    // 删除操作测试
    TEST(hashtableTest, Erase) 
    {
        my_stl::hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc> t1(1);
        for(int i = 0; i < 100; ++i)
        {
            t1.insert_equal(i);
        }
        // 删除单个元素 逐个删除
        for(int i = 0; i < 100; ++i)
        {
            t1.erase(i);
        }
        EXPECT_EQ(t1.empty(), true);
       

    }
}
}
}

