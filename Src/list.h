

#ifndef XT_STL_LIST_H
#define XT_STL_LIST_H

#include "iterator.h"
#include "alloc.h"
#include "contruct.h"

namespace my_stl {
    template <class T>
    struct __list_node {
        typedef __list_node* pointer;
        pointer prev;
        pointer next;
        T data;
    };

    template <class T, class Ref, class Ptr>
    struct __list_iterator {
        typedef __list_iterator<T, T&, T*>  iterator;
        typedef __list_iterator<T, Ref, Ptr>    self;

        typedef bidirectional_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef Ptr                         pointer;
        typedef Ref                         reference;
        typedef __list_node<T>*             link_type;
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;

        link_type _M_node;

        __list_iterator(link_type x) : _M_node(x) {}
        __list_iterator() {}
        __list_iterator(const iterator& x) : _M_node(x._M_node) {};

        bool operator== (const self& x) const {
            return _M_node == x._M_node;
        }
        bool operator!= (const self& x) const {
            return _M_node != x._M_node;
        }

        reference operator*() const { return (*_M_node).data; }
        pointer operator->() const { return &(operator*()); }

        self& operator++() {
            _M_node = (link_type)((*_M_node).next);
            return *this;
        }
        self operator++(int) {
            self tmp = *this; //这个不会使用到重载的*吗？
            ++*this;
            return tmp;
        }

        self& operator--() {
            _M_node = (link_type)((*_M_node).prev);
            return *this;
        }
        self operator--(int) {
            self tmp = *this;
            --*this;
            return tmp;
        }
    };

    template <class T, class Alloc = alloc> 
    class list {
        // 内嵌型别声明
        public: 
            typedef __list_node<T>                  list_node;
            typedef simple_alloc<list_node, Alloc>  list_node_allocator;

            typedef T                   value_type;
            typedef value_type*         pointer;
            typedef const value_type*   const_pointer;
            typedef value_type&         reference;
            typedef const value_type&   const_reference;
            typedef list_node*          link_type;
            typedef size_t              size_type;
            typedef ptrdiff_t           difference_type;

            typedef __list_iterator<T, T&, T*>  iterator;
            typedef __list_iterator<T, const T&, const T*>  const_iterator; 

        // 数据成员 
        private:
            link_type _M_node; // 指向空白的尾节点哨兵

        // 构造 赋值 析构相关
        public:
            list() { this->empty_initialize(); }
            explicit list(size_type n) { this->fill_initialize(n, value_type());}
            list(size_type n, const value_type& value) { this->fill_initialize(n, value);}
            list(int n, const value_type& value) { this->fill_initialize(n, value);}
            list(long n, const value_type& value) { this->fill_initialize(n, value); }
            // 范围构造
            template <class InputIter>
            list(InputIter first, InputIter last) {this->range_initialize(first, last);}
            // 初始化列表构造
            list(std::initializer_list<T> ilist) {this->range_initialize(ilist.begin(), ilist.end());}
            // 拷贝构造
            list(const list& rhs) { range_initialize(rhs.begin(), rhs.end());}
            // 移动构造
            list(list&& rhs) noexcept 
                :_M_node(rhs._M_node)
            { 
                rhs._M_node = nullptr;
            }
            // 赋值
            list& operator=(const list& rhs)
            {  
                if(this != &rhs)
                {
                    this->copy_assign(rhs.begin(), rhs.end());
                }
                return *this;
            }
            // 移动赋值
            list& operator=(list&& rhs) noexcept
            {
                this->clear();
                this->splice(this->end(), rhs); // 移动后不应该清空rhs吗
                return *this;
            }
            // 初始化列表赋值
            list& operator=(std::initializer_list<T> ilist)
            {
                list tmp(ilist.begin(), ilist.end());
                swap(tmp);
                return *this;
            }
            // 析构函数
            ~list()
            {
                if(this->_M_node != nullptr)
                {
                    this->clear();
                    this->destroy_node(this->_M_node);
                    this->_M_node = nullptr;
                }
            }

