#include<gtest/gtest.h>

#include <unordered_set>
#include "hash_set.h"

namespace my_stl
{
namespace stl_test
{
namespace hash_set_test
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
    TEST(HashSetTest, Constructor) 
    {

        // 默认构造函数
        my_stl::hash_set<int> ms1;
        std::unordered_set<int> ss1;
        for(int i = 0; i < 10; ++i)
        {
            ms1.insert(i);
            ss1.insert(i);
        }
        for(int i = 0; i < 10; ++i)
        {
            EXPECT_EQ(ms1.count(i), ss1.count(i)); 
        }
        // 两者不能直接通过迭代器比较元素是否相等 因为只是无序容器 hash函数和hashbuckets可能有所不同
        // EXPECT_CON_EQ(ms1, ss1, ms1.size());  // failed
        EXPECT_EQ(10, ms1.size());

        // 迭代器范围构造函数
        int arr1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        my_stl::hash_set<int> ms2(arr1, arr1 + 10);
        std::unordered_set<int> ss2(arr1, arr1 + 10);
        for(int i = 0; i < 10; ++i)
        {
            EXPECT_EQ(ms2.count(i), ss2.count(i)); 
        }
    }

    TEST(HashSetTest, Capacity) 
    {
        // 迭代器范围构造函数
        int arr1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        my_stl::hash_set<int> ms1(arr1, arr1 + 10);
        std::unordered_set<int> ss1(arr1, arr1 + 10);

        EXPECT_EQ(ms1.size(), ss1.size());
        EXPECT_EQ(ms1.empty(), ss1.empty());
    }

    // 插入测试
    TEST(HashSetTest, Insert) 
    {
        my_stl::hash_set<int> ms1;
        std::unordered_set<int> ss1;
        for(int i = 0; i < 10; ++i)
        {
            ms1.insert(i);
            ms1.insert(i); // 重复插入
            ss1.insert(i);
            ss1.insert(i); // 重复插入
        }
        EXPECT_EQ(ms1.size(), ss1.size());
        for(int i = 0; i < 10; ++i)
        {
            EXPECT_EQ(ms1.count(i), ss1.count(i)); 
        }        
    }
    // find测试
    TEST(HashSetTest, find) 
    {
        my_stl::hash_set<int> ms1;
        std::unordered_set<int> ss1;
        for(int i = 0; i < 10; ++i)
        {
            ms1.insert(i);
            ms1.insert(i); // 重复插入
            ss1.insert(i);
            ss1.insert(i); // 重复插入
        }
        EXPECT_EQ(ms1.size(), ss1.size());
        for(int i = 0; i < 10; ++i)
        {
            ms1.find(i);
        }
        for(int i = 20; i < 30; ++i)
        {
            EXPECT_EQ(ms1.find(i)._M_cur, nullptr);
        }        
    }
    // 删除操作测试
    TEST(HashSetTest, Erase) 
    {
        my_stl::hash_set<int> ms1;
        std::unordered_set<int> ss1;
        for(int i = 0; i < 100; ++i)
        {
            ms1.insert(i);
            ss1.insert(i);
        }
        // 删除单个元素 逐个删除
        for(int i = 0; i < 100; ++i)
        {
            ms1.erase(i);
            ss1.erase(i);

        }
        for(int i = 0; i < 10; ++i)
        {
            EXPECT_EQ(ms1.count(i), ss1.count(i)); 
        }            
        EXPECT_EQ(ms1.empty(), true);
    
    }
}



}



}