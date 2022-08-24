
#ifndef XT_STL_STACK_H
#define XT_STL_STACK_H

#include "deque.h"
namespace my_stl
{
    // template <typename, typename> class stack;
    // template <class T, class Sequence>
    // bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y);
    // template <class T, class Sequence>
    // bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y);

    // 配接器 stack 第一模板参数为数据类型 第二模板参数为底层容器 默认为deque   
    template <class T, class Sequence = my_stl::deque<T> >
    class stack {
        // 型别声明 从底层容器取得
        public:
            typedef typename Sequence::value_type value_type;
            typedef typename Sequence::size_type size_type;
            typedef typename Sequence::reference reference;
            typedef typename Sequence::const_reference const_reference;

        // 数据成员
        private:
            Sequence _M_c;

        // 构造、赋值和析构
        public:
            stack() = default;
            explicit stack(size_type n)
            :_M_c(n)
            {}

            stack(size_type n, const value_type& value)
            :_M_c(n, value)
            {}

            template<class Iter>
            stack(Iter first, Iter last)
            :_M_c(first, last)
            {}
            stack(std::initializer_list<T> ilist) 
            :_M_c(ilist.begin(), ilist.end())
            {}
            stack(const Sequence& c)
            :_M_c(c)
            {}

            stack(const stack& rhs)
            :_M_c(rhs._M_c)
            {}

            stack(Sequence&& c) noexcept(std::is_nothrow_move_constructible<Sequence>::value)
            :_M_c(my_stl::move(c))
            {}

            stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Sequence>::value)
            :_M_c(my_stl::move(rhs._M_c))
            {}

            stack& operator= (stack& rhs)
            {
                this->_M_c = rhs._M_c; // 使用底层容器的赋值操作符
                return *this;
            }

            stack& operator= (stack&& rhs) noexcept(std::is_nothrow_constructible<Sequence>::value)
            {
                this->_M_c = my_stl::move(rhs._M_c);
                return *this;
            }

            stack& operator=(std::initializer_list<value_type> ilist)
            {
                this->_M_c = ilist;
                return *this;
            }

            ~stack() = default;

        // 访问元素相关
        public:
            reference top() { return _M_c.back(); }
            const_reference top() const { return _M_c.back(); }

        // 容量相关
        public:
            bool empty() const { return _M_c.empty(); }
            size_type size() const { return _M_c.size(); }

        // 修改容器相关
        public:
            void push(const value_type& x) { _M_c.push_back(x); }
            void pop() { _M_c.pop_back(); }

        public:
            void swap(stack& rhs) noexcept
            {
                my_stl::swap(this->_M_c, rhs._M_c);
            }
    
        public:
            friend bool operator== (const stack& lhs, const stack& rhs) { return lhs._M_c == rhs._M_c; }
            friend bool operator< (const stack& lhs, const stack& rhs) { return lhs._M_c < rhs._M_c; };
    };

    // 比较运算符重载
    template <class T, class Sequence>
    bool operator==(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs)
    {
        return lhs == rhs;
    }
    template <class T, class Sequence>
    bool operator<(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs)
    {
        return lhs < rhs;
    }
    template <class T, class Sequence>
    bool operator!=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T, class Sequence>
    bool operator> (const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs)
    {
        return rhs < lhs;
    }
    template <class T, class Sequence>
    bool operator<=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T, class Sequence>
    bool operator>=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs)
    {
        return !(lhs < rhs);
    }

    // 重载swap 对于stack 类型
    template <class T, class Sequence>
    void swap(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs)
    {
        lhs.swap(rhs); 
    }

} // my_stl

#endif /* XT_STL_STACK_H */