#include <gtest/gtest.h>

#include <map>
#include <vector.h>
#include "../Src/map.h"
#include "../Src/vector.h"

namespace my_stl
{
namespace stl_test
{
namespace map_test
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

    TEST(MapTest, Constructor)
    {
        my_stl::map<int, int, my_stl::less<int>> mm1;
        std::map<int, int, std::less<int>> ss1;
        my_stl::vector<my_stl::pair<int, int>> mv1;
        std::vector<std::pair<int, int>> sv1;
        for(int i = 0; i < 100; ++i)
        {
            mv1.push_back(my_stl::make_pair(i, i+1));
            sv1.push_back(std::make_pair(i, i+1));
        }
        my_stl::map<int, int, my_stl::less<int>> mm2(mv1.begin(), mv1.end());
        std::map<int, int, std::less<int>> ss2(sv1.begin(), sv1.end());
        EXPECT_CON_EQ(mm2, ss2, mm2.size());
    }

    TEST(MapTest, Insert)
    {
        my_stl::map<int, int, my_stl::less<int>> mm1;
        std::map<int, int, std::less<int>> ss1;
        for(int i = 0; i < 100; ++i)
        {
            mm1.insert(my_stl::make_pair(i,i+1));
            ss1.insert(std::make_pair(i,i+1));
        }
        auto it_mm1 = mm1.begin();
        auto it_ss1 = ss1.begin();
        while(it_ss1 != ss1.end())
        {
            EXPECT_EQ((*it_mm1).first, (*it_ss1).first);
            EXPECT_EQ((*it_mm1).second, (*it_ss1).second);
            EXPECT_CON_EQ(mm1, ss1, mm1.size());
            ++it_mm1;
            ++it_ss1;
        }
    }

    TEST(MapTest, Erase)
    {
        my_stl::map<int, int, my_stl::less<int>> mm1;
        std::map<int, int, std::less<int>> ss1;
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
        auto it_mm1 = mm1.begin();
        auto it_ss1 = ss1.begin();
        while(it_ss1 != ss1.end())
        {
            EXPECT_EQ((*it_mm1).first, (*it_ss1).first);
            EXPECT_EQ((*it_mm1).second, (*it_ss1).second);
            EXPECT_CON_EQ(mm1, ss1, mm1.size());
            ++it_mm1;
            ++it_ss1;
        }
    }
}
}
}