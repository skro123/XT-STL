
#ifndef __SGI_STL_INTERNAL_DEQUE_H
#define __SGI_STL_INTERNAL_DEQUE_H

#include "stl_iterator.h"
#include "stl_alloc.h"
#include "stl_uninitialized.h"
#include "stl_contruct.h"

namespace my_stl {

    inline size_t __deque_buf_size(size_t n, size_t sz) {
        return n != 0 ? n : (sz < 512 ? size_t(512/sz) : size_t(1));
    }
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

    template <class T, class Alloc = alloc, size_t BufSize = 0>
    class deque {
        public:
        // 基础类型
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        public:
        typedef __deque_iterator<T, T&, T*, BufSize> iterator;

        protected:
        typedef pointer* map_pointer;

        // 数据成员
        protected:
        iterator start;
        iterator finish;
        map_pointer map;
        size_type map_size;

        public:
        iterator begin() { return start; }
        iterator end() { return finish; }
        reference operator[] (size_type n) {
            return start[difference_type(n)];
        }
        
        reference front() { return *start; }
        reference back() {
            iterator tmp = finish;
            --tmp;
            return *tmp;
        }
        size_type size() const { return finish - start;}
        size_type max_size() const { return size_type(-1);}
        bool empty() const { return start == finish;}

        protected:
        typedef simple_alloc<value_type, Alloc> data_allocator;
        typedef simple_alloc<pointer, Alloc> map_allocator;

        static size_type buffer_size() {
            return __deque_buf_size(BufSize, sizeof(value_type));
        }
        static size_type initial_map_size() { return 8; }
        
        protected:
        pointer allocate_node() { return data_allocator::allocate(buffer_size()); }
        void deallocate_node(pointer p) { data_allocator::deallocate(p, buffer_size()); }

        public:
        void create_map_and_nodes(size_type num_elements);
        void destroy_map_and_nodes();
        void fill_initialized(size_type n, const value_type& value);
        deque(int n, const value_type& value)
        : start(), finish(), map(0), map_size(0)
        {
            fill_initialized(n, value);
        }
        deque()
        : start(), finish(), map(0), map_size(0)
        {
            create_map_and_nodes(0);
        }

        ~deque() {
            // 先析构所有数据元素，以防其中有指针
            destroy(start, finish);
            destroy_map_and_nodes();

        }

        void reallocate_map(size_type nodes_to_add, bool add_at_front);
        void reserve_map_at_back(size_type nodes_to_add = 1) {
            if (nodes_to_add + 1 > map_size - (finish.node - start.node)) {
                reallocate_map(nodes_to_add, false);
            }
        }
        void reserve_map_at_front(size_type nodes_to_add = 1) {
            if (nodes_to_add > start.node - map) {
                reallocate_map(nodes_to_add, true);
            }
        }

        void push_back_aux(const value_type& t);
        void push_back(const value_type& t) {
            if (finish.cur != finish.last - 1)
            {
                construct(finish.cur, t);
                ++finish.cur;
            }
            else
                push_back_aux(t);
        }

        void push_front_aux(const value_type& t);
        void push_front(const value_type& t) {
            if (start.cur != start.first) {
                construct(start.cur - 1, t);
                --start.cur;
            }
            else
                push_front_aux(t);
        }

        void pop_back_aux();
        void pop_back() {
            // finish.cur 是不可达的 前闭后开
            if (finish.cur != finish.first) {
                --finish.cur; // 这里使用的是原始指针的--运算符
                destroy(finish.cur);
            }
            else
                pop_back_aux();
        }

        void pop_front_aux();
        void pop_front() {
            if (start.cur != start.last - 1) {
                destroy(start.cur);
                ++start.cur;
            }
            else
                pop_front_aux();
        }
        void clear();
        iterator earse(iterator pos);
        iterator earse(iterator first, iterator last);

        iterator insert_aux(iterator pos, const value_type& x);
        iterator insert(iterator pos, const value_type& x) {
            if (pos.cur == start.cur) {
                push_front(x);
                return start;
            }
            else if (pos.cur == finish.cur) {
                push_back(x);
                iterator tmp = finish;
                return --tmp;
            }
            else {
                return insert_aux(pos, x);
            }
        }
    };

    template <class T, class Alloc, size_t BufSiz>
    typename deque<T, Alloc, BufSiz>::iterator
    deque<T, Alloc, BufSiz>::
    insert_aux(iterator pos, const value_type& x) {
        // std::cout << "in insert_aux" << std::endl;
        difference_type index = pos - start;
        value_type x_copy = x;
        // std::cout << "index:" << index << std::endl;
        if (index < (size() / 2)) {
            // std::cout << "in insert_aux index < size() >> 1" << std::endl;
            push_front(front());
            iterator front1 = start + 1;
            iterator front2 = front1 + 1;
            pos = start + index;
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
            iterator back1 = finish - 1;
            iterator back2 = back1 - 1;
            pos = start + index;
            std::copy_backward(pos, back2, back1);
            // std::cout << "in insert_aux else out" << std::endl;
        }
        *pos = x_copy;
        return pos;
    }    

