#ifndef XT_STL_ALGO_H
#define XT_STL_ALGO_H

#include <cstddef>
#include <ctime>

#include "algobase.h"
#include "heap_algo.h"
#include "set_algo.h"
#include "functional.h"
#include "iterator.h"
namespace my_stl
{
    template <typename ForwardIterator, typename BinaryPredicate>
    ForwardIterator
    __adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred)
    {
        if(first == last) return last;
        ForwardIterator next = first;
        while(++next != last)
        {
            if(binary_pred(*first, *next)) return first;
            first = next;
        }
        return last;
    }

    /**
     * @brief 找到第一对相等的相邻元素
     * 
     * @param first 一个forward的迭代器
     * @param last 一个forward的迭代器
     * @return 返回两个相等的相邻元素的第一个元素的位置，否则是last。
     * 
     */
    template <typename ForwardIterator>
    ForwardIterator
    adjacent_find(ForwardIterator first, ForwardIterator last)
    {
        return my_stl::__adjacent_find(first, last, equal_to<typename iterator_traits<ForwardIterator>::value_type>());
    }

    /**
     * @brief 找到第一对使binary_pred为True的相邻元素
     * 
     * @param first 一个forward的迭代器
     * @param last 一个forward的迭代器
     * @return 返回两个使binary_pred为True的相邻元素的第一个元素的位置，否则是last。
     * 
     */
    template <typename ForwardIterator, typename BinaryPredicate>
    ForwardIterator
    adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred)
    {
        return my_stl::__adjacent_find(first, last, binary_pred);
    }

    template<typename Value>
    struct Iter_equals_val
    {
        Value& _M_value;
        Iter_equals_val(Value& value)
        :_M_value(value)
        { }
        template<typename Iterator>
        bool operator()(Iterator it)
        { return *it == this->_M_value; }
    };

    template <typename InputIterator, typename Tp>
    inline
    typename iterator_traits<InputIterator>::difference_type
    count(InputIterator first, InputIterator last, const Tp& value)
    {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        for(; first != last; ++first)
        {
            if(*first == value)
            {
                ++n;
            }
        }
        return n;
    }

    template <typename InputIterator, typename Predicate>
    inline
    typename iterator_traits<InputIterator>::difference_type
    count_if(InputIterator first, InputIterator last, Predicate pred)
    {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        for(; first != last; ++first)
        {
            if(pred(*first))
            {
                ++n;
            }
        }
        return n;
    }  

    //find
    template <class InputIterator, class Tp>
    InputIterator
    find(InputIterator first, InputIterator last, const Tp& value)
    {
        while (first != last && *first != value) ++first;
        return first;
    }

    //find_if
    template <class InputIterator, class UnaryPredicate>
    InputIterator
    find_if(InputIterator first, InputIterator last, UnaryPredicate unary_pred)
    {
        while (first != last && !unary_pred(*first)) ++first;
        return first;
    }

    // search
    template <class ForwardIterator1, class ForwardIterator2>
    ForwardIterator1
    search(ForwardIterator1 first1, ForwardIterator1 last1,
        ForwardIterator2 first2, ForwardIterator2 last2)
    {
        auto d1 = my_stl::distance(first1, last1);
        auto d2 = my_stl::distance(first2, last2);
        if (d1 < d2) return last1;
        auto current1 = first1;
        auto current2 = first2;
        while(current2 != last2)
        {
            if (*current1 == *current2)
            {
                // 两个元素相等 准备对比下一个元素
                ++current1;
                ++current2;
            }
            else
            {
                if (d1 == d2)
                {
                    return last1;
                }
                else
                {
                    current1 = ++first1;
                    current2 = first2;
                    --d1;  // 排除序列一中一个元素，序列一长度减1
                }
            }
        }
        return first1;
    }
    // Compare重载版本
    template <class ForwardIterator1, class ForwardIterator2, class Compare>
    ForwardIterator1
    search(ForwardIterator1 first1, ForwardIterator1 last1,
        ForwardIterator2 first2, ForwardIterator2 last2, Compare comp)
    {
        auto d1 = my_stl::distance(first1, last1);
        auto d2 = my_stl::distance(first2, last2);
        if(d1 < d2)
            return last1;
        auto current1 = first1;
        auto current2 = first2;
        while(current2 != last2)
        {
            if(comp(*current1, *current2))
            {
                ++current1;
                ++current2;
            }
            else
            {
                if (d1 == d2)
                {
                    return last1;
                }
                else
                {
                    current1 = ++first1;
                    current2 = first2;
                    --d1;
                }
            }
        }
        return first1;
    }

    // search_n
    template <class ForwardIterator, class Size, class T>
    ForwardIterator
    search_n(ForwardIterator first, ForwardIterator last, Size n, const T& value)
    {
        if (n <= 0)
        {
            return first;
        }
        else
        {
            first = my_stl::find(first, last, value);
            while (first != last)
            {
                auto m = n - 1;
                auto i = first;
                ++i;
                while (i != last && m != 0 && *i == value)
                {
                    ++i;
                    --m;
                }
                if (m == 0)
                {
                    return first;
                }
                else
                {
                    first = my_stl::find(i, last, value);
                }
            }
            return last;
        }
    }

    // Compare重载版本
    template <class ForwardIterator, class Size, class T, class Compare>
    ForwardIterator
    search_n(ForwardIterator first, ForwardIterator last,
            Size n, const T& value, Compare comp)
    {
        if (n <= 0)
        {
            return first;
        }
        else
        {
            while (first != last)
            {
                if (comp(*first, value))
                    break;
                ++first;
            }
            while (first != last)
            {
                auto m = n - 1;
                auto i = first;
                ++i;
                while (i != last && m != 0 && comp(*i, value))
                {
                    ++i;
                    --m;
                }
                if (m == 0)
                {
                    return first;
                }
                else
                {
                    while (i != last)
                    {
                        if (comp(*i, value))
                            break;
                        ++i;
                    }
                    first = i;
                }
            }
            return last;
        }
    }

    // find_end
    template <class ForwardIterator1, class ForwardIterator2>
    ForwardIterator1
    __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                    ForwardIterator2 first2, ForwardIterator2 last2,
                    forward_iterator_tag, forward_iterator_tag)
    {
        if (first2 == last2)
        {
            return last1;
        }
        else
        {
            auto result = last1;
            while (true)
            {
            // 利用 search 查找某个子序列的首次出现点，找不到则返回 last1
            auto new_result = my_stl::search(first1, last1, first2, last2);
            if (new_result == last1)
            {
                return result;
            }
            else
            {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
            }
        }
    }

    template <class BidirectionalIterator1, class BidirectionalIterator2>
    BidirectionalIterator1
    __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
                    BidirectionalIterator2 first2, BidirectionalIterator2 last2,
                    bidirectional_iterator_tag, bidirectional_iterator_tag)
    {
        typedef reverse_iterator<BidirectionalIterator1> reverse_iterator1;
        typedef reverse_iterator<BidirectionalIterator2> reverse_iterator2;
        reverse_iterator1 rlast1(first1);
        reverse_iterator2 rlast2(first2);
        reverse_iterator1 rresult = my_stl::search(reverse_iterator1(last1), rlast1, reverse_iterator2(last2), rlast2);
        if (rresult == rlast1)
        {
            return last1;
        }
        else
        {
            auto result = rresult.base();
            my_stl::advance(result, -my_stl::distance(first2, last2));
            return result;
        }
    }

    template <class ForwardIterator1, class ForwardIterator2>
    ForwardIterator1
    find_end(ForwardIterator1 first1, ForwardIterator1 last1,
            ForwardIterator2 first2, ForwardIterator2 last2)
    {
        typedef typename iterator_traits<ForwardIterator1>::iterator_category Category1;
        typedef typename iterator_traits<ForwardIterator2>::iterator_category Category2;
        return my_stl::__find_end(first1, last1, first2, last2, Category1(), Category2());
    }

    // Compare重载版本
    template <class ForwardIterator1, class ForwardIterator2, class Compare>
    ForwardIterator1
    __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                    ForwardIterator2 first2, ForwardIterator2 last2,
                    forward_iterator_tag, forward_iterator_tag, Compare comp)
    {
        if (first2 == last2)
        {
            return last1;
        }
        else
        {
            auto result = last1;
            while (true)
            {
                auto new_result = my_stl::search(first1, last1, first2, last2, comp);
                if (new_result == last1)
                {
                    return result;
                }
                else
                {
                    result = new_result;
                    first1 = new_result;
                    ++first1;
                }
            }
        }
    }

    template <class BidirectionalIterator1, class BidirectionalIterator2, class Compare>
    BidirectionalIterator1
    __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
                    BidirectionalIterator2 first2, BidirectionalIterator2 last2,
                    bidirectional_iterator_tag, bidirectional_iterator_tag, Compare comp)
    {
        typedef reverse_iterator<BidirectionalIterator1> reverse_iterator1;
        typedef reverse_iterator<BidirectionalIterator2> reverse_iterator2;
        reverse_iterator1 rlast1(first1);
        reverse_iterator2 rlast2(first2);
        reverse_iterator1 rresult = my_stl::search(reverse_iterator1(last1), rlast1, reverse_iterator2(last2), rlast2, comp);
        if (rresult == rlast1)
        {
            return last1;
        }
        else
        {
            auto result = rresult.base();
            my_stl::advance(result, -my_stl::distance(first2, last2));
            return result;
        }
    }

    template <class ForwardIterator1, class ForwardIterator2, class Compare>
    ForwardIterator1
    find_end(ForwardIterator1 first1, ForwardIterator1 last1,
            ForwardIterator2 first2, ForwardIterator2 last2, Compare comp)
    {
        typedef typename iterator_traits<ForwardIterator1>::iterator_category Category1;
        typedef typename iterator_traits<ForwardIterator2>::iterator_category Category2;
        return my_stl::__find_end(first1, last1, first2, last2, Category1(), Category2(), comp);
    }

    // find_first_of
    template <class InputIterator, class ForwardIterator>
    InputIterator
    find_first_of(InputIterator first1, InputIterator last1,
                ForwardIterator first2, ForwardIterator last2)
    {
        for (; first1 != last1; ++first1)
        {
            for (auto iter = first2; iter != last2; ++iter)
            {
                if (*first1 == *iter)
                    return first1;
            }
        }
        return last1;
    }

    template <class InputIterator, class ForwardIterator, class Compare>
    InputIterator
    find_first_of(InputIterator first1, InputIterator last1,
                ForwardIterator first2, ForwardIterator last2, Compare comp)
    {
        for (; first1 != last1; ++first1)
        {
            for (auto iter = first2; iter != last2; ++iter)
            {
                if (comp(*first1, *iter))
                    return first1;
            }
        }
        return last1;
    }
    // for_each
    template <class InputIterator, class Function>
    Function for_each(InputIterator first, InputIterator last, Function f)
    {
        for (; first != last; ++first)
        {
            f(*first);
        }
        return f;
    }
    // gengerat
    template <class ForwardIterator, class Generator>
    void generate(ForwardIterator first, ForwardIterator last, Generator gen)
    {
        for (; first != last; ++first)
        {
            *first = gen();
        }
    }
    // generate_n
    template <class ForwardIterator, class Size, class Generator>
    void generate_n(ForwardIterator first, Size n, Generator gen)
    {
        for (; n > 0; --n, ++first)
        {
            *first = gen();
        }
    }

    // includes
    template <class InputIterator1, class InputIterator2>
    bool includes(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, InputIterator2 last2)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (*first2 < *first1)
            {
                return false;
            }
            else if (*first1 < *first2)
            {
                ++first1;
            }
            else
            {
                ++first1, ++first2;
            }
        }
        return first2 == last2;
    }

    // 使用Compare类型的操作代替==
    template <class InputIterator1, class InputIterator2, class Compare>
    bool includes(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, InputIterator2 last2, Compare comp)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (comp(*first2, *first1))
            {
                return false;
            }
            else if (comp(*first1, *first2))
            {
                ++first1;
            }
            else
            {
                ++first1, ++first2;
            }
        }
        return first2 == last2;
    }

    // max_element
    template <class ForwardIterator>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
    {
        if (first == last)
            return first;
        auto result = first;
        while (++first != last)
        {
            if (*result < *first) result = first;
        }
        return result;
    }

    // 使用Compare类型的操作代替==
    template <class ForwardIterator, class Compare>
    ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp)
    {
        if (first == last)
            return first;
        auto result = first;
        while (++first != last)
        {
            if (comp(*result, *first)) result = first;
        }
        return result;
    }

    // min_element
    template <class ForwardIterator>
    ForwardIterator min_elememt(ForwardIterator first, ForwardIterator last)
    {
    if (first == last)
        return first;
    auto result = first;
    while (++first != last)
    {
        if (*first < *result) result = first;
    }
    return result;
    }

    // 使用Compare类型的操作代替==
    template <class ForwardIterator, class Compare>
    ForwardIterator min_elememt(ForwardIterator first, ForwardIterator last, Compare comp)
    {
        if (first == last)
            return first;
        auto result = first;
        while (++first != last)
        {
            if (comp(*first, *result)) result = first;
        }
        return result;
    }

    // merge
    template <class InputIterator1, class InputIterator2, class OutputIterator>
    OutputIterator
    merge(InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2,
        OutputIterator result)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (*first2 < *first1)
            {
                *result = *first2;
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
            }
            ++result;
        }
        return my_stl::copy(first2, last2, my_stl::copy(first1, last1, result));
    }

    // 使用Compare类型的操作代替==
    template <class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
    OutputIterator
    merge(InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2,
        OutputIterator result, Compare comp)
    {
        while (first1 != last1 && first2 != last2)
        {
            if (comp(*first2, *first1))
            {
                *result = *first2;
                ++first2;
            }
            else
            {
                *result = *first1;
                ++first1;
            }
            ++result;
        }
        return my_stl::copy(first2, last2, my_stl::copy(first1, last1, result));
    }

    // partition
    template <class BidirectionalIterator, class UnaryPredicate>
    BidirectionalIterator
    partition(BidirectionalIterator first, BidirectionalIterator last,
            UnaryPredicate unary_pred)
    {
        while (true)
        {
            while (first != last && unary_pred(*first))
            {
                ++first;
            }
            if (first == last) break;
            --last;
            while (first != last && !unary_pred(*last))
            {
                --last;
            }
            if (first == last) break;
            my_stl::iter_swap(first, last);
            ++first;
        }
        return first;
    }
    // remove_copy
    template <class InputIterator, class OutputIterator, class T>
    OutputIterator
    remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value)
    {
        for (; first != last; ++first)
        {
            if (*first != value)
            {
                *result++ = *first;
            }
        }
        return result;
    }
    // remove
    template <class ForwardIterator, class T>
    ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value)
    {
        first = my_stl::find(first, last, value); // 找到第一个等于value的位置 即可被覆盖的位置
        auto next = first;
        return first == last ? first : my_stl::remove_copy(++next, last, first, value);
    }
    // remove_copy_if
    template <class InputIterator, class OutputIterator, class UnaryPredicate>
    OutputIterator
    remove_copy_if(InputIterator first, InputIterator last,
                OutputIterator result, UnaryPredicate unary_pred)
    {
        for (; first != last; ++first)
        {
            if (!unary_pred(*first))
            {
                *result = *first;
                ++result;
            }
        }
        return result;
    }
    // remove_if
    template <class ForwardIterator, class UnaryPredicate>
    ForwardIterator
    remove_if(ForwardIterator first, ForwardIterator last, UnaryPredicate unary_pred)
    {
        first = my_stl::find_if(first, last, unary_pred);  // 找到第一个unary_pred(value)为true的位置 即可被覆盖的位置
        auto next = first;
        return first == last ? first : my_stl::remove_copy_if(++next, last, first, unary_pred);
    }

    // replace
    template <class ForwardIterator, class T>
    void replace(ForwardIterator first, ForwardIterator last,
                const T& old_value, const T& new_value)
    {
        for (; first != last; ++first)
        {
            if (*first == old_value)
                *first = new_value;
        }
    }

    // replace_if
    template <class ForwardIterator, class UnaryPredicate, class T>
    void replace_if(ForwardIterator first, ForwardIterator last,
                    UnaryPredicate unary_pred, const T& new_value)
    {
        for (; first != last; ++first)
        {
            if (unary_pred(*first))
                *first = new_value;
        }
    }

    // replace_copy
    template <class InputIterator, class OutputIterator, class T>
    OutputIterator
    replace_copy(InputIterator first, InputIterator last,
                OutputIterator result, const T& old_value, const T& new_value)
    {
        for (; first != last; ++first, ++result)
        {
            *result = *first == old_value ? new_value : *first;
        }
        return result;
    }

    // replace_copy_if
    template <class InputIterator, class OutputIterator, class UnaryPredicate, class T>
    OutputIterator
    replace_copy_if(InputIterator first, InputIterator last,
                    OutputIterator result, UnaryPredicate unary_pred, const T& new_value)
    {
        for (; first != last; ++first, ++result)
        {
            *result = unary_pred(*first) ? new_value : *first;
        }
        return result;
    }

    // reverse
    template <class BidirectionalIterator>
    void __reverse(BidirectionalIterator first, BidirectionalIterator last,
                        bidirectional_iterator_tag)
    {
        while (true)
        {
            if (first == last || first == --last)
                return;
            my_stl::iter_swap(first++, last);
        }
    }

    // __reverse 的 random_access_iterator_tag 版本
    template <class RandomIterator>
    void __reverse(RandomIterator first, RandomIterator last,
                        random_access_iterator_tag)
    {
        while (first < last)
            my_stl::iter_swap(first++, --last);
    }

    template <class BidirectionalIterator>
    void reverse(BidirectionalIterator first, BidirectionalIterator last)
    {
        my_stl::__reverse(first, last, iterator_category(first));
    }

    // reverse_copy
    template <class BidirectionalIterator, class OutputIterator>
    OutputIterator
    reverse_copy(BidirectionalIterator first, BidirectionalIterator last,
                OutputIterator result)
    {
        while (first != last)
        {
            --last;
            *result = *last;
            ++result;
        }
        return result;
    }

    // rotate 
    // 版本一 forward_iterator_tag版本
    template <class ForwardIterator>
    ForwardIterator
    __rotate(ForwardIterator first, ForwardIterator middle,
                    ForwardIterator last, forward_iterator_tag)
    {
        auto first2 = middle;
        do
        {
            my_stl::swap(*first++, *first2++);
            if (first == middle) middle = first2;
        } while (first2 != last);  // 后半段移到前面

        auto new_middle = first;   // 迭代器返回的位置
        first2 = middle;
        while (first2 != last)
        {   // 调整剩余元素
            my_stl::swap(*first++, *first2++);
            if (first == middle)
            {
                middle = first2;
            }
            else if (first2 == last)
            {
                first2 = middle;
            }
        }
        return new_middle;
    }
    // 版本二 bidirectional_iterator_tag 版本
    template <class BidirectionalIterator>
    BidirectionalIterator
    __rotate(BidirectionalIterator first, BidirectionalIterator middle,
                    BidirectionalIterator last, bidirectional_iterator_tag)
    {
        my_stl::reverse(first, middle);
        my_stl::reverse(middle, last);
        my_stl::reverse(first, last);   
    }
    // 辗转相除求最大公因子
    template <class EuclideanRingElement>
    EuclideanRingElement rgcd(EuclideanRingElement m, EuclideanRingElement n)
    {
        while (n != 0)
        {
            auto t = m % n;
            m = n;
            n = t;
        }
        return m;
    }
    // 版本三 random_access_iterator_tag 版本
    template <class RandomIterator>
    RandomIterator
    __rotate(RandomIterator first, RandomIterator middle,
                    RandomIterator last, random_access_iterator_tag)
    {
        auto n = last - first;
        auto l = middle - first;
        auto r = n - l;
        auto result = first + (last - middle);
        if (l == r)
        {
            std::swap_ranges(first, middle, middle);
            return result;
        }
        auto cycle_times = rgcd(n, l);
        for (auto i = 0; i < cycle_times; ++i)
        {
            auto tmp = *first;
            auto p = first;
            if (l < r)
            {
                for (auto j = 0; j < r / cycle_times; ++j)
                {
                    if (p > first + r)
                    {
                    *p = *(p - r);
                    p -= r;
                    }
                    *p = *(p + l);
                    p += l;
                }
            }
            else
            {
                for (auto j = 0; j < l / cycle_times - 1; ++j)
                {
                    if (p < last - l)
                    {
                    *p = *(p + l);
                    p += l;
                    }
                    *p = *(p - r);
                    p -= r;
                }
            }
            *p = tmp;
            ++first;
        }
        return result;
    }
    template <class ForwardIterator>
    ForwardIterator
    rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last)
    {
    if (first == middle)
        return last;
    if (middle == last)
        return first;
    return my_stl::__rotate(first, middle, last, iterator_category(first));
    }

    // rotate_copy
    template <class ForwardIterator, class OutputIterator>
    ForwardIterator
    rotate_copy(ForwardIterator first, ForwardIterator middle,
                ForwardIterator last, OutputIterator result)
    {
        return my_stl::copy(first, middle, my_stl::copy(middle, last, result));
    }

    // swap_ranges
    template <class ForwardIterator1, class ForwardIterator2>
    ForwardIterator2
    swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
                ForwardIterator2 first2)
    {
        for (; first1 != last1; ++first1, ++first2)
        {
            my_stl::iter_swap(first1, first2);
        }
        return first2;
    }

    // transform
    template <class InputIterator, class OutputIterator, class UnaryOperation>
    OutputIterator
    transform(InputIterator first, InputIterator last,
            OutputIterator result, UnaryOperation unary_op)
    {
        for (; first != last; ++first, ++result)
        {
            *result = unary_op(*first);
        }
        return result;
    }

    template <class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOperation>
    OutputIterator
    transform(InputIterator1 first1, InputIterator1 last1,
            InputIterator2 first2, OutputIterator result, BinaryOperation binary_op)
    {
        for (; first1 != last1; ++first1, ++first2, ++result)
        {
            *result = binary_op(*first1, *first2);
        }
        return result;
    }

    // unique_copy
    // 重载的forward版本
    template <class InputIterator, class ForwardIterator>
    ForwardIterator
    __unique_copy(InputIterator first, InputIterator last,
                        ForwardIterator result, forward_iterator_tag)
    {
        *result = *first;
        while (++first != last)
        {
            if (*result != *first)
                *++result = *first;
        }
        return ++result;
    }

    // __unique_copy 的 output_iterator_tag 版本
    // 重载的output版本
    template <class InputIterator, class OutputIterator>
    OutputIterator
    __unique_copy(InputIterator first, InputIterator last,
                        OutputIterator result, output_iterator_tag)
    {
        auto value = *first;
        *result = value;
        while (++first != last)
        {
            if (value != *first)
            {
                value = *first;
                *++result = value;
            }
        }
        return ++result;
    }

    template <class InputIterator, class OutputIterator>
    OutputIterator
    unique_copy(InputIterator first, InputIterator last, OutputIterator result)
    {
        if (first == last)
            return result;
        return my_stl::__unique_copy(first, last, result, iterator_category(result));
    }

    // 重载Compare 重载的forward版本
    template <class InputIterator, class ForwardIterator, class Compare>
    ForwardIterator
    __unique_copy(InputIterator first, InputIterator last,
                        ForwardIterator result, forward_iterator_tag, Compare comp)
    {
        *result = *first;
        while (++first != last)
        {
            if (!comp(*result, *first))
                *++result = *first;
        }
        return ++result;
    }

    // 重载Compare 重载的Output版本
    template <class InputIterator, class OutputIterator, class Compare>
    OutputIterator
    __unique_copy(InputIterator first, InputIterator last,
                        OutputIterator result, output_iterator_tag, Compare comp)
    {
        auto value = *first;
        *result = value;
        while (++first != last)
        {
            if (!comp(value, *first))
            {
                value = *first;
                *++result = value;
            }
        }
        return ++result;
    }

    template <class InputIterator, class OutputIterator, class Compare>
    OutputIterator
    unique_copy(InputIterator first, InputIterator last, OutputIterator result, Compare comp)
    {
        if (first == last)
            return result;
        return my_stl::__unique_copy(first, last, result, iterator_category(result), comp);
    }

    // unique
    template <class ForwardIterator>
    ForwardIterator unique(ForwardIterator first, ForwardIterator last)
    {
        first = my_stl::adjacent_find(first, last);
        return my_stl::unique_copy(first, last, first);
    }

    // 重载Compare
    template <class ForwardIterator, class Compare>
    ForwardIterator unique(ForwardIterator first, ForwardIterator last, Compare comp)
    {
        first = my_stl::adjacent_find(first, last, comp);
        return my_stl::unique_copy(first, last, first, comp);
    }

}

#endif /* __XT_STL_INTERNAL_ALGO_H */