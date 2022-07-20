

#ifndef __SGI_STL_INTERNAL_LIST_H
#define __SGI_STL_INTERNAL_LIST_H

#include "stl_iterator.h"
#include "stl_alloc.h"
#include "stl_contruct.h"

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

        link_type node;

        __list_iterator(link_type x) : node(x) {}
        __list_iterator() {}
        __list_iterator(const iterator& x) : node(x.node) {};

        bool operator== (const self& x) const {
            return node == x.node;
        }
        bool operator!= (const self& x) const {
            return node != x.node;
        }

        reference operator*() const { return (*node).data; }
        pointer operator->() const { return &(operator*()); }

        self& operator++() {
            node = (link_type)((*node).next);
            return *this;
        }
        self operator++(int) {
            self tmp = *this; //这个不会使用到重载的*吗？
            ++*this;
            return tmp;
        }

        self& operator--() {
            node = (link_type)((*node).prev);
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
        protected:
        typedef __list_node<T> list_node;
        typedef simple_alloc<list_node, Alloc> list_node_allocator;

        public:
        typedef T value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef list_node* link_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        protected:
        link_type node;

        public:
        typedef __list_iterator<T, T&, T*>  iterator;
        typedef __list_iterator<T, const T&, const T*>  const_iterator; 


        // 基础操作
        public:
        iterator begin() const { return (*node).next;}
        iterator end() const { return node;}
        bool empty() const { return node->next == node; }       
        size_type size() const {
            size_type result = 0;
            distance(begin(), end(), result); // 需要重载这个函数
            return result;
        }
        reference front() { return *begin(); }
        reference back() { return *(--end()); }

        // 空间管理
        protected:
        link_type get_node() { return list_node_allocator::allocate(); }
        void put_node(link_type p) { list_node_allocator::deallocate(p); }

        link_type create_node(const T& x) {
            link_type p = get_node();
            construct(&p->data, x);
            return p;
        }
        void destroy_node(link_type p) {
            destroy(&p->data);
            put_node(p);
        }

        protected:
        void empty_initialize() {
            node = get_node();
            node->next = node;
            node->prev = node;
        }

        public:
        list() { empty_initialize(); }


        public:
        iterator insert(iterator position, const T& x) {
            link_type tmp  = create_node(x);
            tmp->prev = position.node->prev;
            tmp->next = position.node;
            (position.node->prev)->next = tmp;
            position.node->prev = tmp;
            return tmp;
        }

        void push_back(const T& x) {
            insert(end(), x);
        }

        void push_front(const T& x) {
            insert(begin(), x);
        }

        iterator erase(iterator position) 
        {
            /*
                删除node节点哨兵会导致错误
                但保证不是node节点应该由使用者负责
            */
            link_type next_node = position.node->next;
            position.node->prev->next = next_node;
            next_node->prev = position.node->prev;
            destroy_node(position.node);
            return iterator(next_node);
        }

        void pop_frnot() { erase(begin()); }
        void pop_back() {
            iterator tmp = end();
            erase(--tmp);
        }

        void clear() {
            link_type cur = node->next;
            while(cur != node) {
                link_type tmp = cur;
                cur = cur->next;
                destroy_node(tmp);
            }
            node->next = node;
            node->prev = node;
        }

        // 改写原版 利用erase函数返回值递增
        void remove(const T& value) {
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

        // 改写原版 利用erase函数返回值递增first
        void unique() {
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

        /* 原版 unique()
        void unique() {
            iterator first = begin();
            iterator last = end();
            if (first == last) return;
            iterator next = first;
            while (++next != last) {
                if (*first == *next)
                erase(next);
                else
                first = next;
                next = first;
            }
        }
        */

        // protected: 
        public:
        void transfer (iterator position, iterator first, iterator last) {
            // 前三步处理三个迭代器前节点的next
            position.node->prev->next = first.node;
            first.node->prev->next = last.node;
            last.node->prev->next = position.node;
            // 后四步处理三个迭代器的prev, 三者交换prev需要一个临时变量
            link_type tmp = position.node->prev;
            position.node->prev = last.node->prev;
            last.node->prev = first.node->prev;
            first.node->prev = tmp;
        }     
        

        
    };
}

#endif /* __SGI_STL_INTERNAL_LIST_H */