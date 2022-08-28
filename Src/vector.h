
#ifndef XT_STL_VECTOR_H
#define XT_STL_VECTOR_H

#include <cstddef>
#include "alloc.h"
#include "algobase.h"
#include "uninitialized.h"
#include "contruct.h"
#include "util.h"

namespace my_stl
{
    template <class T, class Alloc = alloc>
    class vector {
        /*-------------------内嵌型别定义--------------------*/
        public:
            // 五种基本的+const的
            typedef T                   value_type;
            typedef value_type*         pointer;
            typedef const value_type*   const_pointer;
            typedef value_type&         reference;
            typedef const value_type&   const_reference;
            typedef size_t              size_type;
            typedef ptrdiff_t           difference_type;
            // 迭代器的
            typedef value_type*         iterator;
            typedef const value_type*   const_iterator;
            // 空间分配器的
            typedef simple_alloc<T, Alloc> data_allocator;

        /*-------------------数据成员--------------------*/
        private:
            iterator _M_start;         //指向分配空间开始
            iterator _M_finish;        //指向已用空间结尾
            iterator _M_end_of_storage;//指向分配空间结尾


        /*-------------------函数成员-构造析构--------------------*/
        public:
            // 默认分配16个元素大小的空间，分配失败则设为nullptr
            vector() noexcept 
            {init_try();}

            // 分配n个元素大小的空间并支持使用元素默认构造函数或指定值初始化
            // explicit阻止size_type类型的变量隐式转换 
            explicit vector(size_type n) {fill_initialize(n, value_type());}
            vector(size_type n, const value_type& value) {fill_initialize(n, value); }
            vector(int n, const value_type& value) {fill_initialize(n, value);}
            vector(long n, const value_type& value) {fill_initialize(n, value);}

            // 用两个迭代器范围构造
            template <class Iteraotr>
            vector(Iteraotr first, Iteraotr last)
            {
                range_initialize(first, last);
            }

            // 用初始化列表初始化
            vector(std::initializer_list<value_type> ilist)
            {
                range_initialize(ilist.begin(), ilist.end());
            }

            // 拷贝构造函数
            vector(const vector& rhs) 
            {
                range_initialize(rhs.begin(), rhs.end());
            }

            // 移动构造函数
            // Todo 这里rhs 能直接使用vector的私有成员？？
            vector(vector&& rhs) noexcept
            :_M_start(rhs._M_start),
            _M_finish(rhs._M_finish),
            _M_end_of_storage(rhs._M_end_of_storage)
            {
                rhs._M_start = nullptr;
                rhs._M_finish = nullptr;
                rhs._M_end_of_storage = nullptr;
            }

        /*-------------------函数成员-析构函数--------------------*/
            // 1.析构每一个元素；2.释放空间。
            ~vector() 
            {
                destroy(_M_start, _M_finish);
                deallocate();
            }   

        /*-------------------函数成员-赋值运算符重载--------------------*/
        public:

            // 拷贝赋值函数
            vector& operator=(const vector& rhs)
            {
                // 比较地址是否相同 排除自我复制
                if (this != &rhs)
                {
                    const size_type r_Len = rhs.size();
                    // 分为三种情况处理 提高效率 
                    if (r_Len > capacity())
                    {
                        // 第一种 rhs的元素数量大于该对象容量 
                        // 直接新分配一个 同时采用异常安全的形式
                        // 该tmp对象实际上只有三个指针 不会占用太多栈空间
                        vector tmp(rhs.begin(), rhs.end());
                        swap(tmp);
                    }
                    else if (r_Len > this->size())
                    {
                        // 第二中情况 rhs中的元素数量大于该对象已使用空间大小但总空间能容下
                        // 前一段直接复制 另一段未使用的空间使用rhs对应位置元素构造出
                        my_stl::copy(rhs.begin(), rhs.begin() + size(), _M_start);
                        my_stl::uninitialized_copy(rhs.begin() + size(), rhs.end(), _M_finish);
                        _M_finish = _M_start + r_Len;
                    }
                    else
                    {
                        // 最后一种情况， rhs中的元素数量小于该对象的元素数量
                        // rhs的所有元素赋值到该对象，并将该对象多余部分析构
                        auto index = my_stl::copy(rhs.begin(), rhs.end(), _M_start);
                        my_stl::destroy(index, _M_finish);
                        _M_finish = _M_start + r_Len;
                    }
                }
                return *this;
            }

            // 移动赋值函数
            vector& operator=(vector&& rhs) noexcept
            {
                destroy(_M_start, _M_finish);
                data_allocator::deallocate(_M_start, capacity());
                this->_M_start = rhs._M_start;
                this->_M_finish = rhs._M_finish;
                this->_M_end_of_storage = rhs._M_end_of_storage;
                rhs._M_start = nullptr;
                rhs._M_finish = nullptr;
                rhs._M_end_of_storage = nullptr;
                return *this;
            }