        // 迭代器相关
        public:
            iterator begin() noexcept
            {  return (*_M_node).next; }
            iterator begin() const noexcept 
            { return (*_M_node).next;}
            iterator end() noexcept 
            { return _M_node;}
            iterator end() const noexcept
            { return _M_node;}

        // 容量相关
        public:
            bool empty() const { return _M_node->next == _M_node; }       
            size_type size() const {
                size_type result = 0;
                distance(begin(), end(), result); // 需要重载这个函数
                return result;
            }

        // 元素访问相关
        public:
            reference front() { return *begin(); }
            reference back() { return *(--end()); }


        // 容器修改
        public:
            // 插入
            iterator insert(iterator position, const T& x);
            void push_back(const T& x) { insert(end(), x); }
            void push_front(const T& x) { insert(begin(), x);}

            // 删除
            iterator erase(iterator position);
            iterator erase(iterator first, iterator last);
            void pop_frnot() { erase(begin()); }
            void pop_back() {
                iterator tmp = end();
                erase(--tmp);
            }

            // 清除
            void clear();
            // 交换
            void swap(list& rhs) noexcept
            {
                my_stl::swap(this->_M_node, rhs._M_node);
            }

        // List 相关操作
        public:
            // 将x接合与position所指位置之前，通常是end(), x必须不同于*this
            void splice(iterator position, list& x);
            // 将it所指元素接合到position所指位置之前。 两者可指向同一个list
            void splice(iterator position, list& x, iterator it);
            // 将[first, last) 内的所有元素接合于position所指位置之前 
            // position和[first, last)可指向同一个list 但 两者不能重合
            void splice(iterator position, list& x, iterator first, iterator last);
            // 改写原版 利用erase函数返回值递增
            void remove(const T& value);
            // 改写原版 利用erase函数返回值递增first
            void unique();
            void transfer (iterator position, iterator first, iterator last);    

        // 辅助函数
        protected:
            // 空间分配与回收
            link_type get_node() { return list_node_allocator::allocate(); }
            void put_node(link_type p) { list_node_allocator::deallocate(p); }

            // 创建节点
            link_type create_node(const T& x) {
                link_type p = get_node();
                construct(&p->data, x);
                return p;
            }
            // 销毁节点
            void destroy_node(link_type p) {
                destroy(&p->data);
                put_node(p);
            }
            
            void empty_initialize() 
            {
                _M_node = get_node();
                _M_node->next = _M_node;
                _M_node->prev = _M_node;
            }
            void fill_initialize(size_type n, const value_type& value);
            template <class InputIter>
            void range_initialize(InputIter first, InputIter last );

            void fill_assign(size_type n, const value_type& value);

            template <class InputIter>
            void copy_assign(InputIter rhs_first, InputIter rhs_last)
            {
                auto lhs_first = this->begin();
                auto lhs_last = this->end();
                for(; lhs_first != lhs_last && rhs_first != rhs_last; ++lhs_first, ++rhs_first)
                {
                    *lhs_first = *rhs_first;
                } 
                if(rhs_first == rhs_last)
                {
                    this->erase(lhs_first, lhs_last);
                }
                else
                {
                    for(; rhs_first != rhs_last; ++rhs_first)
                    {
                        this->push_back(*rhs_first);
                    }
                }
            }

    };

    template <class T, class Alloc>
    void 
    list<T, Alloc>::splice(iterator position, list& x)
    {
        assert(this != &x);
        if(!x.empty())
        {
            this->transfer(position, x.begin(), x.end());
        }
    }

    template <class T, class Alloc>
    void
    list<T, Alloc>::splice(iterator position, list& x, iterator it)
    {
        iterator it_next = it;
        ++it_next;
        if (position == it || position == it_next) return;
        this->transfer(position, it, it_next);
    }

    template <class T, class Alloc>
    void
    list<T, Alloc>::splice(iterator position, list& x, iterator first, iterator last)
    {
        if(first != last) { this->transfer(position, first, last); }
    }

