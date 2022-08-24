
#ifndef XT_STL_DEQUE_H
#define XT_STL_DEQUE_H

#include "iterator.h"
#include "alloc.h"
#include "uninitialized.h"
#include "contruct.h"
#include "algobase.h"
#include "util.h"
namespace my_stl {

    inline size_t __deque_buf_size(size_t n, size_t sz) {
        return n != 0 ? n : (sz < 512 ? size_t(512/sz) : size_t(1));
    }
    // 双端队列专属迭代器 
    template <class T, class Ref, class Ptr, size_t BuffSize>
    struct __deque_iterator {
        typedef __deque_iterator<T, T&, T*, BuffSize>   iterator;
        typedef __deque_iterator<T, const T&, const T*, BuffSize>   const_iterator;
        static size_t buffer_size() {
            return __deque_buf_size(BuffSize, sizeof(T));
        }

        typedef random_access_iterator_tag iterator_category;
        typedef T   value_type;
        typedef Ptr pointer;
        typedef Ref reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T** map_pointer;

        typedef __deque_iterator self;

        T* cur;
        T* first;
        T* last;
        map_pointer node;

        // 迭代器关键行为
        void set_node(map_pointer new_node) {
            node = new_node;
            first = *new_node;
            // last 指向一个不存在的元素
            last = first + difference_type(buffer_size());
        }

        reference operator*() const {return *cur;}
        pointer operator->() const {return &(operator*());}
        difference_type operator-(const self& x) const {
            return difference_type(buffer_size()) * (node - x.node - 1)
                        + (cur - first) + (x.last - x.cur);
        }
        self& operator++() {
            ++cur;
            if(cur == last) {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }
        self operator++(int) {
            self tmp = *this;
            ++*this;
            return tmp;
        }
        
        self operator--() {
            // 注意这里与++不同
            // cur不会指向last，可以先加再判断
            // 但cur可能已经指向了first
            if(cur == first) {
                set_node(node -1);
                cur = last;
            }      
            --cur; 
            return *this;
        }

        self operator--(int) {
            self tmp = *this;
            --*this;
            return tmp;
        }

        self& operator+=(difference_type n) {
            difference_type offset = n + (cur - first);
            if(offset >= 0 && offset < difference_type(buffer_size()))
                cur += n;
            else {
                difference_type node_offset = 
                    offset > 0 ? offset / difference_type(buffer_size())
                        : -difference_type((-offset - 1) / buffer_size()) - 1;
                set_node(node + node_offset);
                cur = first + (offset - node_offset * difference_type(buffer_size()));
            }
            return *this;
        }

        self operator+ (difference_type n) const {
            self tmp = *this;
            return tmp += n;
        }
        self& operator-= (difference_type n) {return *this += -n;}
        self operator- (difference_type n) const {
            self tmp = *this;
            return tmp -= n;
        }

        reference operator[](difference_type n) const { return *(*this + n);}

        bool operator==(const self& x) const { return cur == x.cur;}
        bool operator!=(const self& x) const { return cur != x.cur;}
        bool operator<(const self& x) const {
            return (x.node == node) ? (cur < x.cur) : (node < x.node);
        }
        
    };

    // 双端队列
    template <class T, class Alloc = alloc, size_t BufSize = 0>
    class deque {
        //---------------------内嵌型别定义-----------------------//
        public:
            // 基础类型
            typedef T value_type;
            typedef value_type* pointer;
            typedef value_type& reference;
            typedef const value_type& const_reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
            // 专属迭代器
            typedef __deque_iterator<T, T&, T*, BufSize> iterator;

        protected:
            typedef pointer* map_pointer;

        // 空间分配器
        protected:
            typedef simple_alloc<value_type, Alloc> data_allocator;  // 分配元素的
            typedef simple_alloc<pointer, Alloc> map_allocator;  // 分配缓冲区的

        //------------------------- 数据成员 ---------------------//
        private:
            iterator _M_start;   // 连接开始元素所在的缓冲区
            iterator _M_finish;  // 连接最后一个元素所在的缓冲去
            map_pointer _M_map;  // 一个保存多个缓冲区的地址的连续空间
            size_type _M_map_size;  // 有多少个缓冲区 即保存了多少个缓冲区的地址

