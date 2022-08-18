#include<gtest/gtest.h>

#include <set>

#include "../Src/set.h"

namespace my_stl
{
namespace stl_test
{
namespace set_test
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
    TEST(SetTest, Constructor) 
    {

        // 默认构造函数
        my_stl::set<int, my_stl::less<int>> ms1;
        std::set<int, std::less<int>> ss1;
        for(int i = 0; i < 10; ++i)
        {
            ms1.insert(i);
            ss1.insert(i);
        }
        EXPECT_CON_EQ(ms1, ss1, ms1.size());
        EXPECT_EQ(10, ms1.size());

        // 迭代器范围构造函数
        int arr1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        my_stl::set<int, my_stl::less<int>> ms2(arr1, arr1 + 10);
        std::set<int, std::less<int>> ss2(arr1, arr1 + 10);
        EXPECT_EQ(*ms2.begin(), *ss2.begin());

        // 初始化列表构造
        my_stl::set<int, my_stl::less<int>> ms3{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        std::set<int, std::less<int>> ss3{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        EXPECT_EQ(*ms3.begin(), *ss3.begin());

        // 拷贝构造
        my_stl::set<int, my_stl::less<int>> ms4(ms1);
        std::set<int, std::less<int>> ss4(ss1);
        EXPECT_CON_EQ(ms4, ss4, ms1.size());

        // 移动构造
        my_stl::set<int, my_stl::less<int>> ms5(std::move(ms1));
        std::set<int, std::less<int>> ss5(std::move(ss1));
        EXPECT_CON_EQ(ms5, ss5, ms5.size());
        EXPECT_EQ(10, ms5.size());        

        // 赋值
        my_stl::set<int, my_stl::less<int>> ms6;
        std::set<int, std::less<int>> ss6;
        ms6 = ms5;
        ss6 = ss5;
        EXPECT_CON_EQ(ms6, ss6, ms6.size());
        EXPECT_EQ(10, ms6.size());

        // 移动赋值
        my_stl::set<int, my_stl::less<int>> ms7;
        std::set<int, std::less<int>> ss7;
        ms7 = std::move(ms6);
        ss7 = std::move(ss6);
        EXPECT_CON_EQ(ms7, ss7, ms7.size());
        EXPECT_EQ(10, ms7.size());

        EXPECT_EQ(0, ms6.size()); // 0
        EXPECT_EQ(0, ss6.size()); // 0

        // 初始化列表复制
        std::set<int, std::less<int>> ss8 = {1, 2, 3, 4, 5, 6, 7};
        my_stl::set<int, my_stl::less<int>> ms8 = {1, 2, 3, 4, 5, 6, 7};
        EXPECT_CON_EQ(ms8, ss8, ms8.size());
    }

    // 插入删除测试
    TEST(SetTest, Insert) 
    {
        my_stl::set<int, my_stl::less<int>> ms1;
        std::set<int, std::less<int>> ss1;
        for(int i = 0; i < 10; ++i)
        {
            ms1.insert(i);
            ms1.insert(i); // 重复插入
            ss1.insert(i);
            ss1.insert(i); // 重复插入
        }
        EXPECT_CON_EQ(ms1, ss1, ms1.size());
        EXPECT_EQ(10, ms1.size());
    }

}



}



}