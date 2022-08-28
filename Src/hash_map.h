
#ifndef XT_STL_HASH_MAP_H
#define XT_STL_HASH_MAP_H
#include "stl_config.h"
#include "hashtable.h"
#include "functional.h"
#include "pair.h"
namespace my_stl
{

    template <class _Key, class _Value, class _HashFcn = hash<_Key>, class _EqualKey = equal_to<_Key>, class _Alloc = alloc>
    class hash_map
    {
        // 型别声明
        public:
            typedef hashtable<pair<const _Key, _Value>, _Key, _HashFcn, my_stl::selectfirst<pair<const _Key, _Value> >, _EqualKey, _Alloc> rep_type;
            typedef typename rep_type::key_type key_type;
            typedef typename rep_type::value_type value_type;
            typedef typename rep_type::hasher hasher;
            typedef typename rep_type::key_equal key_equal;

            typedef typename rep_type::size_type size_type;
            typedef typename rep_type::difference_type difference_type;
            typedef typename rep_type::pointer pointer;
            typedef typename rep_type::const_pointer const_pointer;
            typedef typename rep_type::reference reference;
            typedef typename rep_type::const_reference const_reference;

            typedef typename rep_type::iterator iterator;
            typedef typename rep_type::const_iterator const_iterator;

        // 数据成员 底层容器使用hashtable承载
        private:
            rep_type _M_rep;
        
        // 构造、析构和赋值
        public:
            hash_map() 
              : _M_rep(100, hasher(), key_equal()) 
            {}
            explicit hash_map(size_type n) 
              : _M_rep(n, hasher(), key_equal()) 
            {}
            hash_map(size_type n, const hasher& hf) 
              : _M_rep(n, hf, key_equal()) 
            {}
            hash_map(size_type n, const hasher& hf, const key_equal& eql)
              : _M_rep(n, hf, eql) 
            {}

            template <class InputIterator>
            hash_map(InputIterator first, InputIterator last)
              : _M_rep(100, hasher(), key_equal()) 
            { _M_rep.insert_unique(first, last); }
            template <class InputIterator>
            hash_map(InputIterator first, InputIterator last, size_type n)
              : _M_rep(n, hasher(), key_equal()) 
            { _M_rep.insert_unique(first, last); }
            template <class InputIterator>
            hash_map(InputIterator first, InputIterator last, size_type n, const hasher& hf)
              : _M_rep(n, hf, key_equal()) 
            { _M_rep.insert_unique(first, last); }
            template <class InputIterator>
            hash_map(InputIterator first, InputIterator last, size_type n, const hasher& hf, const key_equal& eql)
              : _M_rep(n, hf, eql) 
            { _M_rep.insert_unique(first, last); }

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
            iterator begin() { return _M_rep.begin(); }
            iterator end() { return _M_rep.end(); }
            const_iterator begin() const { return _M_rep.begin(); }
            const_iterator end() const { return _M_rep.end(); }

        // 相关操作
        public:
            // 插入
            pair<iterator, bool> insert(const value_type& obj)
            { return _M_rep.insert_unique(obj); }
            template <class InputIterator>
            void insert(InputIterator first, InputIterator last) 
            { _M_rep.insert_unique(first,last); }
            pair<iterator, bool> insert_noresize(const value_type& obj)
            { return _M_rep.insert_unique_noresize(obj); }    

            // 查找 若没有则插入
            _Value& operator[](const key_type& key) 
            {
                return _M_rep.find_or_insert(value_type(key, _Value())).second;
            }     

            // 查找 和 计数
            iterator find(const key_type& key) 
            { return _M_rep.find(key); }
            const_iterator find(const key_type& key) const 
            { return _M_rep.find(key); }
            size_type count(const key_type& key) const 
            { return _M_rep.count(key); }
            
            pair<iterator, iterator> equal_range(const key_type& key)
            { return _M_rep.equal_range(key); }
            pair<const_iterator, const_iterator> equal_range(const key_type& key) const
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
            void swap(hash_map& rhs) { _M_rep.swap(rhs._M_rep); }
            bool operator== (const hash_map& rhs)
            {
              return this->_M_rep = rhs._M_rep;
            }
    };

    // 重载比较运算符
    template <class _Key, class _Value, class _HashFcn, class _EqualKey, class _Alloc>
    inline bool operator==(const hash_map<_Key, _HashFcn, _EqualKey, _Alloc>& lhs,
                        const hash_map<_Key, _HashFcn, _EqualKey, _Alloc>& rhs)
    {
        return lhs == rhs;
    }
    // 重载swap 对于hash_map类型
    template <class _Key, class _Value, class _HashFcn, class _EqualKey, class _Alloc>
    inline void swap(hash_map<_Key, _HashFcn, _EqualKey, _Alloc>& lhs,
                    hash_map<_Key, _HashFcn, _EqualKey, _Alloc>& rhs)
    {
        lhs.swap(rhs);
    }


