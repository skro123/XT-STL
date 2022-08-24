#include <gtest/gtest.h>

#include <vector>
#include "../Src/vector.h"

namespace my_stl
{
namespace stl_test
{
namespace vector_test
{
    // 比较两个容器
    template <class Tp, class Up>
    void __expect_con_eq(Tp container1, Up container2, size_t n)
    {
        auto first1 = container1.begin();
        auto first2 = container2.begin();
        for( ; n > 0; --n, ++first1, ++first2)
        {
            EXPECT_EQ(*first1, *first2);
        }
    }
    // 比较原生数组
    template <class Tp, class Up>
    void __expect_con_eq(Tp* a, Up* b, size_t n)
    {
        for(size_t i = 0; i < n; ++i)
        {
            EXPECT_EQ(a[i], b[i]);
        }
    }
    template <class T, class U>
    void EXPECT_CON_EQ(T a, U b, size_t n)
    {
        __expect_con_eq(a, b, n);
    }

    // 构造函数测试
    TEST(VectorTest, Constructor) 
    {
        int a[] = { 1,2,3,4,5 };
        // 默认构造函数
        my_stl::vector<int> v1;
        // 指定n个元素的构造函数
        my_stl::vector<int> v2(10);
        // 指定n个元素以指定值的构造函数
        my_stl::vector<int> v3(10, 1);
        // 迭代器范围构造
        my_stl::vector<int> v4(a, a + 5);
        // 拷贝构造
        my_stl::vector<int> v5(v2);
        // 移动构造
        my_stl::vector<int> v6(std::move(v2));
        // 初始化列表构造
        my_stl::vector<int> v7{ 1,2,3,4,5,6,7,8,9 };
    }
    // Todo ：如何测试析构函数？重复创建对象并析构
    // 看看内存是否能释放掉？？

    // 赋值函数测试
    TEST(VectorTest, AssignOperator) 
    {
        my_stl::vector<int> v3(10, 1);
        my_stl::vector<int> v8, v9, v10;
        // 拷贝赋值
        v8 = v3;
        // 移动赋值
        v9 = std::move(v3);
        // 初始化列表赋值
        v10 = { 1,2,3,4,5,6,7,8,9 };
    }

    // 迭代器测试
    TEST(VectorTest, Iterator) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::vector<int> v1(a, a + 5);
        EXPECT_EQ(1, *(v1.begin()));
        EXPECT_EQ(2, *(v1.begin()+1));
        EXPECT_EQ(3, *(v1.begin()+2));
        EXPECT_EQ(4, *(v1.begin()+3));
        EXPECT_EQ(5, *(v1.begin()+4));
        EXPECT_EQ(5, *(v1.end() - 1));
        // const 对象
        const my_stl::vector<int> v2(a, a + 5);
        EXPECT_EQ(1, *(v2.begin()));
        EXPECT_EQ(2, *(v2.begin()+1));
        EXPECT_EQ(3, *(v2.begin()+2));
        EXPECT_EQ(4, *(v2.begin()+3));
        EXPECT_EQ(5, *(v2.begin()+4));
        EXPECT_EQ(5, *(v2.end() - 1));
        // 返回const 目前只是测是否能用 
        // 但应该测返回的迭代器是否不可以修改
        const my_stl::vector<int> v3(a, a + 5);
        EXPECT_EQ(1, *(v3.cbegin()));
        EXPECT_EQ(2, *(v3.cbegin()+1));
        EXPECT_EQ(3, *(v3.cbegin()+2));
        EXPECT_EQ(4, *(v3.cbegin()+3));
        EXPECT_EQ(5, *(v3.cbegin()+4));
        EXPECT_EQ(5, *(v3.cend() - 1));
    }

    // 容量相关测试
    TEST(VectorTest, Capacity) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::vector<int> v1(a, a + 5);
        EXPECT_EQ(5, v1.size());
        EXPECT_EQ(4611686018427387903, v1.max_size());
        EXPECT_EQ(false, v1.empty());
        EXPECT_EQ(16, v1.capacity());
        my_stl::vector<int> v2;
        EXPECT_EQ(true, v2.empty());
    }

    // 元素访问相关测试
    TEST(VectorTest, ElementAccess) 
    {
        int a[] = { 1,2,3,4,5 };
        my_stl::vector<int> v1(a, a + 5);
        EXPECT_EQ(1, v1[0]);
        EXPECT_EQ(2, v1[1]);
        EXPECT_EQ(3, v1[2]);
        EXPECT_EQ(4, v1[3]);
        EXPECT_EQ(5, v1[4]);

        EXPECT_EQ(1, v1.front());
        EXPECT_EQ(5, v1.back());

        EXPECT_EQ(3, v1.at(2));
        EXPECT_EQ(v1.begin(), v1.data());
        
        // const对象重载
        const my_stl::vector<int> v2(a, a + 5);
        EXPECT_EQ(1, v2[0]);
        EXPECT_EQ(2, v2[1]);
        EXPECT_EQ(3, v2[2]);
        EXPECT_EQ(4, v2[3]);
        EXPECT_EQ(5, v2[4]);

        EXPECT_EQ(1, v2.front());
        EXPECT_EQ(5, v2.back());

        EXPECT_EQ(3, v2.at(2));
        EXPECT_EQ(v2.begin(), v2.data());
    }

    // 容器修改相关测试
    TEST(VectorTest, Modifiers) 
    {
        int a[] = { 1,2,3,4,5 };
        std::vector<int> v1(a, a + 5);
        my_stl::vector<int> v2(a, a + 5);
        EXPECT_CON_EQ(v1, v2, v2.size());

        // push_back
        v1.push_back(6);
        v2.push_back(6);
        EXPECT_CON_EQ(v1, v2, v2.size());

        v1.push_back(10);
        v2.push_back(10);
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());

        // pop_back
        v1.pop_back();
        v2.pop_back();
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());

        // insert
        v1.insert(v1.begin(), 2, -1);
        v2.insert(v2.begin(), 2, -1);
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());

        v1.insert(v1.begin() + 2, 1, -2);
        v2.insert(v2.begin() + 2, 1, -2);
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());

        v1.insert(v1.end(), 1, -2);
        v2.insert(v2.end(), 1, -2);
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());

        // earse
        // iterator erase(iterator position)
        v1.erase(v1.begin());
        v2.erase(v2.begin());
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());
        // iterator erase(iterator first, iterator last)
        v1.erase(v1.begin() + 2, v1.end());
        v2.erase(v2.begin() + 2, v2.end());
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());

        // clear
        v1.clear();
        v2.clear();
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());

        // resize
        v1.resize(10);
        v2.resize(10);
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());
        v1.resize(20, -1);
        v2.resize(20, -1);
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());
        v1.resize(15);
        v2.resize(15);
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());

        // swap
        std::vector<int> v3(a, a + 5);
        my_stl::vector<int> v4(a, a + 5);
        EXPECT_CON_EQ(v3, v4, v4.size());
        v1.swap(v3);
        v2.swap(v4);
        EXPECT_CON_EQ(v1, v2, v2.size());
        EXPECT_EQ(v1.size(), v2.size());

    }
}
}
}


