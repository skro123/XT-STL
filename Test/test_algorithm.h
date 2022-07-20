#include <gtest/gtest.h>
#include <algorithm>
#include <functional>
#include <numeric>

#include "../Src/algobase.h"

namespace my_stl
{
namespace stl_test
{
namespace algorithm_test
{
    template <class T, class U>
    void expect_con_eq(T a, U b, size_t n)
    {
        for(int i = 0; i < n; ++i)
        {
            EXPECT_EQ(a[i], b[i]);
        }
    }
    TEST(AlgorithmTest, copy_test) 
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
        int exp[5], act[5];
        std::copy(arr1, arr1 + 5, exp);
        my_stl::copy(arr1, arr1 + 5, act);
        expect_con_eq(exp, act, 5);

        std::copy(arr1 + 5, arr1 + 10, exp);
        std::copy(arr1 + 5, arr1 + 10, act);
        expect_con_eq(exp, act, 5);

    }
    TEST(AlgorithmTest, copy_backward_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        std::vector<int> exp{ 0,0,0,0,0,6,7,8,9,10 };
        int act[] = { 0,0,0,0,0,6,7,8,9,10 };
        std::copy_backward(arr1, arr1 + 5, exp.begin() + 5);
        my_stl::copy_backward(arr1, arr1 + 5, act + 5);
        expect_con_eq(exp, act, exp.size());
        std::copy_backward(exp.begin(), exp.begin() + 8, exp.begin() + 9);
        my_stl::copy_backward(act, act + 8, act + 9);
        expect_con_eq(exp, act, exp.size());
    }
    bool is_odd(int i) { return i & 1; };
    bool is_even(int i) { return !(i & 1); }
    TEST(AlgorithmTest, copy_if_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
        int exp[5], act[5];
        std::copy_if(arr1, arr1 + 10, exp, is_odd);
        my_stl::copy_if(arr1, arr1 + 10, act, is_odd);
        expect_con_eq(exp, act, 5);
        std::copy_if(arr1, arr1 + 10, exp, is_even);
        my_stl::copy_if(arr1, arr1 + 10, act, is_even);
        expect_con_eq(exp, act, 5);
    }
    TEST(AlgorithmTest, copy_n_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
        int exp[10], act[10];
        std::copy_n(arr1, 10, exp);
        my_stl::copy_n(arr1, 10, act);
        expect_con_eq(exp, act, 5);
        std::copy_n(arr1 + 5, 5, exp);
        my_stl::copy_n(arr1 + 5, 5, act);
        expect_con_eq(exp, act, 5);
    }
    TEST(AlgorithmTest, move_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        int arr2[] = { 1,2,3,4,5 };
        int exp[5], act[5];
        std::move(arr1, arr1 + 5, exp);
        my_stl::move(arr2, arr2 + 5, act);
        expect_con_eq(exp, act, 5);
        expect_con_eq(arr1, arr2, 5);
    }
    TEST(AlgorithmTest, fill_n_test)
    {
        int arr1[5];
        int arr2[5];
        std::fill_n(arr1, arr1 + 5, 2);
        my_stl::fill_n(arr2, arr2 + 5, 2);
        expect_con_eq(arr1, arr2, 5);
        // int arr1[] = { 1,2,3,4,5 };
        // std::vector<int> exp{ 0,0,0,0,0,6,7,8,9,10 };
        // int act[] = { 0,0,0,0,0,6,7,8,9,10 };
        // std::copy_backward(arr1, arr1 + 5, exp.begin() + 5);
        // my_stl::copy_backward(arr1, arr1 + 5, act + 5);
        // expect_con_eq(exp, act, exp.size());
        // std::copy_backward(exp.begin(), exp.begin() + 8, exp.begin() + 9);
        // my_stl::copy_backward(act, act + 8, act + 9);
        // expect_con_eq(exp, act, exp.size());
    }
    
}
}
}