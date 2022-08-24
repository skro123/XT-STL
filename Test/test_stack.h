#include <gtest/gtest.h>

#include <stack>
#include "stack.h"
#include "util.h"

namespace my_stl
{
namespace stl_test
{
namespace list_stack
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
    // 构造函数测试  这里并没有与std::stack 比较 因为很多构造函数或者不支持T-T
    TEST(StackTest, Constructor) 
    {
        int a[] = { 1,2,3,4,5 };
        // 默认构造函数
        my_stl::stack<int> md1;
        std::stack<int> sd1;
        // 指定值和大小的构造函数
        my_stl::stack<int> md2(10, 2);
        // 范围构造函数
        my_stl::stack<int> md3(a, a+5);

        // 初始化列表构造
        my_stl::stack<int> m5{1,2,3,4,5};

        // 拷贝构造
        my_stl::stack<int> m6(m5);

        // 移动构造
        my_stl::stack<int> m7(my_stl::move(m6));
    }

    // 赋值函数测试
    TEST(StackTest, AssignOperator) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::stack<int> m1(a, a+5);

        std::stack<int> s2, s3, s4;
        my_stl::stack<int> m2, m3, m4;
        // 拷贝赋值
        m2 = m1;

        // 移动赋值
        m3 = my_stl::move(m1);

        // 初始化列表赋值
        m4 = {1, 2, 3, 4, 5};
    }

    // // 迭代器测试 begin() end() ++ *
    // TEST(StackTest, Iterator) 
    // {
    //     int a[] = { 1,2,3,4,5 };
    //     my_stl::stack<int> md1(a, a + 5);
    //     int i = 0;
    //     for(auto it = md1.begin(); it != md1.end(); ++it,++i)
    //     {
    //         EXPECT_EQ(a[i], *(it));
    //     }
    // }

    // 容量相关测试
    TEST(StackTest, Capacity) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::stack<int> md1(a, a + 5);
        EXPECT_EQ(5, md1.size());
        EXPECT_EQ(false, md1.empty());
        my_stl::stack<int> md2;
        EXPECT_EQ(0, md2.size());
        EXPECT_EQ(true, md2.empty());        
    }

    // 元素访问相关测试
    TEST(StackTest, ElementAccess) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::stack<int> md1(a, a + 5);
        const my_stl::stack<int> md2(a, a+5);
        EXPECT_EQ(5, md1.top());
        EXPECT_EQ(5, md2.top());
    }

    // 容器修改相关测试
    TEST(StackTest, Push) 
    {
        int a[] = { 1,2,3,4,5,5,4,3,2,1};
        my_stl::stack<int> m1;
        for(int i = 0; i < 10; ++i)
        {
            m1.push(a[i]);
            EXPECT_EQ(a[i], m1.top());
        }
    }

    // 容器修改相关测试
    TEST(StackTest, Pop) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::stack<int> m1(a, a + 5);
        for(int i = 4; i > 0; --i)
        {
            EXPECT_EQ(a[i], m1.top());
            m1.pop();
        }
        EXPECT_EQ(false, m1.empty());
        EXPECT_EQ(a[0], m1.top());
        m1.pop();
        EXPECT_EQ(true, m1.empty());

    }
}
}
}


