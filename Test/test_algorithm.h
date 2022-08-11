#include <gtest/gtest.h>
#include <algorithm>
#include <functional>
#include <numeric>
#include <list>
#include "../Src/algobase.h"
#include "../Src/numeric.h"
#include "../Src/heap_algo.h"
#include "../Src/set_algo.h"


namespace my_stl
{
namespace stl_test
{
namespace algorithm_test
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

    TEST(AlgorithmTest, copy_test) 
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
        int exp[5], act[5];
        std::copy(arr1, arr1 + 5, exp);
        my_stl::copy(arr1, arr1 + 5, act);
        EXPECT_CON_EQ(exp, act, 5);

        std::copy(arr1 + 5, arr1 + 10, exp);
        std::copy(arr1 + 5, arr1 + 10, act);
        EXPECT_CON_EQ(exp, act, 5);

    }
    TEST(AlgorithmTest, copy_backward_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        std::vector<int> exp{ 0,0,0,0,0,6,7,8,9,10 };
        int _act[] = { 0,0,0,0,0,6,7,8,9,10 };
        my_stl::vector<int> act(_act, _act + 10);
        std::copy_backward(arr1, arr1 + 5, exp.begin() + 5);
        my_stl::copy_backward(arr1, arr1 + 5, act.begin() + 5);
        EXPECT_CON_EQ(exp, act, exp.size());
        std::copy_backward(exp.begin(), exp.begin() + 8, exp.begin() + 9);
        my_stl::copy_backward(act.begin(), act.begin() + 8, act.begin() + 9);
        EXPECT_CON_EQ(exp, act, exp.size());
    }
    bool is_odd(int i) { return i & 1; };
    bool is_even(int i) { return !(i & 1); }
    TEST(AlgorithmTest, copy_if_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
        int exp[5], act[5];
        std::copy_if(arr1, arr1 + 10, exp, is_odd);
        my_stl::copy_if(arr1, arr1 + 10, act, is_odd);
        EXPECT_CON_EQ(exp, act, 5);
        std::copy_if(arr1, arr1 + 10, exp, is_even);
        my_stl::copy_if(arr1, arr1 + 10, act, is_even);
        EXPECT_CON_EQ(exp, act, 5);
    }
    TEST(AlgorithmTest, copy_n_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
        int exp[10], act[10];
        std::copy_n(arr1, 10, exp);
        my_stl::copy_n(arr1, 10, act);
        EXPECT_CON_EQ(exp, act, 5);
        std::copy_n(arr1 + 5, 5, exp);
        my_stl::copy_n(arr1 + 5, 5, act);
        EXPECT_CON_EQ(exp, act, 5);
    }
    TEST(AlgorithmTest, move_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        int arr2[] = { 1,2,3,4,5 };
        int exp[5], act[5];
        std::move(arr1, arr1 + 5, exp);
        my_stl::move(arr2, arr2 + 5, act);
        EXPECT_CON_EQ(exp, act, 5);
        EXPECT_CON_EQ(arr1, arr2, 5);
    }
    TEST(AlgorithmTest, fill_n_test)
    {
        int arr1[5];
        int arr2[5];
        std::fill_n(arr1, 5, 2);
        my_stl::fill_n(arr2, 5, 2);
        EXPECT_CON_EQ(arr1, arr2, 5);
        std::fill_n(arr1 + 2, 3, 3);
        my_stl::fill_n(arr2 + 2, 3, 3);
        EXPECT_CON_EQ(arr1, arr2, 5);
        std::vector<int> v1(20);
        std::vector<int> v2(20);
        std::fill_n(v1.begin(), v1.capacity(), 5);
        my_stl::fill_n(v2.begin(), v2.capacity(), 5);
        EXPECT_CON_EQ(v1, v2, v1.size());
        std::list<int> li1(10);
        std::list<int> li2(10);
        std::fill_n(li1.begin(), li1.size(), 10);
        my_stl::fill_n(li2.begin(), li2.size(), 10);
        EXPECT_CON_EQ(li1, li2, li1.size());
    }
    TEST(AlgorithmTest, fill_test)
    {
        int arr1[5];
        int arr2[5];
        std::fill_n(arr1, 5, 2);
        my_stl::fill_n(arr2, 5, 2);
        EXPECT_CON_EQ(arr1, arr2, 5);
        std::fill_n(arr1 + 2, 3, 3);
        my_stl::fill_n(arr2 + 2, 3, 3);
        EXPECT_CON_EQ(arr1, arr2, 5);
        std::vector<int> v1(20);
        std::vector<int> v2(20);
        std::fill_n(v1.begin(), v1.capacity(), 5);
        my_stl::fill_n(v2.begin(), v2.capacity(), 5);
        EXPECT_CON_EQ(v1, v2, v1.size());
        std::list<int> li1(10);
        std::list<int> li2(10);
        std::fill_n(li1.begin(), li1.size(), 10);
        my_stl::fill_n(li2.begin(), li2.size(), 10);
        EXPECT_CON_EQ(li1, li2, li1.size());
    }

