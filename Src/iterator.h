

#ifndef XT_STL_ITERATOR_H
#define XT_STL_ITERATOR_H

#include <cstddef>

namespace my_stl
{
    //----------------------------定义迭代器五种类型---------------------------//
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    // 声明迭代器类型
    template <class Category, class T, class Distance = ptrdiff_t, \
    class Pointer = T*, class Reference = T&>
    struct iterator {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Distance    difference_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
    };

    // iterator的traits
    template <class Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type       value_type;
        typedef typename Iterator::difference_type  difference_type;
        typedef typename Iterator::pointer          pointer;
        typedef typename Iterator::reference        reference;
    };

    // iterator的traits 针对原生指针特化
    template <class T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef ptrdiff_t                       difference_type;
        typedef T*                              pointer;
        typedef T&                              reference;
    };

    template <class T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef ptrdiff_t                   difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    // 取得迭代器类型
    template <class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&) {
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();
    }

    // 取得迭代器的距离类型
    template <class Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }
    // 取得迭代器值类型
    template <class Iterator>
    inline typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    /*
     下面取得迭代器类型
      distance 函数重载为两种形式 一种不带初始距离n
      每种重载形式会针对迭代器类型进行强化
      若是随机迭代器可以直接相减 否则逐个移动
    */
    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last, input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while(first != last) {
            ++first;
            ++n;
        }
        return n;
    }

    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last, random_access_iterator_tag) {
        return last - first;
    }

    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last) {
        typedef typename iterator_traits<InputIterator>::iterator_category category;
        return __distance(first, last, category());
    }

    template <class InputIterator, class Distance>
    inline void __distance(InputIterator first, InputIterator last, Distance& n,
                            input_iterator_tag) 
    {
        while (first != last) 
        {
            ++first;
            ++n;
        }
    }

    template <class RandomAccessIterator, class Distance>
    inline void __distance(RandomAccessIterator first, RandomAccessIterator last,
                            Distance& n, random_access_iterator_tag) 
    {
        n += last - first;
    }

    template <class InputIterator, class Distance>
    inline void distance(InputIterator first, InputIterator last, Distance& n)
    {
        __distance(first, last, n, iterator_category(first));
    }
    /* distance end */

    /*
        下面advance针对迭代器进行移动
        __advance为实际操作的函数 针对输入迭代器被重载为三种
        对于随机迭代器 直接使用 + 运算符进行操作
        对于双向迭代器 对于距离n是否大于等于0 分别使用前置++和前置--进行移动
        对于InputIterator 只能向前移动 这里假设距离n应该大于0 使用前置++进行移动
    */
    template <class InputIterator, class Distance>
    inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
        while(n--) ++i;
    }

    template <class InputIterator, class Distance>
    inline void __advance(InputIterator& i, Distance n, bidirectional_iterator_tag) {
        if(n >= 0)
            while(n--) ++i;
        else
            while(n++) --i;
    }

    template <class InputIterator, class Distance>
    inline void __advance(InputIterator& i, Distance n, random_access_iterator_tag) {
        i += n;
    }

    template <class InputIterator, class Distance>
    inline void advance(InputIterator& i, Distance n) {
        __advance(i, n, iterator_category(i));
    }
    /* advance end */

    //----------------------反向迭代器------------------------------------------//
    template <class Iterator>
    class reverse_iterator
    {
        // 迭代器要求的五种内嵌型别声明
        public:
            typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
            typedef typename iterator_traits<Iterator>::value_type        value_type;
            typedef typename iterator_traits<Iterator>::difference_type   difference_type;
            typedef typename iterator_traits<Iterator>::pointer           pointer;
            typedef typename iterator_traits<Iterator>::reference         reference;

            typedef Iterator                                              iterator_type;
            typedef reverse_iterator<Iterator>                            self;

        // 数据成员
        private:
            Iterator _M_current;

        // 构造函数
        public:
            reverse_iterator() {}
            explicit reverse_iterator(iterator_type i) :_M_current(i) {}
            reverse_iterator(const self& rhs) :_M_current(rhs._M_current) {}

        public:
            iterator_type base() const 
            { return _M_current; }

            reference operator*() const
            {
                auto tmp = _M_current;
                return *--tmp;
            }
            pointer operator->() const
            {
                return &(operator*());
            }

            // 前进(++)变为后退(--)
            self& operator++()
            {
                --_M_current;
                return *this;
            }
            self operator++(int)
            {
                self tmp = *this;
                --_M_current;
                return tmp;
            }
            // 后退(--)变为前进(++)
            self& operator--()
            {
                ++_M_current;
                return *this;
            }
            self operator--(int)
            {
                self tmp = *this;
                ++_M_current;
                return tmp;
            }

            self& operator+=(difference_type n)
            {
                _M_current -= n;
                return *this;
            }
            self operator+(difference_type n) const
            {
                return self(_M_current - n);
            }
            self& operator-=(difference_type n)
            {
                _M_current += n;
                return *this;
            }
            self operator-(difference_type n) const
            {
                return self(_M_current + n);
            }

            reference operator[](difference_type n) const
            {
                return *(*this + n);
            }
    };

    // 重载 operator-
    template <class Iterator>
    typename reverse_iterator<Iterator>::difference_type
    operator-(const reverse_iterator<Iterator>& lhs,
            const reverse_iterator<Iterator>& rhs)
    {
        return rhs.base() - lhs.base();
    }

    // 重载比较操作符
    template <class Iterator>
    bool operator==(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <class Iterator>
    bool operator<(const reverse_iterator<Iterator>& lhs,
    const reverse_iterator<Iterator>& rhs)
    {
        return rhs.base() < lhs.base();
    }

    template <class Iterator>
    bool operator!=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class Iterator>
    bool operator>(const reverse_iterator<Iterator>& lhs,
                const reverse_iterator<Iterator>& rhs)
    {
        return rhs < lhs;
    }

    template <class Iterator>
    bool operator<=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class Iterator>
    bool operator>=(const reverse_iterator<Iterator>& lhs,
                    const reverse_iterator<Iterator>& rhs)
    {
        return !(lhs < rhs);
    }
}

#endif /* __SGI_STL_INTERNAL_ITERATOR_H */