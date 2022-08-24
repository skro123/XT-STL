#ifndef XT_STL_FUNCTIONAL_H
#define XT_STL_FUNCTIONAL_H

#include <cstddef>

namespace my_stl
{

    // 定义一元函数的参数和返回值型别
    template <class Arg, class Result>
    struct unarg_function
    {
        typedef Arg         argument_type;
        typedef Result      result_type;
    };

    // 定义二元函数的参数和返回值型别
    template <class Arg1, class Arg2, class Result>
    struct binary_function
    {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };

    //------------------算术类----------------------//
    // 加法
    template <class T>
    struct plus :public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) const { return x + y; }
    };

    // 减法
    template <class T>
    struct minus : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) const { return x - y; }
    };

    // 乘法
    template <class T>
    struct multiplies : public binary_function<T, T, T>
    {
        T operator()(const T&x, const T& y) const { return x * y; }
    };

    // 除法
    template <class T>
    struct divides : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) const { return x / y; }
    };

    // 模取
    template <class T>
    struct modules : public binary_function<T, T, T>
    {
        T operator()(const T& x, const T& y) const { return x % y; }
    };

    // 否定
    template <class T>
    struct negate : public unarg_function<T, T>
    {
        T operator()(const T& x) const { return -x; }
    };

    // 加法证同元素
    template <class T>
    T identity_element(plus<T>)
    {
        return T(0);
    };

    // 乘法证同元素
    template <class T>
    T identity_element(multiplies<T>)
    {
        return T(1);
    };

    //-------------------关系运算符类------------------//
    // 等于
    template <class T>
    struct equal_to : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const { return x == y;}
    };

    // 不等于
    template <class T>
    struct not_equal_to : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const { return x != y;}
    };

    // 大于
    template <class T>
    struct greater : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const { return x > y; }
    };

    // 小于
    template <class T>
    struct less : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const { return x < y; }
    };

    // 大于等于
    template <class T>
    struct greater_equal : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const { return x >= y; }
    };

    // 小于等于
    template <class T>
    struct less_equal : public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const { return x <= y; }
    };
    

    //---------------------逻辑运算符-------------------//
    // 逻辑与
    template <class T>
    struct logical_and :public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const { return x && y; }
    };

    // 逻辑或
    template <class T>
    struct logical_or :public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const { return x || y; }
    };

    // 逻辑非
    template <class T>
    struct logical_not :public unarg_function<T, bool>
    {
        bool operator()(const T& x) const { return !x; }
    };


    //-------------------证同、选择和投影----------------//
    // 证同函数
    template <class T>
    struct identity :public unarg_function<T, T>
    {
        const T& operator()(const T& x) const { return x; }
    };

    // 选择函数 返回pair的第一个元素
    template <class Pair>
    struct selectfirst :public unarg_function<Pair, typename Pair::first_type>
    {
        const typename Pair::first_type& operator()(const Pair& x) const
        {
            return x.first;
        }
    };
    
    // 选择函数 返回pair的第二个元素
    template <class Pair>
    struct selectsecond :public unarg_function<Pair, typename Pair::second_type>
    {
        const typename Pair::second_type& operator()(const Pair& x) const
        {
            return x.second;
        }
    };

    // 投影函数 返回第一参数
    template <class Arg1, class Arg2>
    struct projectfirst :public binary_function<Arg1, Arg2, Arg1>
    {
        Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
    };

    // 投影函数 返回第二参数
    template <class Arg1, class Arg2>
    struct projectsecond :public binary_function<Arg1, Arg2, Arg1>
    {
        Arg2 operator()(const Arg1&, const Arg2& y) const { return y; }
    };
    
    //----------------------哈希函数对象-----------------//


} /* my_stl */



#endif /* XT_STL_FUNCTIONAL_H */