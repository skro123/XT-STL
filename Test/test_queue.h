#include <gtest/gtest.h>

#include <queue>
#include "queue.h"
#include "util.h"

namespace my_stl
{
namespace stl_test
{
namespace list_queue
{
        // 比较两个容器
    template <class Tp, class Up, class Size>
    void __expect_con_eq(Tp& container1, Up& container2, Size n)
    {
        auto first1 = container1.begin();
        auto first2 = container2.begin();
        for( ; n > 0; --n, ++first1, ++first2)
        {
            EXPECT_EQ(*first1, *first2);
        }
    }
    // 比较原生数组
    template <class Tp, class Up, class Size>
    void __expect_con_eq(Tp* a, Up* b, Size n)
    {
        for(Size i = 0; i < n; ++i)
        {
            EXPECT_EQ(a[i], b[i]);
        }
    }
    template <class T, class U, class Size>
    void EXPECT_CON_EQ(T& a, U& b, Size n)
    {
        __expect_con_eq(a, b, n);
    }
    // 构造函数测试  这里并没有与std::queue 比较 因为很多构造函数或者不支持T-T
    TEST(QueueTest, Constructor) 
    {
        int a[] = { 1,2,3,4,5 };
        // 默认构造函数
        my_stl::queue<int> md1;
        std::queue<int> sd1;
        // 指定值和大小的构造函数
        my_stl::queue<int> md2(10, 2);
        // 范围构造函数
        my_stl::queue<int> md3(a, a+5);

    }

    // 赋值函数测试
    TEST(QueueTest, AssignOperator) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::queue<int> m1(a, a+5);
        my_stl::queue<int> m2, m3, m4;
        // 拷贝赋值
        m2 = m1;
    }

    // // 迭代器测试 begin() end() ++ *
    // TEST(QueueTest, Iterator) 
    // {
    //     int a[] = { 1,2,3,4,5 };
    //     my_stl::queue<int> md1(a, a + 5);
    //     int i = 0;
    //     for(auto it = md1.begin(); it != md1.end(); ++it,++i)
    //     {
    //         EXPECT_EQ(a[i], *(it));
    //     }
    // }

    // 容量相关测试
    TEST(QueueTest, Capacity) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::queue<int> md1(a, a + 5);
        EXPECT_EQ(5, md1.size());
        EXPECT_EQ(false, md1.empty());
        my_stl::queue<int> md2;
        EXPECT_EQ(0, md2.size());
        EXPECT_EQ(true, md2.empty());        
    }

    // 元素访问相关测试
    TEST(QueueTest, ElementAccess) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::queue<int> md1(a, a + 5);
        const my_stl::queue<int> md2(a, a+5);
        EXPECT_EQ(1, md1.front());
        EXPECT_EQ(1, md2.front());        
        EXPECT_EQ(5, md1.back());
        EXPECT_EQ(5, md2.back());
    }

    // 容器修改相关测试
    TEST(QueueTest, Push) 
    {
        int a[] = { 1,2,3,4,5,5,4,3,2,1};
        my_stl::queue<int> m1;
        for(int i = 0; i < 10; ++i)
        {
            m1.push(a[i]);
            EXPECT_EQ(a[i], m1.back());
            EXPECT_EQ(a[0], m1.front());
        }
    }

    // 容器修改相关测试
    TEST(QueueTest, Pop) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::queue<int> m1(a, a + 5);
        for(int i = 0; i < 4; ++i)
        {
            EXPECT_EQ(a[i], m1.front());
            EXPECT_EQ(a[4], m1.back());
            m1.pop();
        }
        EXPECT_EQ(false, m1.empty());
        EXPECT_EQ(a[4], m1.front());
        m1.pop();
        EXPECT_EQ(true, m1.empty());
    }
}
}
}


