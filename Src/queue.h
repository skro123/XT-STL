
#ifndef XT_STL_QUEUE_H
#define XT_STL_QUEUE_H

#include "deque.h"
#include "vector.h"
#include "functional.h"
#include "heap_algo.h"

namespace my_stl 
{
    template <typename, typename> class queue;
    template <class T, class Sequence>
    bool operator== (const queue<T, Sequence>&, const queue<T, Sequence>&);
    template <class T, class Sequence>
    bool operator< (const queue<T, Sequence>&, const queue<T, Sequence>&);

    template <class T, class Sequence = my_stl::deque<T> >
    class queue {
        // 内嵌型别声明 调用底层容器的
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
            queue() = default;
            template <class InputIter>
            queue(InputIter first, InputIter last)
                :_M_c(first, last) 
            {}            
            ~queue() = default;
            queue& operator=(const queue& rhs) 
            {
                _M_c = rhs._M_c; 
                return *this; 
            }            
        
        // 元素访问相关
        public:
            reference front() { return _M_c.front(); }
            const_reference front() const { return _M_c.front(); }
            reference back() { return _M_c.back(); }
            const_reference back() const { return _M_c.back();}

        // 容量相关
        public:
            bool empty() { return _M_c.empty(); }
            size_type size() { return _M_c.size(); }
        
        // 修改容器相关
        public:
            void pop() { _M_c.pop_front();}
            void push(const value_type& x) { _M_c.push_back(x);}
            void clear()
            {
                while(!this->empty())
                {
                    this->pop();
                }
            }            

        friend bool operator== (const queue& x, const queue& y) {return x._M_c == y._M_c;}
        friend bool operator< (const queue& x, const queue& y) {return x._M_c < y._M_c;}    
    };


    template <class T, class Sequence =my_stl::vector<T>, 
        class Compare = my_stl::less<typename Sequence::value_type> >
    class priority_queue
    {
        public:
        // 内嵌类型声明
          typedef typename Sequence::value_type value_type;
          typedef typename Sequence::size_type size_type;
          typedef typename Sequence::reference reference;
          typedef typename Sequence::const_reference const_reference;

        // 数据成员
        private:
          Sequence _M_c;
          Compare _M_comp;

        // 构造、析构和赋值
        public:
            priority_queue() = default;
            explicit priority_queue(const Compare& x) : _M_c(), _M_comp(x) {}
            
            template <class InputIterator>
            priority_queue(InputIterator first, InputIterator last)
            : _M_c(first, last) { make_heap(_M_c.begin(), _M_c.end(), _M_comp); }
            template <class InputIterator>
            priority_queue(InputIterator first, InputIterator last, const Compare& x)
            : _M_c(first, last), _M_comp(x) { make_heap(_M_c.begin(), _M_c.end(), _M_comp); }
            ~priority_queue() = default;

            priority_queue& operator=(const priority_queue& rhs)
            {
                _M_c = rhs._M_c;
                _M_comp = rhs._M_comp;
                my_stl::make_heap(_M_c.begin(), _M_c.end(), _M_comp);
                return *this;
            }

        // 容量相关
        public:
            bool empty() const { return _M_c.empty(); }
            size_type size() const { return _M_c.size(); }

        // 元素访问
        public:
            const_reference top() const { return _M_c.front(); }
        
        // 修改容器
        public:
            void push(const value_type& x)
            {
                try
                {
                    _M_c.push_back(x);
                    push_heap(_M_c.begin(), _M_c.end(), _M_comp);
                }
                catch(...)
                {
                    _M_c.clear();
                }
            }
            
            void pop()
            {
                try
                {
                    pop_heap(_M_c.begin(), _M_c.end(), _M_comp);
                    _M_c.pop_back();
                }
                catch(...)
                {
                    _M_c.clear();
                }
            }

            void clear()
            {
                while(!this->empty())
                {
                    this->pop();
                }
            }
    };
}

#endif /* XT_STL_QUEUE_H */