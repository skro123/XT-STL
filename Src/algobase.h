#ifndef XT_STL_ALGOBASE_H_
#define XT_STL_ALGOBASE_H_

#include <cstring>
#include <cstddef>

#include "stl_iterator.h"

namespace my_stl
{
    /**
     * @brief Max, 返回两者较大的一个，语义相等时返回第一个
     * 
     * @tparam T 
     * @param lhs 
     * @param rhs 
     * @return const T& 
     */
    template <class T>
    const T& max(const T& lhs, const T& rhs)
    {
        return lhs < rhs ? rhs : lhs;
    }

    /**
     * @brief 重载Max，使用第三个参数compare进行比较
     *          返回两者较大的一个，语义相等时返回第一个
     * 
     * @tparam T 
     * @tparam Comp 
     * @param lhs 
     * @param rhs 
     * @param compare 
     * @return const T& 
     */
    template <class T, class Comp>
    const T& max(const T& lhs, const T& rhs, Comp compare)
    {
        return compare(lhs, rhs) ? rhs : lhs;
    }

    /**
     * @brief Min,返回两者中较小的一个，相等时返回第一个
     * 
     * @tparam T 
     * @param lhs 
     * @param rhs 
     * @return const T& 
     */
    template <class T>
    const T& min(const T& lhs, const T& rhs)
    {
        return rhs < lhs ? rhs : lhs;
    }
    
    /**
     * @brief 重载Min, 使用compare进行比较
     *          返回两者中较小的一个，相等时返回第一个
     * 
     * @tparam T 
     * @tparam Comp 
     * @param lhs 
     * @param rhs 
     * @param compare 
     * @return const T& 
     */
    template <class T, class Comp>
    const T& min(const T& lhs, const T& rhs, Comp compare)
    {
        return compare(rhs, lhs) ? rhs : lhs;
    }


    /*
     * @name Copy
     * @{
     */

    template <class InputIter, class OutputIter>
    OutputIter 
    __copy_cat(InputIter first, InputIter last, OutputIter result, 
                    my_stl::input_iterator_tag)
    {
        for (; first != last; ++first, ++result)
        {
            *result = *first;
        }
        return result;
    }