    TEST(AlgorithmTest, lexicographical_compare_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        int arr2[] = { 0,2,4,6,8 };
        int arr3[] = { 1,2,3,4,5 };
        int arr4[] = { 1,2,3,4,5,6 };
        int arr5[] = { 2,3,4 };
        EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 5, arr2, arr2 + 5),
                    my_stl::lexicographical_compare(arr1, arr1 + 5, arr2, arr2 + 5));
        EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 5, arr3, arr3 + 5),
                    my_stl::lexicographical_compare(arr1, arr1 + 5, arr3, arr3 + 5));
        EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 5, arr4, arr4 + 6, std::less<int>()),
                    my_stl::lexicographical_compare(arr1, arr1 + 5, arr4, arr4 + 6, std::less<int>()));
        EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 5, arr5, arr5 + 3, std::less<int>()),
                    my_stl::lexicographical_compare(arr1, arr1 + 5, arr5, arr5 + 3, std::less<int>()));
    }

    TEST(AlgorithmTest, mismatch_test)
    {
        int arr1[] = { 1,1,2,2,3,4,5 };
        int arr2[] = { 1,1,2,2,3,3,3 };
        int arr3[] = { 0,1,2,2,3,4,5 };
        int arr4[] = { 1,1,2,2,3,4,5 };
        auto p1 = std::mismatch(arr1, arr1 + 7, arr2);
        auto p2 = my_stl::mismatch(arr1, arr1 + 7, arr2);
        auto p3 = std::mismatch(arr1, arr1 + 7, arr3);
        auto p4 = my_stl::mismatch(arr1, arr1 + 7, arr3);
        auto p5 = std::mismatch(arr1, arr1 + 7, arr4, std::equal_to<int>());
        auto p6 = my_stl::mismatch(arr1, arr1 + 7, arr4, std::equal_to<int>());
        EXPECT_EQ(p1.first, p2.first);
        EXPECT_EQ(p1.second, p2.second);
        EXPECT_EQ(p3.first, p4.first);
        EXPECT_EQ(p3.second, p4.second);
        EXPECT_EQ(p5.first, p6.first);
        EXPECT_EQ(p5.second, p6.second);
    }

    //----------------------numeric 5个函数-------------------//
    TEST(AlgorithmTest, accumulate_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        EXPECT_EQ(std::accumulate(arr1, arr1 + 5, 0),
                    my_stl::accumulate(arr1, arr1 + 5, 0));
        EXPECT_EQ(std::accumulate(arr1, arr1 + 5, 5),
                    my_stl::accumulate(arr1, arr1 + 5, 5));
        EXPECT_EQ(std::accumulate(arr1, arr1 + 5, 0, std::minus<int>()),
                    my_stl::accumulate(arr1, arr1 + 5, 0, std::minus<int>()));
    }

    TEST(AlgorithmTest, adjacent_difference_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        int arr2[] = { 1,1,1,1,1 };
        int exp[5], act[5];
        std::adjacent_difference(arr1, arr1 + 5, exp);
        my_stl::adjacent_difference(arr1, arr1 + 5, act);
        EXPECT_CON_EQ(exp, act, 5);
        std::adjacent_difference(arr2, arr2 + 5, exp, std::minus<int>());
        my_stl::adjacent_difference(arr2, arr2 + 5, act, std::minus<int>());
        EXPECT_CON_EQ(exp, act, 5);
    }

    TEST(AlgorithmTest, inner_product_test)
    {
        int arr1[] = { 1,1,1,1,1 };
        int arr2[] = { 2,2,2,2,2 };
        int arr3[] = { 1,2,3,4,5 };
        EXPECT_EQ(std::inner_product(arr1, arr1 + 5, arr3, 0),
                    my_stl::inner_product(arr1, arr1 + 5, arr3, 0));
        EXPECT_EQ(std::inner_product(arr2, arr2 + 5, arr3, 0, std::minus<int>(), std::multiplies<int>()),
                    my_stl::inner_product(arr2, arr2 + 5, arr3, 0, std::minus<int>(), std::multiplies<int>()));
    }

    TEST(AlgorithmTest, iota_test)
    {
        int arr1[10];
        int arr2[10];
        std::iota(arr1, arr1 + 10, 1);
        my_stl::iota(arr2, arr2 + 10, 1);
        EXPECT_CON_EQ(arr1, arr2, 10);
        std::iota(arr1, arr1 + 10, -1);
        my_stl::iota(arr2, arr2 + 10, -1);
        EXPECT_CON_EQ(arr1, arr2, 10);
    }

    TEST(AlgorithmTest, partial_sum_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int exp1[5], act1[5];
        int exp2[9], act2[9];
        std::partial_sum(arr1, arr1 + 5, exp1);
        my_stl::partial_sum(arr1, arr1 + 5, act1);
        std::partial_sum(arr1, arr1 + 9, exp2);
        my_stl::partial_sum(arr1, arr1 + 9, act2);
        EXPECT_CON_EQ(exp1, act1, 5);
        EXPECT_CON_EQ(exp2, act2, 5);
        std::partial_sum(arr1, arr1 + 9, exp2, std::multiplies<int>());
        my_stl::partial_sum(arr1, arr1 + 9, act2, std::multiplies<int>());
        EXPECT_CON_EQ(exp2, act2, 9);
    }

    //--------------------heap_algo test--------------------------//
    TEST(AlgorithmTest, make_heap_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int arr2[] = { 1,2,3,4,5,6,7,8,9 };
        int arr3[] = { 2,1,6,5,4,9,8,7,6 };
        int arr4[] = { 2,1,6,5,4,9,8,7,6 };
        int arr5[] = { 1,1,2,2,3,3,4,4,5,5 };
        int arr6[] = { 1,1,2,2,3,3,4,4,5,5 };
        std::make_heap(arr1, arr1 + 9);
        my_stl::make_heap(arr2, arr2 + 9);
        std::make_heap(arr3, arr3 + 9);
        my_stl::make_heap(arr4, arr4 + 9);
        std::make_heap(arr5, arr5 + 10, std::greater<int>());
        my_stl::make_heap(arr6, arr6 + 10, std::greater<int>());
        EXPECT_CON_EQ(arr1, arr2, 9);
        EXPECT_CON_EQ(arr3, arr4, 9);
        EXPECT_CON_EQ(arr5, arr6, 10);
    }

    TEST(AlgorithmTest, push_heap_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int arr2[] = { 1,2,3,4,5,6,7,8,9 };
        std::make_heap(arr1, arr1 + 4);
        std::make_heap(arr2, arr2 + 4);
        for (int i = 4; i <= 9; ++i)
        {
            std::push_heap(arr1, arr1 + i);
            my_stl::push_heap(arr2, arr2 + i);
            EXPECT_CON_EQ(arr1, arr2, i);
        }
        int arr3[] = { 1,2,3,4,5,6,7,8,9 };
        int arr4[] = { 1,2,3,4,5,6,7,8,9 };
        std::make_heap(arr3, arr3 + 9, std::greater<int>());
        std::make_heap(arr4, arr4 + 9, std::greater<int>());
        std::push_heap(arr3, arr3 + 9, std::greater<int>());
        my_stl::push_heap(arr4, arr4 + 9, std::greater<int>());
        EXPECT_CON_EQ(arr3, arr4, 9);
    }

    TEST(AlgorithmTest, pop_heap_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int arr2[] = { 1,2,3,4,5,6,7,8,9 };
        std::make_heap(arr1, arr1 + 9);
        std::make_heap(arr2, arr2 + 9);
        for (int i = 9; i > 0; --i)
        {
            std::pop_heap(arr1, arr1 + i);
            my_stl::pop_heap(arr2, arr2 + i);
            EXPECT_CON_EQ(arr1, arr2, i);
        }
        int arr3[] = { 1,2,3,4,5,6,7,8,9 };
        int arr4[] = { 1,2,3,4,5,6,7,8,9 };
        std::make_heap(arr3, arr3 + 9, std::greater<int>());
        std::make_heap(arr4, arr4 + 9, std::greater<int>());
        std::pop_heap(arr3, arr3 + 9, std::greater<int>());
        my_stl::pop_heap(arr4, arr4 + 9, std::greater<int>());
        EXPECT_CON_EQ(arr3, arr4, 9);
    }

    TEST(AlgorithmTest, sort_heap_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int arr2[] = { 1,2,3,4,5,6,7,8,9 };
        int arr3[] = { 1,2,3,4,5,6,7,8,9 };
        int arr4[] = { 1,2,3,4,5,6,7,8,9 };
        std::make_heap(arr1, arr1 + 9);
        std::make_heap(arr2, arr2 + 9);
        std::make_heap(arr3, arr3 + 9, std::greater<int>());
        std::make_heap(arr4, arr4 + 9, std::greater<int>());
        std::sort_heap(arr1, arr1 + 9);
        my_stl::sort_heap(arr2, arr2 + 9);
        std::sort_heap(arr3, arr3 + 9, std::greater<int>());
        my_stl::sort_heap(arr4, arr4 + 9, std::greater<int>());
        EXPECT_CON_EQ(arr1, arr2, 9);
        EXPECT_CON_EQ(arr3, arr4, 9);
    }

    TEST(AlgorithmTest, set_union_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        int arr2[] = { 1,3,5,7,9 };
        int arr3[] = { 2,4,6,8,10 };
        int exp[10] = { 0 }, act[10] = { 0 };
        std::set_union(arr1, arr1 + 5, arr2, arr2 + 5, exp);
        my_stl::set_union(arr1, arr1 + 5, arr2, arr2 + 5, act);
        EXPECT_CON_EQ(exp, act, 5);
        std::set_union(arr1, arr1 + 5, arr3, arr3 + 5, exp);
        my_stl::set_union(arr1, arr1 + 5, arr3, arr3 + 5, act);
        EXPECT_CON_EQ(exp, act, 5);
        std::set_union(arr2, arr2 + 5, arr3, arr3 + 5, exp, std::less<int>());
        my_stl::set_union(arr2, arr2 + 5, arr3, arr3 + 5, act, std::less<int>());
        EXPECT_CON_EQ(exp, act, 5);
    }  

    TEST(AlgorithmTest, set_intersection_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int arr2[] = { 1,2,3,4,5,6 };
        int arr3[] = { 1,2,3 };
        int exp[9] = { 0 }, act[9] = { 0 };
        std::set_intersection(arr1, arr1 + 9, arr2, arr2 + 6, exp);
        my_stl::set_intersection(arr1, arr1 + 9, arr2, arr2 + 6, act);
        EXPECT_CON_EQ(exp, act, 9);
        std::set_intersection(arr2, arr2 + 6, arr3, arr3 + 3, exp);
        my_stl::set_intersection(arr2, arr2 + 6, arr3, arr3 + 3, act);
        EXPECT_CON_EQ(exp, act, 9);
        std::set_intersection(arr1, arr1 + 9, arr3, arr3 + 3, exp, std::less<int>());
        my_stl::set_intersection(arr1, arr1 + 9, arr3, arr3 + 3, act, std::less<int>());
        EXPECT_CON_EQ(exp, act, 9);
    }  

    TEST(AlgorithmTest, set_difference_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int arr2[] = { 1,2,3,4,5,6 };
        int arr3[] = { 1,2,3 };
        int exp[6] = { 0 }, act[6] = { 0 };
        std::set_difference(arr1, arr1 + 9, arr2, arr2 + 6, exp);
        my_stl::set_difference(arr1, arr1 + 9, arr2, arr2 + 6, act);
        EXPECT_CON_EQ(exp, act, 6);
        std::set_difference(arr2, arr2 + 6, arr3, arr3 + 3, exp);
        my_stl::set_difference(arr2, arr2 + 6, arr3, arr3 + 3, act);
        EXPECT_CON_EQ(exp, act, 6);
        std::set_difference(arr1, arr1 + 9, arr3, arr3 + 3, exp, std::less<int>());
        my_stl::set_difference(arr1, arr1 + 9, arr3, arr3 + 3, act, std::less<int>());
        EXPECT_CON_EQ(exp, act, 6);
    }  

    TEST(AlgorithmTest, set_symmetric_difference_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        int arr2[] = { 1,3,5,7,9 };
        int arr3[] = { 2,4,6,8,10 };
        int exp[10] = { 0 }, act[10] = { 0 };
        std::set_symmetric_difference(arr1, arr1 + 5, arr2, arr2 + 5, exp);
        my_stl::set_symmetric_difference(arr1, arr1 + 5, arr2, arr2 + 5, act);
        EXPECT_CON_EQ(exp, act, 10);
        std::set_symmetric_difference(arr1, arr1 + 5, arr3, arr3 + 5, exp);
        my_stl::set_symmetric_difference(arr1, arr1 + 5, arr3, arr3 + 5, act);
        EXPECT_CON_EQ(exp, act, 10);
        std::set_symmetric_difference(arr2, arr2 + 5, arr3, arr3 + 5, exp, std::less<int>());
        my_stl::set_symmetric_difference(arr2, arr2 + 5, arr3, arr3 + 5, act, std::less<int>());
        EXPECT_CON_EQ(exp, act, 10);
    }  
} // algorithm_test
} // stl_test
} // my_stl