    template <class T, class Alloc, size_t BufSiz>
    typename deque<T, Alloc, BufSiz>::iterator
    deque<T, Alloc, BufSiz>::
    earse(iterator pos) {
        iterator next = pos;
        ++next;
        difference_type index = pos - start;
        if (index < size() >> 1) {
            // copy_backward的目的地址也符合前闭后开
            std::copy_backward(start, pos, next);
            pop_front();
        }
        else {
            std::copy(next, finish, pos);
            pop_back();
        }
        return start + index;
    }

    template <class T, class Alloc, size_t BufSiz>
    typename deque<T, Alloc, BufSiz>::iterator
    deque<T, Alloc, BufSiz>::
    earse(iterator first, iterator last) {
        if (first == start && last == finish) {
            clear();
            return finish;
        }
        else {
            difference_type n = last - first;
            difference_type elems_before = first - start;
            if (elems_before < (size() - n) / 2) {
                std::copy_backward(start, first, last);
                iterator new_start = start + n;
                destroy(start, new_start);
                for(map_pointer cur = start.node; cur < new_start.node; ++cur) {
                    map_allocator::deallocate(*cur, buffer_size());
                }
                start = new_start;
            } 
            else {
                copy(last, finish, first);
                iterator new_finish = finish - n;
                destroy(new_finish, finish);
                for(map_pointer cur = new_finish.node; cur < finish.node; ++cur) {
                    map_allocator::deallocate(*cur, buffer_size());
                }
                finish = new_finish;
            }
            return start + elems_before;
        }
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    clear() {
        for (map_pointer node = start.node + 1; node < finish.node; ++node) {
            destroy(*node, *node + buffer_size() );
            data_allocator::deallocate(*node, buffer_size());
        }
        if (start.node != finish.node) {
            destroy(start.cur, start.last);
            destroy(finish.first, finish.cur);
            data_allocator::deallocate(finish.first, buffer_size());
        }
        else
            destroy(start.cur, finish.cur); // 注意这里空间可能不是满的
        finish = start;
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    pop_back_aux() {
        deallocate_node(finish.first);
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
        destroy(finish.cur);
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    pop_front_aux() {
        destroy(start.cur);
        deallocate_node(start.first);
        start.set_node(start.node + 1);
        start.cur = start.first;
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    push_back_aux(const value_type& t) {
        value_type t_copy = t;
        reserve_map_at_back();
        *(finish.node + 1) = allocate_node();
        try {
            construct(finish.cur, t_copy);
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }
        catch(...) {
            std::cout<< "push_back_aux expection" << std::endl;

            deallocate_node(*(finish.node + 1));
            throw;
        }
    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    push_front_aux(const value_type& t) {
        value_type t_copy = t;
        reserve_map_at_front();
        *(start.node - 1) = allocate_node();
        try {
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            construct(start.cur, t_copy);
        }
        catch (...) {
            std::cout<< "push_front_aux expection" << std::endl;

            start.set_node(start.node + 1);
            start.cur = start.first;
            deallocate_node(*(start.node - 1));
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
            for (cur = start.node; cur < finish.node; ++cur) {
                uninitialized_fill(*cur, *cur + buffer_size(), value);
            }
            uninitialized_fill(finish.first, finish.cur, value);       
        }
        catch (...) {
            std::cout<< "fill_initialized expection" << std::endl;

            destroy_map_and_nodes();
        }

    }

    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    destroy_map_and_nodes()
    {
        for (map_pointer cur = start.node; cur != finish.node; ++cur) {
            deallocate_node(*cur);
        }
        map_allocator::deallocate(map, map_size);
    }

    template <class T, class Alloc, size_t BufSiz>
    void 
    deque<T, Alloc, BufSiz>::
    create_map_and_nodes(size_type num_elements)
    {
        size_type num_nodes = num_elements / buffer_size() + 1;
        map_size = std::max(initial_map_size(), num_nodes + 2);
        map = map_allocator::allocate(map_size);

        map_pointer nstart = map + (map_size - num_nodes) / 2;
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
        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        finish.cur  = finish.first + num_elements % buffer_size();
    }
    template <class T, class Alloc, size_t BufSiz>
    void
    deque<T, Alloc, BufSiz>::
    reallocate_map(size_type nodes_to_add, bool add_to_front) {
        size_type old_num_nodes = finish.node - start.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;

        map_pointer new_nstart;
        if (map_size > 2 * new_num_nodes) {
            new_nstart = map + (map_size - new_num_nodes) / 2
                            + (add_to_front ? nodes_to_add : 0);
            if (new_nstart < start.node)
                std::copy(start.node, finish.node + 1, new_nstart);
            else 
                std::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
        } 
        else {
            size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_nstart = new_map + (map_size - new_num_nodes) / 2
                                + (add_to_front ? nodes_to_add : 0);
            std::copy(start.node, finish.node + 1, new_nstart);
            map_allocator::deallocate(map, map_size);
            map = new_map;
            map_size = new_map_size;
        }
        start.set_node(new_nstart);
        finish.set_node(new_nstart + old_num_nodes -1 );
    }
}

#endif /* __SGI_STL_INTERNAL_DEQUE_H */