        //---------------------------构造、赋值和析构-------------------//
        public:
            deque()
            : _M_start(), _M_finish(), _M_map(0), _M_map_size(0)
            { create_map_and_nodes(0); }

            deque(int n, const value_type& value)
            : _M_start(), _M_finish(), _M_map(0), _M_map_size(0)
            { fill_initialized(n, value); }

            template <class InputIter>
            deque(InputIter first, InputIter last)
            { this->copy_initialized(first, last); }
            // 初始化列表构造
            deque(std::initializer_list<value_type> ilist)
            {
                this->copy_initialized(ilist.begin(), ilist.end());
            }
            // 拷贝构造
            deque(const deque& rhs)
            {
                copy_initialized(rhs.begin(), rhs.end());
            }
            // 移动构造
            deque(deque&& rhs) noexcept
             :_M_start(rhs._M_start),
              _M_finish(rhs._M_finish),
              _M_map(rhs._M_map),
              _M_map_size(rhs._M_map_size)
            {
                rhs._M_map = nullptr;
                rhs._M_map_size = 0;
            }
            // 拷贝赋值
            deque& operator= (const deque& rhs)
            {
                if(this != &rhs)
                {
                    const auto len = this->size();
                    if(len > rhs.size())
                    {
                        this->erase(my_stl::copy(rhs._M_start, rhs._M_finish, this->_M_start), this->_M_finish);
                    }
                    else // rhs中的元素更多 lhs中需要再插入一些
                    {
                        iterator mid = rhs.begin() + static_cast<difference_type>(len);
                        // 在lhs已有的空间上copy  不析构元素直接copy是否会造成内存泄漏 但SGI5也是这样操作的
                        my_stl::copy(rhs._M_start, mid, this->_M_start);
                        while(mid != rhs._M_finish)
                        {
                            this->insert(this->end(), *(mid++));
                        }
                    }
                }
                return *this;
            }
            // 移动赋值
            deque& operator= (deque&& rhs)
            {
                this->clear();
                this->_M_start =  my_stl::move(rhs._M_start);
                this->_M_finish = my_stl::move(rhs._M_finish);
                this->_M_map = rhs._M_map;
                this->_M_map_size = rhs._M_map_size;
                rhs._M_map = nullptr;
                rhs._M_map_size = 0;
                return *this;
            }
            // 初始化列表赋值
            deque& operator= (std::initializer_list<value_type> ilist)
            {
                deque tmp(ilist);
                this->swap(tmp);
                return *this;
            }
            
            ~deque() 
            {
                // 先析构所有数据元素，以防其中有指针
                destroy(_M_start, _M_finish);
                destroy_map_and_nodes();
            }

        // -------------------------迭代器------------------------//
        public:
            iterator begin() noexcept
            { return _M_start; }
            iterator begin() const noexcept
            { return _M_start; }
            iterator end()  noexcept
            { return _M_finish; }
            iterator end() const noexcept
            { return _M_finish; }

        //-------------------------元素访问-----------------------//
        public:
            reference operator[] (size_type n) { return _M_start[difference_type(n)];}
            const_reference operator[] (size_type n) const { return _M_start[difference_type(n)];}
            reference front() { return *_M_start; }
            const_reference front() const { return *_M_start; }
            reference back() 
            {
                iterator tmp = _M_finish;
                --tmp;
                return *tmp;
            }
            const_reference back() const 
            {
                iterator tmp = _M_finish;
                --tmp;
                return *tmp;
            }
        
        //--------------------------容量相关------------------------//
        public:
            size_type size() const noexcept 
            { return _M_finish - _M_start;}
            size_type max_size() const noexcept
            { return size_type(-1);}
            bool empty() const noexcept
            { return _M_start == _M_finish;}

        //------------------------容器修改相关操作-----------------------//
        private:
            void push_back_aux(const value_type& t);
            void push_front_aux(const value_type& t);
            iterator insert_aux(iterator pos, const value_type& x);
            void pop_back_aux();
            void pop_front_aux();