    template <class T, class Alloc>
    void 
    list<T, Alloc>::fill_assign(size_type n, const value_type& value)
    {
        auto i = begin();
        auto e = end();
        for(; n > 0 && i != e; --n, ++i)
        {
            *i = value;
        }
        if (n > 0)
        {
            this->insert(e, n, value);
        }
        else
        {
            this->erase(i, e);
        }
    }

    template <class T, class Alloc>
    void 
    list<T, Alloc>::fill_initialize(size_type n, const value_type& value)
    {
        empty_initialize();
        try
        {
            for(; n > 0; --n)
            {
                this->push_back(value);
            }
        }
        catch (...)
        {
            clear();
            destroy_node(this->_M_node);
            this->_M_node = nullptr;
            throw;
        }
    }

    template <class T, class Alloc>
    template <class InputIter>
    void 
    list<T, Alloc>::range_initialize(InputIter first, InputIter last )
    {
        this->empty_initialize();
        size_type n = my_stl::distance(first, last);
        try
        {
            for(; n > 0; --n, ++first)
            {
                this->push_back(*first);
            }
        }
        catch(...)
        {
            this->clear();
            this->destroy_node(this->_M_node);
            this->_M_node = nullptr;
            throw;
        }
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator 
    list<T, Alloc>::insert(iterator position, const T& x) 
    {
        link_type tmp  = create_node(x);
        tmp->prev = position._M_node->prev;
        tmp->next = position._M_node;
        (position._M_node->prev)->next = tmp;
        position._M_node->prev = tmp;
        return tmp;
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator 
    list<T, Alloc>::erase(iterator position) 
    {
        /*
            删除node节点哨兵会导致错误
            但保证不是node节点应该由使用者负责
        */
        link_type next_node = position._M_node->next;
        position._M_node->prev->next = next_node;
        next_node->prev = position._M_node->prev;
        destroy_node(position._M_node);
        return iterator(next_node);
    }

    template <class T, class Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::erase(iterator first, iterator last)
    {
        if(first != last)
        {
            first._M_node->prev->next = last._M_node->next;
            last._M_node->next->prev = first._M_node->prev;
            while (first != last)
            {
                auto cur = first._M_node;
                ++first;
                this->destroy_node(cur);
            }
        }
        return iterator(last._M_node);
    }
    template <class T, class Alloc>
    void 
    list<T, Alloc>::clear() 
    {
        link_type cur = _M_node->next;
        while(cur != _M_node) {
            link_type tmp = cur;
            cur = cur->next;
            destroy_node(tmp);
        }
        _M_node->next = _M_node;
        _M_node->prev = _M_node;
    }

    template <class T, class Alloc>
    void 
    list<T, Alloc>::remove(const T& value) 
    {
        iterator first = begin();
        iterator last = end();
        while (first != last) {
            if (*first == value) {
                first = erase(first);
            } else {
                ++first;
            }
        }
    }

    template <class T, class Alloc>
    void 
    list<T, Alloc>::unique() {
        iterator first = begin();
        iterator last = end();
        if (first == last) return;
        T value = *first;
        ++first;
        while(first != last) {
            if (*first == value) {
                first = erase(first);
            } else {
                value = *first;
                ++first;
            }
        }
    }      

    template <class T, class Alloc>
    void 
    list<T, Alloc>::transfer(iterator position, iterator first, iterator last) 
    {
        // 前三步处理三个迭代器前节点的next
        position._M_node->prev->next = first._M_node;
        first._M_node->prev->next = last._M_node;
        last._M_node->prev->next = position._M_node;
        // 后四步处理三个迭代器的prev, 三者交换prev需要一个临时变量
        link_type tmp = position._M_node->prev;
        position._M_node->prev = last._M_node->prev;
        last._M_node->prev = first._M_node->prev;
        first._M_node->prev = tmp;
    } 
    // 重载 mystl 的 swap
    template <class T>
    void swap(list<T>& lhs, list<T>& rhs) noexcept
    {
        lhs.swap(rhs);
    }
}

#endif /* XT_STL_LIST_H */