            // 初始化列表赋值函数
            vector& operator=(std::initializer_list<value_type> ilist) noexcept
            {
                vector tmp(ilist.begin(), ilist.end());
                swap(tmp);
                return *this;
            }

        /*-------------------函数成员-辅助函数-空间分配及初始化相关----------------*/
        protected:

            // 分配空间并用指定值填充
            iterator allocate_and_fill(size_type n, const T& x) 
            {
                iterator result = data_allocator::allocate(n);
                uninitialized_fill_n(result, n, x);
                return result;
            }


            void init_try()
            {
                try {
                    _M_start = data_allocator::allocate(16);
                    _M_finish = _M_start;
                    _M_end_of_storage = _M_start + 16;
                }
                catch (...) {
                    _M_start = nullptr;
                    _M_finish = nullptr;
                    _M_end_of_storage = nullptr;
                }
            }
            void init_space(size_type size, size_type cap)
            {
                try
                {
                    _M_start = data_allocator::allocate(cap);
                    _M_finish = _M_start + size;
                    _M_end_of_storage = _M_start + cap;
                }
                catch (...)
                {
                    _M_start = nullptr;
                    _M_finish = nullptr;
                    _M_end_of_storage = nullptr;
                    throw;
                }
            }
            template <class iterator>
            void range_initialize(iterator first, iterator last)
            {
                const size_type init_size = my_stl::max(static_cast<size_type>(last - first), static_cast<size_type>(16));
                init_space(static_cast<size_type>(last - first), init_size);
                my_stl::uninitialized_copy(first, last, _M_start);
            }
            void fill_initialize(size_type n, const T& value) 
            {
                _M_start = allocate_and_fill(n, value);
                _M_finish = _M_start + n;
                _M_end_of_storage = _M_finish;
            }
            void deallocate() 
            {
                if(_M_start) {
                    data_allocator::deallocate(_M_start, _M_end_of_storage - _M_start);
                }
            }

        /*----------------成员函数-迭代器相关操作------------------*/
        public:
            iterator            begin()             noexcept
            { return _M_start;}
            iterator            begin()     const   noexcept
            { return _M_start;}

            iterator            end()               noexcept
            { return _M_finish;}
            iterator            end()       const   noexcept
            { return _M_finish;}

            const_iterator      cbegin()    const   noexcept
            { return begin();}
            const_iterator      cend()      const   noexcept
            { return end();}

        /*----------------成员函数-容量相关操作------------------*/
        public:
            size_type           size()      const   noexcept
            { return size_type(end() - begin());}
            size_type           capacity()  const   noexcept
            { return size_type(_M_end_of_storage - begin());}
            size_type           max_size()  const   noexcept
            { return static_cast<size_type>(-1) / sizeof(T);}
            bool                empty()     const   noexcept
            { return begin() == end();} 
            


        /*----------------成员函数-元素访问相关操作------------------*/
        public:
            reference operator[] (size_type n)
            { return *(begin() + n);}
            const_reference operator[]  (size_type n) const
            { return *(begin() + n);} 
            reference front() 
            { return *begin(); }
            const_reference front() const
            { return *begin(); }
            reference back() 
            { return *(end() - 1);}
            const_reference back() const
            { return *(end() - 1);}
            reference at(size_type n)
            { return (*this)[n];}
            const_reference at(size_type n) const
            { return (*this)[n];}
            pointer       data()       noexcept 
            { return _M_start; }
            const_pointer data() const noexcept 
            { return _M_start; }

        /*----------------成员函数-修改容器相关操作------------------*/
        public:
            void push_back(const T& x) 
            {
                if(_M_finish != _M_end_of_storage) 
                {
                    construct(_M_finish, x);
                    ++_M_finish;
                } 
                else 
                {
                    insert_aux(end(), x);
                }
            }

            void pop_back() 
            {
                destroy(_M_finish - 1);
                --_M_finish;
            }

            void insert_aux(iterator position, const T& x);
            void insert(iterator position, size_type n, const T& x);

            iterator erase(iterator first, iterator last) 
            {
                iterator i = my_stl::copy(last, _M_finish, first);
                destroy(i, _M_finish);
                _M_finish = _M_finish - (last - first);
                return first;
            }
            iterator erase(iterator position) 
            {
                if(position + 1 != end())
                    my_stl::copy(position + 1, _M_finish, position);
                --_M_finish;
                destroy(_M_finish);
                return position;
            }

            void clear() 
            { erase(begin(), end()); }

