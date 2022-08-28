#ifndef XT_STL_HASH_SET_H
#define XT_STL_HASH_SET_H

#include "hashtable.h"
#include "pair.h"

namespace my_stl
{
    template <class _Value, class _HashFcn = my_stl::hash<_Value>, class _EqualKey = my_stl::equal_to<_Value>, class _Alloc = alloc>
    class hash_set
    {
        // 型别声明
        public:
            typedef my_stl::hashtable<_Value, _Value, _HashFcn, identity<_Value>, _EqualKey, _Alloc> rep_type;
            typedef typename rep_type::key_type key_type;
            typedef typename rep_type::value_type value_type;
            typedef typename rep_type::hasher hasher;
            typedef typename rep_type::key_equal key_equal;

            typedef typename rep_type::size_type size_type;
            typedef typename rep_type::difference_type difference_type;
            typedef typename rep_type::const_pointer pointer;
            typedef typename rep_type::const_pointer const_pointer;
            typedef typename rep_type::const_reference reference;
            typedef typename rep_type::const_reference const_reference;

            typedef typename rep_type::const_iterator iterator;
            typedef typename rep_type::const_iterator const_iterator;

        // 数据成员 底层容器为hashtable
        private:
            rep_type _M_rep;

        // 构造、析构和赋值
        public:
            hash_set() : _M_rep(100, hasher(), key_equal()) {}
            explicit hash_set(size_type n) : _M_rep(n, hasher(), key_equal()) {}
            hash_set(size_type n, const hasher& hf) : _M_rep(n, hf, key_equal()) {}
            hash_set(size_type n, const hasher& hf, const key_equal& eql)
              : _M_rep(n, hf, eql) 
            {}
            template <class InputIterator>
            hash_set(InputIterator first, InputIterator last)
              : _M_rep(100, hasher(), key_equal()) 
            { _M_rep.insert_unique(first, last); }

            template <class InputIterator>
            hash_set(InputIterator first, InputIterator last, size_type n)
              : _M_rep(n, hasher(), key_equal()) 
            { _M_rep.insert_unique(first, last); }

            template <class InputIterator>
            hash_set(InputIterator first, InputIterator last, size_type n, const hasher& hf)
              : _M_rep(n, hf, key_equal()) { _M_rep.insert_unique(first, last); }

            template <class InputIterator>
            hash_set(InputIterator first, InputIterator last, size_type n, const hasher& hf, const key_equal& eql)
              : _M_rep(n, hf, eql) { _M_rep.insert_unique(first, last); }
            // 只有一个数据成员对象 默认的析构会调用底层容器的析构
            ~hash_set() = default;
            
        // 迭代器相关
        public:
            iterator begin() const { return this->_M_rep.begin(); }
            iterator end() const { return this->_M_rep.end(); }
        
        // 容量相关
        public:
            size_type size() const { return this->_M_rep.size(); }
            size_type max_size() const { return this->_M_rep.max_size(); }
            bool empty() const { return this->_M_rep.empty(); }
            void resize(size_type hint) { _M_rep.resize(hint); }
            size_type bucket_count() const { return _M_rep.bucket_count(); }
            size_type max_bucket_count() const { return _M_rep.max_bucket_count(); }
            size_type elems_in_bucket(size_type n) const { return _M_rep.elems_in_bucket(n); }

        // set相关操作
        public:
            // 插入
            // set中键不可重复 使用insert_unique
            pair<iterator, bool> insert(const value_type& obj)
            {
                pair<typename rep_type::iterator, bool> p = _M_rep.insert_unique(obj);
                return pair<iterator, bool>(p.first, p.second);
            }
            template <class InputIterator>
            void insert(InputIterator first, InputIterator last) 
            { _M_rep.insert_unique(first,last); }
            pair<iterator, bool> insert_noresize(const value_type& obj)
            {
                pair<typename rep_type::iterator, bool> p = _M_rep.insert_unique_noresize(obj);
                return pair<iterator, bool>(p.first, p.second);
            }

            // 查找和计数
            iterator find(const key_type& key) const 
            { return _M_rep.find(key); }

            size_type count(const key_type& key) const 
            { return _M_rep.count(key); }
    
            pair<iterator, iterator> equal_range(const key_type& key) const
            { return _M_rep.equal_range(key); }

            // 删除
            size_type erase(const key_type& key) 
            { return _M_rep.erase(key); }
            void erase(iterator it) 
            { _M_rep.erase(it); }
            void erase(iterator first, iterator last) 
            { _M_rep.erase(first, last); }

            // 清除
            void clear() { _M_rep.clear(); }

        public:
            void swap(hash_set& rhs) { _M_rep.swap(rhs._M_rep); }
            bool operator== (const hash_set& rhs) { return this->_M_rep == rhs._M_rep;}                
    };

    // 重载比较运算符
    template <class _Value, class _HashFcn, class _EqualKey, class _Alloc>
    inline bool operator==(const hash_set<_Value, _HashFcn, _EqualKey, _Alloc>& lhs,
                        const hash_set<_Value, _HashFcn, _EqualKey, _Alloc>& rhs)
    {
        return lhs._M_rep == rhs._M_rep;
    }
    // 重载swap 对于hash_set类型
    template <class Val, class _HashFcn, class _EqualKey, class _Alloc>
    inline void swap(hash_set<Val, _HashFcn, _EqualKey, _Alloc>& lsh,
                    hash_set<Val, _HashFcn, _EqualKey, _Alloc>& rhs) 
    {
        lsh.swap(rhs);
    }




