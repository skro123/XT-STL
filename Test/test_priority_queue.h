#include <gtest/gtest.h>

#include <queue>
#include "queue.h"
#include "util.h"

namespace my_stl
{
namespace stl_test
{
namespace list_priority_queue
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
    // 构造函数测试  这里并没有与std::priority_queue 比较 因为很多构造函数或者不支持T-T
    TEST(PriorityQueueTest, Constructor) 
    {
        int a[] = { 1,2,3,4,5 };
        // 默认构造函数
        my_stl::priority_queue<int> md1;
        std::priority_queue<int> sd1;
        // 指定值和大小的构造函数
        my_stl::priority_queue<int> md2(10, 2);
        // 范围构造函数
        my_stl::priority_queue<int> md3(a, a+5);

    }

    // 赋值函数测试
    TEST(PriorityQueueTest, AssignOperator) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::priority_queue<int> m1(a, a+5);
        my_stl::priority_queue<int> m2, m3, m4;
        // 拷贝赋值
        m2 = m1;
    }

    // // 迭代器测试 begin() end() ++ *
    // TEST(PriorityQueueTest, Iterator) 
    // {
    //     int a[] = { 1,2,3,4,5 };
    //     my_stl::priority_queue<int> md1(a, a + 5);
    //     int i = 0;
    //     for(auto it = md1.begin(); it != md1.end(); ++it,++i)
    //     {
    //         EXPECT_EQ(a[i], *(it));
    //     }
    // }

    // 容量相关测试
    TEST(PriorityQueueTest, Capacity) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::priority_queue<int> md1(a, a + 5);
        EXPECT_EQ(5, md1.size());
        EXPECT_EQ(false, md1.empty());
        my_stl::priority_queue<int> md2;
        EXPECT_EQ(0, md2.size());
        EXPECT_EQ(true, md2.empty());        
    }

    // 元素访问相关测试
    TEST(PriorityQueueTest, ElementAccess) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::priority_queue<int> md1(a, a + 5);
        const my_stl::priority_queue<int> md2(a, a+5);
        EXPECT_EQ(1, md1.top());
        EXPECT_EQ(1, md2.top());        
    }

    // 容器修改相关测试
    TEST(PriorityQueueTest, Push) 
    {
        int a[] = {5,4,3,2,1};
        my_stl::priority_queue<int> m1;
        for(int i = 0; i < 10; ++i)
        {
            m1.push(a[i]);
            EXPECT_EQ(a[i], m1.top());
        }
    }

    // 容器修改相关测试
    TEST(PriorityQueueTest, Pop) 
    {
        int a[] = { 5,4,3,2,1};
        my_stl::priority_queue<int> m1(a, a + 5);
        for(int i = 0; i < 4; ++i)
        {
            EXPECT_EQ(1, m1.top());
            m1.pop();
        }
        EXPECT_EQ(false, m1.empty());
        EXPECT_EQ(1, m1.top());
        m1.pop();
        EXPECT_EQ(true, m1.empty());
    }
}
}
}


