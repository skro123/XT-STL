#include <gtest/gtest.h>
#include <algorithm>
#include <functional>
#include <numeric>
#include <list>
#include "../Src/algo.h"
#include "../Src/algobase.h"
#include "../Src/numeric.h"
#include "../Src/heap_algo.h"
#include "../Src/set_algo.h"
#include "vector.h"

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

    //-------------------algo.h------------------------------------//

    TEST(AlgorithmTest, adjacent_find)
    {
        int arr1[] = { 1,2,3,3,4 };
        int arr2[] = { 1,2,3,4,5 };
        EXPECT_EQ(std::adjacent_find(arr1, arr1 + 5),
                    my_stl::adjacent_find(arr1, arr1 + 5));
        EXPECT_EQ(std::adjacent_find(arr2, arr2 + 5),
                    my_stl::adjacent_find(arr2, arr2 + 5));
        EXPECT_EQ(std::adjacent_find(arr1, arr1 + 5, std::greater<int>()),
                    my_stl::adjacent_find(arr1, arr1 + 5, std::greater<int>()));
    }  

    TEST(AlgorithmTest, count_test)
    {
        int arr1[] = { 1,2,2,3,3,3,4,5,8 };
        EXPECT_EQ(std::count(arr1, arr1 + 9, 2),
                    my_stl::count(arr1, arr1 + 9, 2));
        EXPECT_EQ(std::count(arr1, arr1 + 9, 3),
                    my_stl::count(arr1, arr1 + 9, 3));
        EXPECT_EQ(std::count(arr1, arr1 + 9, 6),
                    my_stl::count(arr1, arr1 + 9, 6));   
    }  
    TEST(AlgorithmTest, count_if_test)
    {
        int arr1[] = { 1,2,2,3,3,3,4,5,8 };
        EXPECT_EQ(std::count_if(arr1, arr1 + 9, is_odd),
                    my_stl::count_if(arr1, arr1 + 9, is_odd));
        EXPECT_EQ(std::count_if(arr1, arr1 + 9, is_even),
                    my_stl::count_if(arr1, arr1 + 9, is_even));
    } 
    TEST(AlgorithmTest, search_test)
    {
        int arr1[] = { 1,2,3,3,3,4,5,6,6, };
        int arr2[] = { 1 };
        int arr3[] = { 3,3 };
        int arr4[] = { 5,6,6,6 };
        EXPECT_EQ(std::search(arr1, arr1 + 9, arr2, arr2 + 1),
                    my_stl::search(arr1, arr1 + 9, arr2, arr2 + 1));
        EXPECT_EQ(std::search(arr1, arr1 + 9, arr3, arr3 + 2),
                    my_stl::search(arr1, arr1 + 9, arr3, arr3 + 2));
        EXPECT_EQ(std::search(arr1, arr1 + 9, arr4, arr4 + 3, std::less<int>()),
                    my_stl::search(arr1, arr1 + 9, arr4, arr4 + 3, std::less<int>()));
        EXPECT_EQ(std::search(arr1, arr1 + 9, arr4, arr4 + 4, std::less<int>()),
                    my_stl::search(arr1, arr1 + 9, arr4, arr4 + 4, std::less<int>()));
    } 
    TEST(AlgorithmTest, search_n_test)
    {
        int arr1[] = { 1,2,2,3,3,3,6,6,9 };
        EXPECT_EQ(std::search_n(arr1, arr1 + 9, 1, 0),
                    my_stl::search_n(arr1, arr1 + 9, 1, 0));
        EXPECT_EQ(std::search_n(arr1, arr1 + 9, 2, 2),
                    my_stl::search_n(arr1, arr1 + 9, 2, 2));
        EXPECT_EQ(std::search_n(arr1, arr1 + 9, 1, 3),
                    my_stl::search_n(arr1, arr1 + 9, 1, 3));
        EXPECT_EQ(std::search_n(arr1, arr1 + 9, 3, 6, std::less<int>()),
                    my_stl::search_n(arr1, arr1 + 9, 3, 6, std::less<int>()));
        EXPECT_EQ(std::search_n(arr1, arr1 + 9, 2, 10, std::less<int>()),
                    my_stl::search_n(arr1, arr1 + 9, 2, 10, std::less<int>()));
    } 
    TEST(AlgorithmTest, find_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        EXPECT_EQ(std::find(arr1, arr1 + 5, 3), my_stl::find(arr1, arr1 + 5, 3));
        EXPECT_EQ(std::find(arr1, arr1 + 5, 6), my_stl::find(arr1, arr1 + 5, 6));
    } 
    TEST(AlgorithmTest, find_if_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        EXPECT_EQ(std::find_if(arr1, arr1 + 5, is_odd),my_stl::find_if(arr1, arr1 + 5, is_odd));
        EXPECT_EQ(std::find_if(arr1, arr1 + 5, is_even),my_stl::find_if(arr1, arr1 + 5, is_even));
    } 
    TEST(AlgorithmTest, find_end_test)
    {
        int arr1[] = { 1,2,3,2,2,3,4,5 };
        int arr2[] = { 2,3 };
        int arr3[] = { 4,5,6 };
        EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr2, arr2 + 1),
                    my_stl::find_end(arr1, arr1 + 8, arr2, arr2 + 1));
        EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr3, arr3 + 2),
                    my_stl::find_end(arr1, arr1 + 8, arr3, arr3 + 2));
        EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr3, arr3 + 2),
                    my_stl::find_end(arr1, arr1 + 8, arr3, arr3 + 2));
        EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr3, arr3, std::less<int>()),
                    my_stl::find_end(arr1, arr1 + 8, arr3, arr3, std::less<int>()));
    } 
    TEST(AlgorithmTest, find_first_of_test)
    {
        int arr1[] = { 1,2,3,4,5 };
        int arr2[] = { 2,3,4 };
        int arr3[] = { 6,7,8 };
        EXPECT_EQ(std::find_first_of(arr1, arr1 + 5, arr2, arr2 + 3),
                    my_stl::find_first_of(arr1, arr1 + 5, arr2, arr2 + 3));
        EXPECT_EQ(std::find_first_of(arr1, arr1 + 5, arr3, arr3 + 3, std::equal_to<int>()),
                    my_stl::find_first_of(arr1, arr1 + 5, arr3, arr3 + 3, std::equal_to<int>()));
    } 
    int for_each_sum = 0;
    void arr_sum(int i) { for_each_sum += i; }
    TEST(AlgorithmTest, for_each_test)
    {
        std::vector<int> v1{ 1,2,3,4,5 };
        std::for_each(v1.begin(), v1.end(), arr_sum);
        EXPECT_EQ(15, for_each_sum);
        my_stl::for_each(v1.begin(), v1.end(), arr_sum);
        EXPECT_EQ(30, for_each_sum);
    } 
    int  gen() { return 5; }
    TEST(AlgorithmTest, generate_test)
    {
        int arr1[5];
        int arr2[5];
        std::generate(arr1, arr1 + 5, gen);
        my_stl::generate(arr2, arr2 + 5, gen);
        EXPECT_CON_EQ(arr1, arr2, 5);
    }
    TEST(AlgorithmTest, generate_n_test)
    {
        int arr1[] = { 1,1,1,1,1,6,7,8,9,10 };
        int arr2[] = { 2,2,2,2,2,6,7,8,9,10 };
        std::generate_n(arr1, 5, gen);
        my_stl::generate_n(arr2, 5, gen);
        EXPECT_CON_EQ(arr1, arr2, 5);
    }
    TEST(AlgorithmTest, includes_test)
    {
        std::vector<int> v1{ 1,2,3,4,5,6,7,8,9 };
        std::vector<int> v2{ 2,3,5,6,9 };
        std::vector<int> v3{ 0,1,2,3,4 };
        std::vector<int> v4{ 1,2,5,7,10 };
        std::vector<int> v5;
        EXPECT_EQ(std::includes(v1.begin(), v1.end(), v2.begin(), v2.end()),
                    my_stl::includes(v1.begin(), v1.end(), v2.begin(), v2.end()));
        EXPECT_EQ(std::includes(v1.begin(), v1.end(), v3.begin(), v3.end()),
                    my_stl::includes(v1.begin(), v1.end(), v3.begin(), v3.end()));
        EXPECT_EQ(std::includes(v1.begin(), v1.end(), v4.begin(), v4.end()),
                    my_stl::includes(v1.begin(), v1.end(), v4.begin(), v4.end()));
        EXPECT_EQ(std::includes(v1.begin(), v1.end(), v5.begin(), v5.end(), std::less<int>()),
                    my_stl::includes(v1.begin(), v1.end(), v5.begin(), v5.end(), std::less<int>()));
    }
    TEST(AlgorithmTest, max_elememt_test)
    {
        int arr1[] = { 1,2,3,4,5,4,3,2,1 };
        double arr2[] = { 1.0,2.2,6.6,8.8,8.81,2.4 };
        EXPECT_EQ(std::max_element(arr1, arr1 + 9),
                        my_stl::max_element(arr1, arr1 + 9));
        EXPECT_EQ(std::max_element(arr2, arr2 + 6, std::less<double>()),
                        my_stl::max_element(arr2, arr2 + 6, std::less<double>()));
    }
    TEST(AlgorithmTest, min_elememt_test)
    {
        int arr1[] = { 1,2,3,4,5,4,3,2,1 };
        double arr2[] = { 1.0,2.2,6.6,8.8,8.81,2.4 };
        EXPECT_EQ(std::min_element(arr1, arr1 + 9),
                        my_stl::min_elememt(arr1, arr1 + 9));
        EXPECT_EQ(std::min_element(arr2, arr2 + 6, std::less<double>()),
                        my_stl::min_elememt(arr2, arr2 + 6, std::less<double>()));
    }
    TEST(AlgorithmTest, merge_test)
    {
        int arr1[] = { 1,2,5,9,10 };
        int arr2[] = { 3,7,8,8,9 };
        int arr3[] = { 1,2,5,9,10 };
        int arr4[] = { 3,7,8,8,9 };
        int exp[10], act[10];
        std::merge(arr1, arr1 + 5, arr2, arr2 + 5, exp);
        my_stl::merge(arr3, arr3 + 5, arr4, arr4 + 5, act);
        EXPECT_CON_EQ(exp, act, 10);
        std::merge(arr1, arr1 + 5, arr2, arr2 + 5, exp, std::less<int>());
        my_stl::merge(arr3, arr3 + 5, arr4, arr4 + 5, act, std::less<int>());
        EXPECT_CON_EQ(exp, act, 10);
    }
    TEST(AlgorithmTest, partition_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int arr2[] = { 1,2,3,4,5,6,7,8,9 };
        std::partition(arr1, arr1 + 9, is_odd);
        my_stl::partition(arr2, arr2 + 9, is_odd);
        EXPECT_CON_EQ(arr1, arr2, 9);
        std::partition(arr1, arr1 + 9, is_even);
        my_stl::partition(arr2, arr2 + 9, is_even);
        EXPECT_CON_EQ(arr1, arr2, 9);
    }  
    TEST(AlgorithmTest, remove_test)
    {
        std::vector<int> v1{ 1,2,3,4,5,6,6,6 };
        std::vector<int> v2(v1);
        std::remove(v1.begin(), v1.end(), 3);
        my_stl::remove(v2.begin(), v2.end(), 3);
        EXPECT_CON_EQ(v1, v2, sizeof(v1)/sizeof(int));
        std::remove(v1.begin(), v1.end(), 6);
        my_stl::remove(v2.begin(), v2.end(), 6);
        EXPECT_CON_EQ(v1, v2, sizeof(v1)/sizeof(int));
    }   
    TEST(AlgorithmTest, remove_copy_test)
    {
        int arr1[] = { 1,2,3,4,5,6,6,6 };
        int arr2[] = { 1,2,3,4,5,6,6,6 };
        int exp[5], act[5];
        std::remove_copy(arr1, arr1 + 8, exp, 6);
        my_stl::remove_copy(arr2, arr2 + 8, act, 6);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
        std::remove_copy(arr1, arr1 + 5, exp, 3);
        my_stl::remove_copy(arr2, arr2 + 5, act, 3);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
    }   
    TEST(AlgorithmTest, remove_copy_if_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
        int exp[5], act[5];
        std::remove_copy_if(arr1, arr1 + 10, exp, is_odd);
        my_stl::remove_copy_if(arr1, arr1 + 10, act, is_odd);
        EXPECT_CON_EQ(exp, act, 5);
        std::remove_copy_if(arr1, arr1 + 10, exp, is_even);
        my_stl::remove_copy_if(arr1, arr1 + 10, act, is_even);
        EXPECT_CON_EQ(exp, act, 5);
    }   
    TEST(AlgorithmTest, remove_if_test)
    {
        std::vector<int> v1{ 1,2,3,4,5,6,7,8,9,10 };
        std::vector<int> v2(v1);
        std::remove_if(v1.begin(), v1.end(), is_odd);
        my_stl::remove_if(v2.begin(), v2.end(), is_odd);
        EXPECT_CON_EQ(v1, v2, sizeof(v1)/sizeof(int));
    }  
    TEST(AlgorithmTest, replace_test)
    {
        int arr1[] = { 1,1,1,2,2,2,3,3,3 };
        int arr2[] = { 1,1,1,2,2,2,3,3,3 };
        std::replace(arr1, arr1 + 9, 1, 4);
        my_stl::replace(arr2, arr2 + 9, 1, 4);
        EXPECT_CON_EQ(arr1, arr2, sizeof(arr1)/sizeof(int));
        std::replace(arr1, arr1 + 9, 2, 5);
        my_stl::replace(arr2, arr2 + 9, 2, 5);
        EXPECT_CON_EQ(arr1, arr2, sizeof(arr1)/sizeof(int));
        std::replace(arr1, arr1 + 9, 3, 6);
        my_stl::replace(arr2, arr2 + 9, 3, 6);
        EXPECT_CON_EQ(arr1, arr2, sizeof(arr1)/sizeof(int));
    } 
    TEST(AlgorithmTest, replace_if_test)
    {
        std::vector<int> v1{ 1,2,3,4,5,6,7,8,9,10 };
        std::vector<int> v2(v1);
        std::replace_if(v1.begin(), v1.end(), is_odd, 1);
        my_stl::replace_if(v2.begin(), v2.end(), is_odd, 1);
        EXPECT_CON_EQ(v1, v2, v1.size());
        std::replace_if(v1.begin(), v1.end(), is_even, 2);
        my_stl::replace_if(v2.begin(), v2.end(), is_even, 2);
        EXPECT_CON_EQ(v1, v2, v1.size());
    }  
    TEST(AlgorithmTest, replace_copy_test)
    {
        int arr1[] = { 1,1,1,2,2,2,3,3,3 };
        int exp[9], act[9];
        std::replace_copy(arr1, arr1 + 9, exp, 1, 4);
        my_stl::replace_copy(arr1, arr1 + 9, act, 1, 4);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
        std::replace_copy(arr1, arr1 + 9, exp, 2, 5);
        my_stl::replace_copy(arr1, arr1 + 9, act, 2, 5);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
        std::replace_copy(arr1, arr1 + 9, exp, 3, 6);
        my_stl::replace_copy(arr1, arr1 + 9, act, 3, 6);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
    }  
    TEST(AlgorithmTest, replace_copy_if_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
        int exp[10] = { 0 }, act[10] = { 0 };
        std::replace_copy_if(arr1, arr1 + 10, exp, is_odd, 1);
        my_stl::replace_copy_if(arr1, arr1 + 10, act, is_odd, 1);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
        std::replace_copy_if(arr1, arr1 + 10, exp, is_even, 2);
        my_stl::replace_copy_if(arr1, arr1 + 10, act, is_even, 2);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
    }    
    TEST(AlgorithmTest, reverse_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int arr2[] = { 1,2,3,4,5,6,7,8,9 };
        std::reverse(arr1, arr1 + 9);
        my_stl::reverse(arr2, arr2 + 9);
        EXPECT_CON_EQ(arr1, arr2, sizeof(arr1)/sizeof(int));
        std::reverse(arr1, arr1 + 5);
        my_stl::reverse(arr2, arr2 + 5);
        EXPECT_CON_EQ(arr1, arr2, sizeof(arr1)/sizeof(int));
    }    
    TEST(AlgorithmTest, reverse_copy_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int exp[5], act[5];
        std::reverse_copy(arr1, arr1 + 5, exp);
        my_stl::reverse_copy(arr1, arr1 + 5, act);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
        std::reverse_copy(arr1 + 4, arr1 + 9, exp);
        my_stl::reverse_copy(arr1 + 4, arr1 + 9, act);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
    }   
    TEST(AlgorithmTest, rotate_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int arr2[] = { 1,2,3,4,5,6,7,8,9 };
        std::rotate(arr1, arr1 + 3, arr1 + 9);
        my_stl::rotate(arr2, arr2 + 3, arr2 + 9);
        EXPECT_CON_EQ(arr1, arr2, sizeof(arr1)/sizeof(int));
        std::rotate(arr1 + 3, arr1 + 5, arr1 + 9);
        my_stl::rotate(arr2 + 3, arr2 + 5, arr2 + 9);
        EXPECT_CON_EQ(arr1, arr2, sizeof(arr1)/sizeof(int));
        std::rotate(arr1, arr1 + 9, arr1 + 9);
        my_stl::rotate(arr2, arr2 + 9, arr2 + 9);
        EXPECT_CON_EQ(arr1, arr2, sizeof(arr1)/sizeof(int));
    }  
    TEST(AlgorithmTest, rotate_copy_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int exp[9], act[9];
        std::rotate_copy(arr1, arr1 + 5, arr1 + 9, exp);
        my_stl::rotate_copy(arr1, arr1 + 5, arr1 + 9, act);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
        std::rotate_copy(arr1, arr1, arr1 + 9, exp);
        my_stl::rotate_copy(arr1, arr1, arr1 + 9, act);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
        std::rotate_copy(arr1, arr1 + 9, arr1 + 9, exp);
        my_stl::rotate_copy(arr1, arr1 + 9, arr1 + 9, act);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
    }  
    TEST(AlgorithmTest, swap_ranges_test)
    {
        int arr1[] = { 4,5,6,1,2,3 };
        int arr2[] = { 4,5,6,1,2,3 };
        int arr3[] = { 1,2,3,4,5,6 };
        int arr4[] = { 1,2,3,4,5,6 };
        int arr5[] = { 1,1,1 };
        int arr6[] = { 1,1,1 };
        std::swap_ranges(arr1, arr1 + 6, arr3);
        my_stl::swap_ranges(arr2, arr2 + 6, arr4);
        std::swap_ranges(arr1, arr1 + 3, arr5);
        my_stl::swap_ranges(arr2, arr2 + 3, arr6);
        EXPECT_CON_EQ(arr1, arr2, sizeof(arr1)/sizeof(int));
        EXPECT_CON_EQ(arr1, arr2, sizeof(arr1)/sizeof(int));
    } 
    int  unary_op(const int& x) { return x + 1; }
    int  binary_op(const int& x, const int& y) { return x + y; }
    TEST(AlgorithmTest, transform_test)
    {
        int arr1[] = { 1,2,3,4,5,6,7,8,9 };
        int arr2[] = { 9,8,7,6,5,4,3,2,1 };
        int exp[9], act[9];
        std::transform(arr1, arr1 + 9, exp, unary_op);
        my_stl::transform(arr1, arr1 + 9, act, unary_op);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
        std::transform(arr1, arr1 + 9, arr2, exp, binary_op);
        my_stl::transform(arr1, arr1 + 9, arr2, act, binary_op);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
    } 
    TEST(AlgorithmTest, unique_test)
    {
        int arr1[] = { 1,1,1,2,2,3,4,4,5,6 };
        int arr2[] = { 1,1,1,2,2,3,4,4,5,6 };
        int arr3[] = { 1,2,3,6,6,6,8,8,9 };
        int arr4[] = { 1,2,3,6,6,6,8,8,9 };
        std::unique(arr1, arr1 + 10);
        my_stl::unique(arr2, arr2 + 10);
        std::unique(arr3, arr3 + 9, std::equal_to<int>());
        std::unique(arr4, arr4 + 9, std::equal_to<int>());
        EXPECT_CON_EQ(arr1, arr2, sizeof(arr1)/sizeof(int));
        EXPECT_CON_EQ(arr3, arr4, sizeof(arr1)/sizeof(int));
    } 
    TEST(AlgorithmTest, unique_copy_test)
    {
        int arr1[] = { 1,1,1,2,2,3,4,4,5,6 };
        int arr2[] = { 1,2,3,6,6,6,8,8,9 };
        int exp[6], act[6];
        std::unique_copy(arr1, arr1 + 10, exp);
        my_stl::unique_copy(arr1, arr1 + 10, act);
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
        std::unique_copy(arr2, arr2 + 9, exp, std::equal_to<int>());
        my_stl::unique_copy(arr2, arr2 + 9, act, std::equal_to<int>());
        EXPECT_CON_EQ(exp, act, sizeof(exp)/sizeof(int));
    } 
} // algorithm_test
} // stl_test
} // my_stl