        public:
            // 插入
            void push_back(const value_type& t) {
                if (_M_finish.cur != _M_finish.last - 1)
                {
                    construct(_M_finish.cur, t);
                    ++_M_finish.cur;
                }
                else
                    push_back_aux(t);
            }
            void push_front(const value_type& t) {
                if (_M_start.cur != _M_start.first) {
                    construct(_M_start.cur - 1, t);
                    --_M_start.cur;
                }
                else
                    push_front_aux(t);
            }
            iterator insert(iterator pos, const value_type& x) {
                if (pos.cur == _M_start.cur) {
                    push_front(x);
                    return _M_start;
                }
                else if (pos.cur == _M_finish.cur) {
                    push_back(x);
                    iterator tmp = _M_finish;
                    return --tmp;
                }
                else {
                    return insert_aux(pos, x);
                }
            }

            // 删除
            void pop_back() {
                // finish.cur 是不可达的 前闭后开
                if (_M_finish.cur != _M_finish.first) {
                    --_M_finish.cur; // 这里使用的是原始指针的--运算符
                    destroy(_M_finish.cur);
                }
                else
                    pop_back_aux();
            }

            void pop_front() {
                if (_M_start.cur != _M_start.last - 1) {
                    destroy(_M_start.cur);
                    ++_M_start.cur;
                }
                else
                    pop_front_aux();
            }
            iterator erase(iterator pos);
            iterator erase(iterator first, iterator last);

            // 清空
            void clear();

            // swap
            void swap(deque& rhs) noexcept;
        
        // 辅助函数
        private:
            static size_type buffer_size() {
                return __deque_buf_size(BufSize, sizeof(value_type));
            }
            static size_type initial_map_size() { return 8; }
            
            pointer allocate_node() { return data_allocator::allocate(buffer_size()); }
            void deallocate_node(pointer p) { data_allocator::deallocate(p, buffer_size()); }

            void create_map_and_nodes(size_type num_elements);
            void destroy_map_and_nodes();
            void fill_initialized(size_type n, const value_type& value);
            template <class InputIter>
            void copy_initialized(InputIter first, InputIter last);
            void reallocate_map(size_type nodes_to_add, bool add_at_front);
            void reserve_map_at_back(size_type nodes_to_add = 1) {
                if (nodes_to_add + 1 > _M_map_size - (_M_finish.node - _M_start.node)) {
                    reallocate_map(nodes_to_add, false);
                }
            }
            void reserve_map_at_front(size_type nodes_to_add = 1) {
                if (nodes_to_add > _M_start.node - _M_map) {
                    reallocate_map(nodes_to_add, true);
                }
            }

    
    };

    template <class T, class Alloc, size_t BufSiz>
    typename deque<T, Alloc, BufSiz>::iterator
    deque<T, Alloc, BufSiz>::
    insert_aux(iterator pos, const value_type& x) {
        // std::cout << "in insert_aux" << std::endl;
        difference_type index = pos - _M_start;
        value_type x_copy = x;
        // std::cout << "index:" << index << std::endl;
        if (index < (size() / 2)) {
            // std::cout << "in insert_aux index < size() >> 1" << std::endl;
            push_front(front());
            iterator front1 = _M_start + 1;
            iterator front2 = front1 + 1;
            pos = _M_start + index;
            iterator pos1 = pos + 1;
            // 这里是从第二个元素开始移动的，因为第一个元素
            // 已经插入到队列头了。
            std::copy(front2, pos1, front1);
        }
        else {
            // std::cout << "in insert_aux else" << std::endl;
            push_back(back());
            // std::cout << "in insert_aux else push_back" << std::endl;
            // std::cout << "back:" << back() << std::endl;
            iterator back1 = _M_finish - 1;
            iterator back2 = back1 - 1;
            pos = _M_start + index;
            std::copy_backward(pos, back2, back1);
            // std::cout << "in insert_aux else out" << std::endl;
        }
        *pos = x_copy;
        return pos;
    }    

    template <class T, class Alloc, size_t BufSiz>
    typename deque<T, Alloc, BufSiz>::iterator
    deque<T, Alloc, BufSiz>::
    erase(iterator pos) {
        iterator next = pos;
        ++next;
        difference_type index = pos - _M_start;
        if (index < size() >> 1) {
            // copy_backward的目的地址也符合前闭后开
            std::copy_backward(_M_start, pos, next);
            pop_front();
        }
        else {
            std::copy(next, _M_finish, pos);
            pop_back();
        }
        return _M_start + index;
    }

    template <class T, class Alloc, size_t BufSiz>
    typename deque<T, Alloc, BufSiz>::iterator
    deque<T, Alloc, BufSiz>::
    erase(iterator first, iterator last) {
        if (first == _M_start && last == _M_finish) {
            clear();
            return _M_finish;
        }
        else {
            difference_type n = last - first;
            difference_type elems_before = first - _M_start;
            if (elems_before < (size() - n) / 2) {
                my_stl::copy_backward(_M_start, first, last);
                iterator new_start = _M_start + n;
                destroy(_M_start, new_start);
                for(map_pointer cur = _M_start.node; cur < new_start.node; ++cur) {
                    data_allocator::deallocate(*cur, buffer_size());
                }
                _M_start = new_start;
            } 
            else {
                my_stl::copy(last, _M_finish, first);
                iterator new_finish = _M_finish - n;
                destroy(new_finish, _M_finish);
                for(map_pointer cur = new_finish.node; cur < _M_finish.node; ++cur) {
                    data_allocator::deallocate(*cur, buffer_size());
                }
                _M_finish = new_finish;
            }
            return _M_start + elems_before;
        }
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    clear() {
        for (map_pointer node = _M_start.node + 1; node < _M_finish.node; ++node) {
            destroy(*node, *node + buffer_size() );
            data_allocator::deallocate(*node, buffer_size());
        }
        if (_M_start.node != _M_finish.node) {
            destroy(_M_start.cur, _M_start.last);
            destroy(_M_finish.first, _M_finish.cur);
            data_allocator::deallocate(_M_finish.first, buffer_size());
        }
        else
            destroy(_M_start.cur, _M_finish.cur); // 注意这里空间可能不是满的
        _M_finish = _M_start;
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::swap(deque& rhs) noexcept
    {
        if(this != &rhs)
        {
            my_stl::swap(this->_M_start, rhs._M_start);
            my_stl::swap(this->_M_finish, rhs._M_finish);
            my_stl::swap(this->_M_map, rhs._M_map);
            my_stl::swap(this->_M_map_size, rhs._M_map_size);
        }
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    pop_back_aux() {
        deallocate_node(_M_finish.first);
        _M_finish.set_node(_M_finish.node - 1);
        _M_finish.cur = _M_finish.last - 1;
        destroy(_M_finish.cur);
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    pop_front_aux() {
        destroy(_M_start.cur);
        deallocate_node(_M_start.first);
        _M_start.set_node(_M_start.node + 1);
        _M_start.cur = _M_start.first;
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    push_back_aux(const value_type& t) {
        value_type t_copy = t;
        reserve_map_at_back();
        *(_M_finish.node + 1) = allocate_node();
        try {
            construct(_M_finish.cur, t_copy);
            _M_finish.set_node(_M_finish.node + 1);
            _M_finish.cur = _M_finish.first;
        }
        catch(...) {
            std::cout<< "push_back_aux expection" << std::endl;

            deallocate_node(*(_M_finish.node + 1));
            throw;
        }
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    push_front_aux(const value_type& t) {
        value_type t_copy = t;
        reserve_map_at_front();
        *(_M_start.node - 1) = allocate_node();
        try {
            _M_start.set_node(_M_start.node - 1);
            _M_start.cur = _M_start.last - 1;
            construct(_M_start.cur, t_copy);
        }
        catch (...) {
            std::cout<< "push_front_aux expection" << std::endl;

            _M_start.set_node(_M_start.node + 1);
            _M_start.cur = _M_start.first;
            deallocate_node(*(_M_start.node - 1));
            throw;
        }
    }

    template <class T, class Alloc, size_t BufSiz>
    void 
    deque<T, Alloc, BufSiz>::
    fill_initialized(size_type n, const value_type& value) 
    {
        create_map_and_nodes(n);
        map_pointer cur;
        try {
            for (cur = _M_start.node; cur < _M_finish.node; ++cur) {
                uninitialized_fill(*cur, *cur + buffer_size(), value);
            }
            uninitialized_fill(_M_finish.first, _M_finish.cur, value);       
        }
        catch (...) {
            std::cout<< "fill_initialized expection" << std::endl;

            destroy_map_and_nodes();
        }

    }

    template <class T, class Alloc, size_t BufSiz>
    template <class InputIter>
    void
    deque<T, Alloc, BufSiz>::copy_initialized(InputIter first, InputIter last)
    {
        const size_type n = my_stl::distance(first, last);
        this->create_map_and_nodes(0);
        for(; first != last; ++first)
        {
            // 这种做法效率不高
            this->push_back(*first);
        }
    }

    template <class T, class Alloc, size_t BufSiz>
    void 
    deque<T, Alloc, BufSiz>::
    create_map_and_nodes(size_type num_elements)
    {
        size_type num_nodes = num_elements / buffer_size() + 1;
        _M_map_size = std::max(initial_map_size(), num_nodes + 2);
        _M_map = map_allocator::allocate(_M_map_size);

        map_pointer nstart = _M_map + (_M_map_size - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes - 1;

        map_pointer cur;
        try {
            for (cur = nstart; cur <= nfinish; ++cur) 
                *cur = allocate_node();
        }
        catch(...) {
            std::cout<< "create_map_and_nodes expection" << std::endl;
            destroy_map_and_nodes();
        }
        _M_start.set_node(nstart);
        _M_finish.set_node(nfinish);
        _M_start.cur = _M_start.first;
        _M_finish.cur  = _M_finish.first + num_elements % buffer_size();
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    destroy_map_and_nodes()
    {
        for (map_pointer cur = _M_start.node; cur != _M_finish.node; ++cur) {
            deallocate_node(*cur);
        }
        map_allocator::deallocate(_M_map, _M_map_size);
    }


    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    reallocate_map(size_type nodes_to_add, bool add_to_front) {
        size_type old_num_nodes = _M_finish.node - _M_start.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;

        map_pointer new_nstart;
        if (_M_map_size > 2 * new_num_nodes) { 
            new_nstart = _M_map + (_M_map_size - new_num_nodes) / 2
                            + (add_to_front ? nodes_to_add : 0);
            if (new_nstart < _M_start.node)
                std::copy(_M_start.node, _M_finish.node + 1, new_nstart);
            else 
                std::copy_backward(_M_start.node, _M_finish.node + 1, new_nstart + old_num_nodes);
        } 
        else {
            size_type new_map_size = _M_map_size + std::max(_M_map_size, nodes_to_add) + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_nstart = new_map + (_M_map_size - new_num_nodes) / 2
                                + (add_to_front ? nodes_to_add : 0);
            std::copy(_M_start.node, _M_finish.node + 1, new_nstart);
            map_allocator::deallocate(_M_map, _M_map_size);
            _M_map = new_map;
            _M_map_size = new_map_size;
        }
        _M_start.set_node(new_nstart);
        _M_finish.set_node(new_nstart + old_num_nodes -1 );
    }

    // 重载比较操作符
    template <class T>
    bool operator==(const deque<T>& lhs, const deque<T>& rhs)
    {
        return lhs.size() == rhs.size() && 
            my_stl::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class T>
    bool operator<(const deque<T>& lhs, const deque<T>& rhs)
    {
        return my_stl::lexicographical_compare(
            lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class T>
    bool operator!=(const deque<T>& lhs, const deque<T>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class T>
    bool operator>(const deque<T>& lhs, const deque<T>& rhs)
    {
        return rhs < lhs;
    }

    template <class T>
    bool operator<=(const deque<T>& lhs, const deque<T>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class T>
    bool operator>=(const deque<T>& lhs, const deque<T>& rhs)
    {
        return !(lhs < rhs);
    }

    // 重载 swap 针对deque参数类型
    template <class T>
    void swap(deque<T>& lhs, deque<T>& rhs)
    {
        lhs.swap(rhs);
    }
} // /* namespace my_stl */

#endif /* XT_STL_DEQUE_H */