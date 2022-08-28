#include<gtest/gtest.h>

#include <unordered_map>
#include "hash_map.h"

namespace my_stl
{
namespace stl_test
{
namespace hash_map_test
{
    
    // 比较两个容器
    template <class Tp, class Up>
    void __expect_con_eq(Tp& container1, Up& container2, size_t n)
    {
        auto first1 = container1.begin();
        auto first2 = container2.begin();
        for( ; n > 0; --n, ++first1, ++first2)
        {
            // EXPECT_EQ(*first1, *first2);
            EXPECT_EQ((*first1).first, (*first1).first);
            EXPECT_EQ((*first1).second, (*first1).second);

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
    void EXPECT_CON_EQ(T& a, U& b, size_t n)
    {
        __expect_con_eq(a, b, n);
    }

    TEST(HashMapTest, Constructor)
    {
        my_stl::hash_map<int, int> mm1;
        std::unordered_map<int, int> ss1;
        my_stl::vector<my_stl::pair<int, int>> mv1;
        std::vector<std::pair<int, int>> sv1;
        for(int i = 0; i < 100; ++i)
        {
            mv1.push_back(my_stl::make_pair(i, i+1));
            sv1.push_back(std::make_pair(i, i+1));
        }
        EXPECT_CON_EQ(mm1, ss1, mm1.size());
        my_stl::hash_map<int, int> mm2(mv1.begin(), mv1.end());
        std::unordered_map<int, int> ss2(sv1.begin(), sv1.end());
        EXPECT_CON_EQ(mm2, ss2, mm2.size());
    }
    TEST(HashMapTest, Capacity) 
    {
        my_stl::vector<my_stl::pair<int, int>> mv1;
        std::vector<std::pair<int, int>> sv1;        
        for(int i = 0; i < 100; ++i)
        {
            mv1.push_back(my_stl::make_pair(i, i+1));
            sv1.push_back(std::make_pair(i, i+1));
        }
        my_stl::hash_map<int, int> mm1(mv1.begin(), mv1.end());
        std::unordered_map<int, int> ss1(sv1.begin(), sv1.end());

        EXPECT_EQ(mm1.size(), ss1.size());
        EXPECT_EQ(mm1.empty(), ss1.empty());
    }
    TEST(HashMapTest, Insert)
    {
        my_stl::hash_map<int, int> mm1;
        std::unordered_map<int, int> ss1;
        for(int i = 0; i < 100; ++i)
        {
            mm1.insert(my_stl::make_pair(i,i+1));
            ss1.insert(std::make_pair(i,i+1));
        }
        for(int i = 0; i < 100; ++i)
        {
            EXPECT_EQ(mm1.count(i), ss1.count(i)); 
        }
        for(int i = 0; i < 100; ++i)
        {
            mm1.insert(my_stl::make_pair(i,i+1));
            ss1.insert(std::make_pair(i,i+1));
        }
        for(int i = 0; i < 100; ++i)
        {
            EXPECT_EQ(mm1.count(i), ss1.count(i)); 
        }

        for(int i = 0; i < 100; ++i)
        {
            mm1[i];
            ss1[i];
        }
        for(int i = 0; i < 100; ++i)
        {
            EXPECT_EQ(mm1.count(i), ss1.count(i)); 
        }
        for(int i = 100; i < 200; ++i)
        {
            mm1[i];
            ss1[i];
        }
        for(int i = 0; i < 200; ++i)
        {
            EXPECT_EQ(mm1.count(i), ss1.count(i)); 
        }
    }
    // find测试
    TEST(HashMapTest, find) 
    {
        my_stl::hash_map<int, int> mm1;
        std::unordered_map<int, int> ss1;
        for(int i = 0; i < 10; ++i)
        {
            mm1.insert(my_stl::make_pair(i,i+1));
            ss1.insert(std::make_pair(i,i+1));
        }
        EXPECT_EQ(mm1.size(), ss1.size());
        for(int i = 0; i < 10; ++i)
        {
            mm1.find(i);
        }
        for(int i = 20; i < 30; ++i)
        {
            EXPECT_EQ(mm1.find(i)._M_cur, nullptr);
        }        
    }
    TEST(HashMapTest, Erase)
    {
        my_stl::hash_map<int, int> mm1;
        std::unordered_map<int, int> ss1;
        for(int i = 0; i < 100; ++i)
        {
            mm1.insert(my_stl::make_pair(i,i+1));
            ss1.insert(std::make_pair(i,i+1));
        }
        
        for(int i = 10; i < 20; ++i)
        {
            mm1.erase(i);
            ss1.erase(i);
        }
        for(int i = 0; i < 100; ++i)
        {
            EXPECT_EQ(mm1.count(i), ss1.count(i)); 
        }   
    }
}
}
}