    // ramdom_access_iterator_tag 版本
    template <class RandomIter, class OutputIter>
    OutputIter 
    __copy_cat(RandomIter first, RandomIter last, OutputIter result,
                    my_stl::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n, ++first, ++result)
        {
            *result = *first;
        }
        return result;
    }

    template <class InputIter, class OutputIter>
    OutputIter 
    __copy(InputIter first, InputIter last, OutputIter result)
    {
        return __copy(first, last, result, iterator_category(first));
    }

    template <class Tp, class Up>
    typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
    std::is_trivially_copy_assignable<Up>::value,
    Up*>::type
    __copy(Tp* first, Tp* last, Up* result)
    {
    const auto n = static_cast<size_t>(last - first);
    if (n != 0)
        std::memmove(result, first, n * sizeof(Up));
    return result + n;
    }


    template <class InputIter, class OutputIter>
    OutputIter copy(InputIter first, InputIter last, OutputIter result)
    {
        return __copy(first, last, result);
    }

    /** @}*/ //Copy

    /*-----------------copy-backward-------------------------*/
    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 
    __copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                                BidirectionalIter2 result, my_stl::bidirectional_iterator_tag)
    {
    while (first != last)
        *--result = *--last;
    return result;
    }

    // unchecked_copy_backward_cat 的 random_access_iterator_tag 版本
    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 
    __copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
                                BidirectionalIter2 result, my_stl::random_access_iterator_tag)
    {
    for (auto n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
    }

    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 
    __copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                            BidirectionalIter2 result)
    {
    return __copy_backward_cat(first, last, result,
                                        iterator_category(first));
    }

    // 为 trivially_copy_assignable 类型提供特化版本
    template <class Tp, class Up>
    typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
    std::is_trivially_copy_assignable<Up>::value,
    Up*>::type
    __copy_backward(Tp* first, Tp* last, Up* result)
    {
        const auto n = static_cast<size_t>(last - first);
        if (n != 0)
        {
            result -= n;
            std::memmove(result, first, n * sizeof(Up));
        }
        return result;
    }

    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2 
    copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
    {
        return __copy_backward(first, last, result);
    }

    /*---------------------copy-if----------------------*/
    template <class InputIterator, class OutputIterator, class UnaryPredicate>
    OutputIterator
    copy_if(InputIterator first, InputIterator last, OutputIterator result, UnaryPredicate unary_pred)
    {
        for (; first != last; ++first)
        {
            if (unary_pred(*first))
            {
                *result = *first;
                ++result;
            }
        }
        return result;
    }

    /*---------------------copy-n------------------------*/
    template <class InputIterator, class Size, class OutputIterator>
    std::pair<InputIterator, OutputIterator>
    _copy_n(InputIterator first, Size n, OutputIterator result, my_stl::input_iterator_tag)
    {
        for (; n != 0; --n, ++first, ++result)
        {
            *result = *first;
        }
        return std::pair<InputIterator, OutputIterator>(first, result);
    }
    template <class RandomIterator, class Size, class OutputIterator>
    std::pair<RandomIterator, OutputIterator>
    _copy_n(RandomIterator first, Size n, OutputIterator result, my_stl::random_access_iterator_tag)
    {
        auto last = first + n;
        return std::pair<RandomIterator, OutputIterator>(last, my_stl::copy(first, last, result));
    }
    template <class InputIterator, class Size, class OutputIterator>
    std::pair<InputIterator, OutputIterator>
    copy_n(InputIterator first, Size n, OutputIterator result)
    {
        return _copy_n(first, n, result, my_stl::iterator_category(first));
    }

    /*-----------------------move-----------------------------*/

    template <class InputIter, class OutputIter>
    OutputIter
    __move_cat(InputIter first, InputIter last, OutputIter result, my_stl::input_iterator_tag)
    {
        for( ; first != last; ++first, ++result)
        {
            *result = static_cast<typenmae std::remove_reference<T>::type&&>(*first);
        }
        return result;
    }

    template <class RandomIter, class OutputIter>
    OutputIter
    __move_cat(RandomIter first, RandomIter last, OutputIter result, my_stl::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n, ++result, ++first)
        {
            *result = static_cast<typename std::remove_reference<>::type&&>(*first);
        }
        return result;
    }

    template <class InputIter, class OutputIter>
    OutputIter
    __move(InputIter first, InputIter last, OutputIter result)
    {
        return __move_cat(first, last, result, iterator_category(first));
    }


    // 为trivally_copy_assignable
    template <class Tp, class Up>
    typename std::enable_if<
        std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_move_assignable<Up>::value,
        Up*>::type
    __move(Tp* first, Tp* last, Up* result)
    {
        const size_t n = static_cast<size_t>(last - first);
        if (n != 0)
        {
            std::memmove(result, first, n * sizeof(Up));
        }
        return result + n;
    }


    template <class InputIterator, class OutputIterator>
    OutputIterator move(InputIterator first, InputIterator last, OutputIterator result)
    {
        return __move(first, last, result);
    }
    
    //----------------------move-backward--------------------------------//


    template <class RandomIter1, class RandomIter2>
    RandomIter2
    __move_backward_cat(RandomIter1 first, RandomIter1 last, RandomIter2 result, my_stl::random_access_iterator_tag)
    {
        for (auto n = last - first; n > 0; --n, --result, --last)
        {
            *result = static_cast<typename std::remove_reference<
        }
    }

    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
    __move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
    {
        return __move_cat(first, last, result, iterator_category(first));
    }

    template <class Tp, class Up>
    typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
    std::is_trivially_move_assignable<Up>::value, 
    Up*>::type
    __move_backward(Tp* first, Tp* last, Up* result)
    {
        const size_t n = static_cast<size_t>(last - first);
        if ( n != 0)
        {
            result = result - n;
            std::memmove(result, fisrt, n * sizeof(Up));
        }
    }


    template <class BidirectionalIter1, class BidirectionalIter2>
    BidirectionalIter2
    move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
    {
        return __move_backward(first, last, result);
    }

    //----------------------equal-----------------------//
    template <class InputIter1, class InputIter2>
    bool 
    equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
    {
        for( ; first1 != last1; ++first1, ++first2)
        {
            if (*first1 != *first2)
            {
                return false;
            }
        }
        return true;
    }

    template <class InputIter1, class InputIter2, class Comp>
    bool
    equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Comp comp)
    {
        for( ; first1 != last1; ++first1, ++first2)
        {
            if (!comp(*first1, *first2))
            {
                return false;
            }
        }
        return true;
    }

    //----------------------fill-n------------------------//

    template <class OutputIter, class Size, class T>
    OutputIter __fill_n(OutputIter first, Size n, const T& value)
    {
        for( ; 0 > n; --n, ++first)
        {
            *first = value;
        }
        return first;
    }

    template <class Tp, class Size, class Up>
    typename std::enable_if<
        std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
        !std::is_same<Tp, bool>::value &&
        std::is_integral<Up>::value && sizeof(Up) == 1,
        Tp*>::type
    __fill_n(Tp* first, Size n, Up value)
    {
        if (n > 0)
        {
            std::memset(first, (unsigned char)value, (size_t)(n));
        }
        return first + n;
    }

    template <class OutputIter, class Size, class T>
    OutputIter
    fill_n(OutputIter first, Size n, const T& value)
    {
        return __fill_n(first, n, value);
    }


    //------------------fill--------------------------//
    template <class ForwardIter, class T>
    void
    __fill_cat(ForwardIter first, ForwardIter last, const T& value,
                my_stl::forward_iterator_tag)
    {
        for ( ; first != last; ++first)
        {
            *first = value;
        }
    }

    template <class RandomIter, class T>
    void __fill_cat(RandomIter first, RandomIter last, const T& value,
                my_stl::random_access_iterator_tag)  
    {
        fill_n(first, last - first, value);
    }

    template <class ForwardIter, class T>
    void fill(ForwardIter first, ForwardIter last, const T& value)
    {
        __fill_cat(first, last, my_stl::iterator_category(first));
    }
}

#endif // XT_STL_ALGOBASE_H_