    template <class _Value, class _HashFcn = hash<_Value>, class _EqualKey = equal_to<_Value>, class _Alloc = alloc>
    class hash_multiset
    {
        // 型别声明
        public:
            typedef hashtable<_Value, _Value, _HashFcn, identity<_Value>, _EqualKey, _Alloc> rep_type;
            typedef typename rep_type::key_type key_type;
            typedef typename rep_type::value_type value_type;
            typedef typename rep_type::hasher hasher;
            typedef typename rep_type::key_equal key_equal;

            typedef typename rep_type::size_type size_type;
            typedef typename rep_type::difference_type difference_type;
            typedef typename rep_type::const_pointer pointer;
            typedef typename rep_type::const_pointer const_pointer;
            typedef typename rep_type::const_reference reference;
            typedef typename rep_type::const_reference const_reference;

            typedef typename rep_type::const_iterator iterator;
            typedef typename rep_type::const_iterator const_iterator;

        // 数据成员 底层容器使用hashtable承载
        private:
            rep_type _M_rep;

        // 构造、析构和赋值
        public:
            hash_multiset() : _M_rep(100, hasher(), key_equal()) {}
            explicit hash_multiset(size_type n) : _M_rep(n, hasher(), key_equal()) {}
            hash_multiset(size_type n, const hasher& hf) : _M_rep(n, hf, key_equal()) {}
            hash_multiset(size_type n, const hasher& hf, const key_equal& eql)
              : _M_rep(n, hf, eql) 
            {}

            template <class InputIterator>
            hash_multiset(InputIterator first, InputIterator last)
              : _M_rep(100, hasher(), key_equal()) 
            { _M_rep.insert_equal(first, last); }

            template <class InputIterator>
            hash_multiset(InputIterator first, InputIterator last, size_type n)
              : _M_rep(n, hasher(), key_equal()) 
            { _M_rep.insert_equal(first, last); }

            template <class InputIterator>
            hash_multiset(InputIterator first, InputIterator last, size_type n, const hasher& hf)
              : _M_rep(n, hf, key_equal()) 
            { _M_rep.insert_equal(first, last); }

            template <class InputIterator>
            hash_multiset(InputIterator first, InputIterator last, size_type n, const hasher& hf, const key_equal& eql)
              : _M_rep(n, hf, eql) 
            { _M_rep.insert_equal(first, last); }
            ~hash_multiset() = default;

        // 容量相关
        public:
            size_type size() const { return _M_rep.size(); }
            size_type max_size() const { return _M_rep.max_size(); }
            bool empty() const { return _M_rep.empty(); }
            void resize(size_type hint) { _M_rep.resize(hint); }
            size_type bucket_count() const { return _M_rep.bucket_count(); }
            size_type max_bucket_count() const { return _M_rep.max_bucket_count(); }
            size_type elems_in_bucket(size_type n) const { return _M_rep.elems_in_bucket(n); }
        
        // 迭代器相关
        public:
            iterator begin() const { return _M_rep.begin(); }
            iterator end() const { return _M_rep.end(); }

        // 容器相关操作
        public:
            // 插入
            iterator insert(const value_type& obj) 
            { return _M_rep.insert_equal(obj); }
            template <class InputIterator>
            void insert(InputIterator first, InputIterator last) 
            { _M_rep.insert_equal(first,last); }
            iterator insert_noresize(const value_type& obj)
            { return _M_rep.insert_equal_noresize(obj); }    

            // 查找 和 计数
            iterator find(const key_type& key) const 
            { return _M_rep.find(key); }

            size_type count(const key_type& key) const 
            { return _M_rep.count(key); }
            
            pair<iterator, iterator> equal_range(const key_type& key) const
            { return _M_rep.equal_range(key); }
            
            // 删除
            size_type erase(const key_type& key) {return _M_rep.erase(key); }
            void erase(iterator it) { _M_rep.erase(it); }
            void erase(iterator first, iterator last) { _M_rep.erase(first, last); }

            // 清除
            void clear() { _M_rep.clear(); }

        public:
            void swap(hash_multiset& rhs) { _M_rep.swap(rhs._M_rep); }
            bool operator== (const hash_multiset& rhs)
            {
                return this->_M_rep = rhs._M_rep;
            }
    };

    template <class Val, class _HashFcn, class _EqualKey, class _Alloc>
    inline bool operator==(const hash_multiset<Val, _HashFcn, _EqualKey, _Alloc>& lhs,
                        const hash_multiset<Val, _HashFcn, _EqualKey, _Alloc>& rhs)
    {
        return lhs == rhs;
    }


    template <class Val, class _HashFcn, class _EqualKey, class _Alloc>
    inline void swap(hash_multiset<Val, _HashFcn, _EqualKey, _Alloc>& lhs,
                    hash_multiset<Val, _HashFcn, _EqualKey, _Alloc>& rhs)
    {
        lhs.swap(rhs);
    }


}
#endif /* XT_STL_HASH_SET_H */

// Local Variables:
// mode:C++
// End:
