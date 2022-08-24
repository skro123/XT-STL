#include <gtest/gtest.h>

#include <deque>
#include "deque.h"
#include "util.h"

namespace my_stl
{
namespace stl_test
{
namespace list_deque
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
    // 构造函数测试
    TEST(DequeTest, Constructor) 
    {
        int a[] = { 1,2,3,4,5 };
        // 默认构造函数
        my_stl::deque<int> md1;
        std::deque<int> sd1;
        // 指定值和大小的构造函数
        my_stl::deque<int> md2(10, 2);
        std::deque<int> sd2(10, 2);
        EXPECT_EQ(sd2.size(), md2.size());
        EXPECT_CON_EQ(md2, sd2, sd2.size());
        // 范围构造函数
        my_stl::deque<int> md3(a, a+5);
        std::deque<int> sd3(a, a+5);
        EXPECT_EQ(sd3.size(), md3.size());
        EXPECT_CON_EQ(md3, sd3, sd3.size());

        // 初始化列表构造
        std::deque<int> s5{1,2,3,4,5};
        my_stl::deque<int> v5{1,2,3,4,5};
        EXPECT_CON_EQ(s5, v5, s5.size());

        // 拷贝构造
        std::deque<int> s6(s5);
        my_stl::deque<int> v6(v5);
        EXPECT_CON_EQ(s6,v6, s6.size());

        // 移动构造
        std::deque<int> s7(std::move(s6));
        my_stl::deque<int> v7(my_stl::move(v6));
        EXPECT_CON_EQ(s7,v7, s7.size()); // 注意被移动的资源不可再访问   
    }

    // 赋值函数测试
    TEST(DequeTest, AssignOperator) 
    {
        int a[] = { 1,2,3,4,5 };
        std::deque<int> s1(a, a+5);
        my_stl::deque<int> m1(a, a+5);
        EXPECT_CON_EQ(s1, m1, s1.size());

        std::deque<int> s2, s3, s4;
        my_stl::deque<int> m2, m3, m4;
        // 拷贝赋值
        s2 = s1;
        m2 = m1;
        EXPECT_EQ(s2.size(), m2.size());
        EXPECT_CON_EQ(s2, m2, s2.size());

        // 移动赋值
        s3 = std::move(s1);
        m3 = my_stl::move(m1);
        EXPECT_CON_EQ(s3, m3, s3.size());

        // 初始化列表赋值
        s4 = {1, 2, 3, 4, 5};
        m4 = {1, 2, 3, 4, 5};
        EXPECT_CON_EQ(s4, m4, s4.size());
    }

    // 迭代器测试 begin() end() ++ *
    TEST(DequeTest, Iterator) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::deque<int> md1(a, a + 5);
        int i = 0;
        for(auto it = md1.begin(); it != md1.end(); ++it,++i)
        {
            EXPECT_EQ(a[i], *(it));
        }
    }

    // 容量相关测试
    TEST(DequeTest, Capacity) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::deque<int> md1(a, a + 5);
        std::deque<int> sd1(a, a+5);
        EXPECT_EQ(sd1.size(), md1.size());
        EXPECT_EQ(sd1.max_size(), sd1.max_size());
        EXPECT_EQ(sd1.empty(), md1.empty());
    }

    // 元素访问相关测试
    TEST(DequeTest, ElementAccess) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::deque<int> md1(a, a + 5);
        std::deque<int> sd1(a, a + 5);
        EXPECT_EQ(md1.front(), sd1.front());
        EXPECT_EQ(md1.back(), sd1.back());
        for(int i = 0; i < 5; ++i)
        {
            EXPECT_EQ(md1[i], sd1[i]);
        }
    }

    // 容器修改相关测试
    TEST(DequeTest, Insert) 
    {
        int a[] = { 1,2,3,4,5 };
        std::deque<int> s1(a, a + 5);
        my_stl::deque<int> m1(a, a + 5);
        EXPECT_CON_EQ(s1, m1, s1.size());

        //insert
        auto it_s1 = s1.begin();
        auto it_m1= m1.begin();
        for(int i  = 0; i < 10; ++i)
        {
            ++it_s1;
            ++it_m1;
            s1.insert(it_s1, i);
            m1.insert(it_m1, i);
        }
        EXPECT_CON_EQ(s1, m1, s1.size());

        // push back
        for(int i = 5; i < 20; ++i)
        {
            s1.push_back(i);
            m1.push_back(i);
        }
        EXPECT_CON_EQ(s1, m1, s1.size());

        // push_front
        for(int i = 5; i < 20; ++i)
        {
            s1.push_front(i);
            m1.push_front(i);
        }
        EXPECT_CON_EQ(s1, m1, s1.size());
    }

    // 容器修改相关测试
    TEST(DequeTest, Erase) 
    {
        int a[] = { 1,2,3,4,5 };
        std::deque<int> s1(a, a + 5);
        my_stl::deque<int> m1(a, a + 5);
        EXPECT_CON_EQ(s1, m1, s1.size());

        // erase
        auto it_s1 = s1.begin();
        auto it_m1= m1.begin();
        ++it_s1;
        ++it_m1;
        s1.erase(it_s1);
        m1.erase(it_m1);
        EXPECT_CON_EQ(s1, m1, s1.size());

        // pop back
        for(int i = 0; i < 2; ++i)
        {
            s1.pop_back();
            m1.pop_back();
        }
        EXPECT_CON_EQ(s1, m1, s1.size());

        for(int i = 0; i < 2; ++i)
        {
            s1.pop_front();
            m1.pop_front();
        }
        EXPECT_CON_EQ(s1, m1, s1.size());

        s1.erase(s1.begin(), s1.end());
        m1.erase(m1.begin(), m1.end());
        EXPECT_CON_EQ(s1, m1, s1.size());
    }

    TEST(DequeTest, Clear) 
    {
        int a[] = { 1,2,3,4,5 };
        std::deque<int> s1(a, a + 5);
        my_stl::deque<int> m1(a, a + 5);
        EXPECT_CON_EQ(s1, m1, s1.size());
        s1.clear();
        m1.clear();
        EXPECT_CON_EQ(s1, m1, s1.size());
    }


}
}
}