            void resize(size_type new_size, const value_type& x) 
            {
                if (new_size < size())
                    erase(begin() + new_size, end());
                else 
                    insert(end(), new_size - size(), x);
            }
            void resize(size_type new_size) 
            { resize(new_size, value_type()); }

            void reserve(size_type n)
            {
                if (capacity() < n)
                {
                    const auto old_size = size();
                    auto tmp = data_allocator::allocate(n);
                    my_stl::uninitialized_copy(_M_start, _M_finish, tmp);
                    data_allocator::deallocate(_M_start, _M_end_of_storage - _M_start);
                    _M_start = tmp;
                    _M_finish = tmp + old_size;
                    _M_end_of_storage = _M_start + n;
                }
            }

            // 与另一个 vector 交换
            void swap(vector& rhs) noexcept;

    }; /* class vector */


    // 与另一个 vector 交换
    template <class T, class Alloc>
    void 
    vector<T, Alloc>::swap(vector<T, Alloc>& rhs) noexcept
    {
        if (this != &rhs)
        {
            my_stl::swap(_M_start, rhs._M_start);
            my_stl::swap(_M_finish, rhs._M_finish);
            my_stl::swap(_M_end_of_storage, rhs._M_end_of_storage);
        }
    }
    
    template <class T, class Alloc>
    void
    vector<T, Alloc>::insert_aux(iterator position, const T& x) 
    {
        if(_M_finish != _M_end_of_storage) {
            construct(_M_finish, *(_M_finish - 1));
            ++_M_finish;
            T x_copy = x;
            std::copy_backward(position, _M_finish - 2, _M_finish - 1);
            *position = x_copy;
        } else {
            const size_type old_size = size();
            const size_type len = old_size != 0 ? 2 * old_size : 1;
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            try {
                new_finish = uninitialized_copy(_M_start, position, new_start);
                construct(new_finish, x);
                ++new_finish;
                new_finish = uninitialized_copy(position, _M_finish, new_finish);
            } 
            catch(...) {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                throw;
            }

            destroy(begin(), end());
            deallocate();

            _M_start = new_start;
            _M_finish = new_finish;
            _M_end_of_storage = new_start + len;
        }
    }

    template <class T, class Alloc>
    void
    vector<T, Alloc>::insert(iterator position, size_type n, const T& x) 
    {
        if (n != 0) {
            if(size_type(_M_end_of_storage - _M_finish) >= n) {
                T x_copy = x;
                const size_type elems_after = _M_finish - position;
                iterator old_finish = _M_finish;
                if(elems_after > n) {
                    uninitialized_copy(_M_finish - n, _M_finish, _M_finish);
                    _M_finish += n;
                    std::copy_backward(position, old_finish - n, old_finish);
                    std::fill(position, position + n, x_copy);
                } else {
                    uninitialized_fill_n(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    uninitialized_copy(position, old_finish, _M_finish);
                    _M_finish += elems_after;
                    std::fill(position, old_finish, x_copy);
                }
            }
        
            else {
                const size_type old_size = size();
                const size_type len = old_size + (old_size > n? old_size : n);
                iterator new_start = data_allocator::allocate(len);
                iterator new_finish = new_start;
                try {
                    new_finish = uninitialized_copy(_M_start, position, new_start);
                    new_finish = uninitialized_fill_n(new_finish, n, x);
                    new_finish = uninitialized_copy(position, _M_finish, new_finish);
                }
                catch (...) {
                    destroy(new_start, new_finish);
                    data_allocator::deallocate(new_start, len);
                    throw;
                }
                destroy(_M_start, _M_finish);
                deallocate();

                _M_start = new_start;
                _M_finish = new_finish;
                _M_end_of_storage = new_start + len;
            }
        }
    } 

    //-------------------重载与vector有关的逻辑运算符-----------------------//
    
    template <class T>
    bool operator==(const vector<T>& lhs, const vector<T>& rhs)
    {
        return lhs.size() == rhs.size() &&
        my_stl::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class T>
    bool operator<(const vector<T>& lhs, const vector<T>& rhs)
    {
        return my_stl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), lhs.end());
    }

    template <class T>
    bool operator!=(const vector<T>& lhs, const vector<T>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class T>
    bool operator>(const vector<T>& lhs, const vector<T>& rhs)
    {
        return rhs < lhs;
    }

    template <class T>
    bool operator<=(const vector<T>& lhs, const vector<T>& rhs)
    {
        return !(rhs < lhs);
    }

    template <class T>
    bool operator>=(const vector<T>& lhs, const vector<T>& rhs)
    {
        return !(lhs < rhs);
    }

    // 对 vector 重载swap
    template <class T, class Alloc>
    void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) noexcept
    {
        // todo 是否需要像MyTinySTL中使用右值引用？？
        lhs.swap(rhs);
    }
} /* my_stl */

#endif /* XTSTL_VECTOR_H */
