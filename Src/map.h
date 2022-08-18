#ifndef XT_STL_MAP_H_
#define XT_STL_MAP_H_

#include "../Src/tree.h"
#include "../Src/functional.h"
#include "../Src/pair.h"

namespace my_stl
{
    template <class Key, class Value, class Compare = my_stl::less<Key>>
    class map
    {
        // 内嵌型别声明
        public:
            typedef Key     key_type;
            typedef Value   mapped_type;
            typedef my_stl::pair<const Key, Value> value_type;
            typedef Compare key_compare;

            // 用于元素比较的函数对象
            class value_compare : public binary_function <value_type, value_type, bool>
            {
                friend class map<Key, Value, Compare>;
                protected:
                    Compare comp;

                    value_compare(Compare c) : comp(c) {}
                public:
                    value_compare() = default;
                    bool operator()(const value_type& lhs, const value_type& rhs) const
                    {
                        return comp(lhs.first, rhs.first);
                    }
            };

            private:
                typedef my_stl::rb_tree<value_type, my_stl::selectfirst<value_type>, key_compare> rep_type;
                rep_type _M_t;

            public:
                typedef typename rep_type::node_type        node_type;
                typedef typename rep_type::pointer          pointer;
                typedef typename rep_type::const_pointer    const_pointer;
                typedef typename rep_type::reference        reference;
                typedef typename rep_type::const_reference  const_reference;
                typedef typename rep_type::iterator         iterator;
                typedef typename rep_type::const_iterator   const_iterator;
                typedef typename rep_type::size_type        size_type;
                typedef typename rep_type::difference_type  difference_type;
                typedef typename rep_type::data_allocator   data_allocator;

            // 构造、赋值和析构
            public:
                map() = default;
                template <class InputIter>
                map(InputIter first, InputIter last)
                 :_M_t()
                 { this->_M_t.insert_unique(first, last);}
                // 拷贝构造
                map(const map& rhs)
                 :_M_t(rhs._M_t)
                {}
                // 移动构造
                map(map&& rhs) noexcept
                 :_M_t(my_stl::move(rhs._M_t))
                {}

                // 赋值
                map& operator=(const map& rhs)
                {
                    this->_M_t = rhs._M_t;
                    return *this;
                }
                // 移动赋值
                map& operator=(map&& rhs)
                {
                    this->_M_t = my_stl::move(rhs._M_t);
                    return *this;
                }
                // 初始化列表赋值
                map& operator=(std::initializer_list<value_type> ilist)
                {
                    this->_M_t.clear();
                    this->_M_t.insert_unique(ilist.begin(), ilist.end());
                    return *this;
                }
                
                // 相关接口
                key_compare     get_key_compare()   const   { return this->_M_t._M_key_compare();}
                value_compare   get_value_compare() const   { return value_compare(this->_M_t->_M_key_compare());}


                // 迭代器相关
                iterator    begin()     noexcept
                { return this->_M_t.begin();}
                const_iterator begin()  const noexcept
                { return this->_M_t.begin();}
                iterator    end()       noexcept
                { return this->_M_t.end();}
                const_iterator end()    const noexcept
                { return this->_M_t.end();}
                const_iterator cbegin() const noexcept
                { return begin();}
                const_iterator cend()   const noexcept
                { return end();}

                // 容量相关
                bool    empty() const noexcept { return this->_M_t.empty();}
                size_type size() const noexcept { return this->_M_t.size();}
                size_type max_size() const noexcept { return this->_M_t.max_size();}

                // 访问元素
                mapped_type& at(const key_type& key)
                {
                    // iterator it = 
                }

                // 插入删除相关
                pair<iterator, bool> insert(const value_type& value)
                {
                    return this->_M_t.insert_unique(value);
                }
                // pair<iterator, bool> insert(value_type&& value)
                // {
                //     return this->_M_t.insert_unique(value);
                // }

                void erase(iterator position) { this->_M_t.erase(position);}
                size_type erase(const key_type& key) { return this->_M_t.erase(key);}
                size_type erase(const value_type& key) { return this->_M_t.erase(key);}
                void erase(iterator first, iterator last) { this->_M_t.erase(first, last);}

                void clear() { this->_M_t.clear();}

                // map相关操作
                iterator find(const key_type& key)  { return this->_M_t.find(key);}
                const_iterator find(const key_type& key) const { return this->_M_t.find(key);}

                size_type count(const key_type& key) const { return this->_M_t.count_unique(key);}

                iterator lower_bound(const key_type& key) { return this->_M_t.lower_bound(key);}
                const_iterator lower_bound(const key_type& key) const { return this->_M_t.lower_bound(key);}

                iterator upper_bound(const key_type& key) { return this->_M_t.upper_bound(key);}
                const_iterator upper_bound(const key_type& key) const { return this->_M_t.upper_bound(key);}

                // TODO equal_range

                void swap(map& rhs) noexcept
                { this->_M_t.swap(rhs._M_t);}

                
            // 重载比较运算符
            public:
                friend bool operator==(const map& lhs, const map& rhs) { return lhs._M_t == rhs._M_t;}
                friend bool operator< (const map& lhs, const map& rhs) { return lhs._M_t < rhs._M_t;}

    };

    // 重载map的比较运算符
    template <class Key, class Value, class Compare>
    bool operator==(const map<Key, Value, Compare>& lhs, const map<Key, Value, Compare>& rhs)
    {
        return lhs == rhs;
    }
    template <class Key, class Value, class Compare>
    bool operator<(const map<Key, Value, Compare>& lhs, const map<Key, Value, Compare>& rhs)
    {
        return lhs < rhs;
    }
    template <class Key, class Value, class Compare>
    bool operator!=(const map<Key, Value, Compare>& lhs, const map<Key, Value, Compare>& rhs)
    {
        return lhs != rhs;
    }
    template <class Key, class Value, class Compare>
    bool operator>(const map<Key, Value, Compare>& lhs, const map<Key, Value, Compare>& rhs)
    {
        return rhs < lhs;
    }
    template <class Key, class Value, class Compare>
    bool operator<=(const map<Key, Value, Compare>& lhs, const map<Key, Value, Compare>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class Key, class Value, class Compare>
    bool operator>=(const map<Key, Value, Compare>& lhs, const map<Key, Value, Compare>& rhs)
    {
        return !(lhs < rhs);
    }

    // 重载针对map的swap
    template <class Key, class Value, class Compare>
    void swap(map<Key, Value, Compare>& lhs, map<Key, Value, Compare>& rhs)
    {
        lhs.swap(rhs);
    }



}

#endif