    template <class _Key, class _Value, class _HashFcn = hash<_Key>, class _EqualKey = equal_to<_Key>, class _Alloc = alloc>
    class hash_multimap
    {
        // 型别声明
        public:
            typedef hashtable<pair<const _Key, _Value>, _Key, _HashFcn, selectfirst<pair<const _Key, _Value> >, _EqualKey, _Alloc> rep_type;
            typedef typename rep_type::key_type key_type;
            typedef typename rep_type::value_type value_type;
            typedef typename rep_type::hasher hasher;
            typedef typename rep_type::key_equal key_equal;

            typedef typename rep_type::size_type size_type;
            typedef typename rep_type::difference_type difference_type;
            typedef typename rep_type::pointer pointer;
            typedef typename rep_type::const_pointer const_pointer;
            typedef typename rep_type::reference reference;
            typedef typename rep_type::const_reference const_reference;

            typedef typename rep_type::iterator iterator;
            typedef typename rep_type::const_iterator const_iterator;

        // 数据成员 底层使用hashtable承载
        private:
            rep_type _M_rep;

        // 构造、析构和赋值
        public:
            hash_multimap() 
              : _M_rep(100, hasher(), key_equal()) 
            {}
            explicit hash_multimap(size_type n) 
              : _M_rep(n, hasher(), key_equal()) 
            {}
            hash_multimap(size_type n, const hasher& hf) 
              : _M_rep(n, hf, key_equal()) 
            {}
            hash_multimap(size_type n, const hasher& hf, const key_equal& eql)
              : _M_rep(n, hf, eql) 
            {}

            template <class InputIterator>
            hash_multimap(InputIterator first, InputIterator last)
              : _M_rep(100, hasher(), key_equal()) 
            { _M_rep.insert_equal(first, last); }

            template <class InputIterator>
            hash_multimap(InputIterator first, InputIterator last, size_type n)
              : _M_rep(n, hasher(), key_equal()) 
            { _M_rep.insert_equal(first, last); }

            template <class InputIterator>
            hash_multimap(InputIterator first, InputIterator last, size_type n, const hasher& hf)
              : _M_rep(n, hf, key_equal()) 
            { _M_rep.insert_equal(first, last); }

            template <class InputIterator>
            hash_multimap(InputIterator first, InputIterator last, size_type n, const hasher& hf, const key_equal& eql)
              : _M_rep(n, hf, eql) 
            { _M_rep.insert_equal(first, last); }

        // 容量相关
        public:
            size_type size() const { return _M_rep.size(); }
            size_type max_size() const { return _M_rep.max_size(); }
            bool empty() const { return _M_rep.empty(); }
            void resize(size_type hint) { _M_rep.resize(hint); }
            size_type bucket_count() const { return _M_rep.bucket_count(); }
            size_type max_bucket_count() const { return _M_rep.max_bucket_count(); }
            size_type elems_in_bucket(size_type n) const
                { return _M_rep.elems_in_bucket(n); }

        // 迭代器
        public:
            iterator begin() { return _M_rep.begin(); }
            iterator end() { return _M_rep.end(); }
            const_iterator begin() const { return _M_rep.begin(); }
            const_iterator end() const { return _M_rep.end(); }

        // 相关操作
        public:
            // 插入
            iterator insert(const value_type& obj) { return _M_rep.insert_equal(obj); }
            template <class InputIterator>
            void insert(InputIterator first, InputIterator last) { _M_rep.insert_equal(first,last); }
            iterator insert_noresize(const value_type& obj)
                { return _M_rep.insert_equal_noresize(obj); }    

            // 查找 和 计数
            iterator find(const key_type& key) { return _M_rep.find(key); }
            const_iterator find(const key_type& key) const { return _M_rep.find(key); }

            size_type count(const key_type& key) const { return _M_rep.count(key); }
        
            pair<iterator, iterator> equal_range(const key_type& key)
                { return _M_rep.equal_range(key); }
            pair<const_iterator, const_iterator> equal_range(const key_type& key) const
                { return _M_rep.equal_range(key); }

            // 删除
            size_type erase(const key_type& key) {return _M_rep.erase(key); }
            void erase(iterator it) { _M_rep.erase(it); }
            void erase(iterator first, iterator last) { _M_rep.erase(first, last); }

            // 清除
            void clear() { _M_rep.clear(); }

        public:
            void swap(hash_multimap& rhs) { _M_rep.swap(rhs._M_rep); }
            friend bool operator== <> (const hash_multimap&, const hash_multimap&);
    };

    template <class _Key, class _Value, class HF, class EqKey, class _Alloc>
    inline bool operator==(const hash_multimap<_Key, HF, EqKey, _Alloc>& lhs,
                        const hash_multimap<_Key, HF, EqKey, _Alloc>& rhs)
    {
        return lhs._M_rep == rhs._M_rep;
    }
    template <class _Key, class _Value, class _HashFcn, class _EqualKey, class _Alloc>
    inline void swap(hash_multimap<_Key, _HashFcn, _EqualKey, _Alloc>& lhs,
                    hash_multimap<_Key, _HashFcn, _EqualKey, _Alloc>& rhs)
    {
        lhs.swap(rhs);
    }

}

#endif /* XT_STL_HASH_MAP_H */

