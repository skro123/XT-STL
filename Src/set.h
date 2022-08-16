#ifndef XT_STL_SET_H_
#define XT_STL_SET_H_

#include "tree.h"
#include "functional.h"

namespace my_stl
{
    template <class Key, class Compare = my_stl::less<Key>>
    class set
    {
        public:
            typedef Key     key_type;
            typedef Key     value_type;
            typedef Compare key_compare;
            typedef Compare value_compare;

        private:
            // 这里使用RB Tree作为底层数据结构
            typedef my_stl::rb_tree<Key, Compare> rep_type;
            rep_type t;

        // 取出底层数据结构的内嵌型别定义
        public:
            typedef typename rep_type::node_type        node_type;
            typedef typename rep_type::const_pointer    pointer;
            typedef typename rep_type::const_pointer    const_pointer;
            typedef typename rep_type::const_reference  reference;
            typedef typename rep_type::const_reference  const_reference;
            typedef typename rep_type::const_iterator   iterator;
            typedef typename rep_type::const_iterator   const_iterator;
            typedef typename rep_type::size_type        size_type;
            typedef typename rep_type::difference_type  difference_type;

        // 构造、析构和复制函数
        public:
            // 默认构造函数
            set() = default;
            // TODO:迭代器范围构造  insert_unique 未实现
            // template <class InputIter>
            // set(InputIter first, InputIter last)
            //  : t()
            // { t.insert_unique(first, last);}
            // TODO初始化列表构造
            // set(std::initializer_list<value_type> ilist)
            //  : t()
            // { t.insert_unique(ilist.begin(), ilist.end());}
            // 拷贝构造
            set(const set& rhs)
             : t(rhs.t)
            {}
            // 移动构造
            set(set&& rhs) noexcept
             : t(my_stl::move(rhs.t))
            {}
            // 重载copy
            set& operator=(const set& rhs)
            {
                // if(this->t != rhs.t)
                // {
                //     // copy and swap
                //     set tmp_set = set(rhs.t);
                //     auto tmp_rep = this->t;
                //     this->t = tmp_set.t;
                //     tmp_set.t = tmp_rep;
                // }
                // return *this;
                
                // 这里借用底层数据结构的复制运算符可以完成
                this->t = rhs.t;
                return *this;
            }
            // 移动复制
            set& operator=(set&& rhs)
            {
                this->t = my_stl::move(rhs.t);
                return *this;
            }
            // 初始化列表复制
            set& operator=(std::initializer_list<value_type> ilist)
            {
                this->t.clear();
                this->t.insert_unique(ilist.begin(), ilist.end());
                return *this;
            }
            // 析构
            // 合成的析构函数 调用底层数据结构的析构函数释放资源
        // 相关接口
        public:


        // 迭代器相关
        public:
            // begin
            iterator begin() noexcept
            { return this->t.begin();}
            const_iterator begin() const noexcept
            { return this->t.begin();}
            // end
            iterator end() noexcept
            { return this->t.end();}
            const_iterator end() const noexcept
            { return this->t.end();}
            // cbegin
            const_iterator cbegin() const noexcept
            { return this->t.begin();}
            // cend
            const_iterator cend() const noexcept
            { return this->t.end();}

        // 容量相关
        public:
            bool empty() const noexcept
            { return this->t.empty();}
            size_t size() const noexcept
            { return this->t.size();}
            size_t max_size() const noexcept
            { return this->t.max_size();}
        // 插入删除相关
        public:
            // insert
            pair<iterator, bool> insert(const value_type& value)
            {
                return this->t.insert_unique(value);
            }
            // erase
            size_type   erase(const key_type& key) { return this->t.erase(key);}
            void        erase(iterator position) { this->t.erase(position);}
            void        erase(iterator first, iterator last) { this->t.erase(first, last);}
            // clear
            void clear() { this->clear();}
        
        // set相关
        public:
            iterator       find(const key_type& key)              { return this->t.find(key); }
            const_iterator find(const key_type& key)        const { return this->t.find(key); }
            size_type      count(const key_type& key)       const { return this->t.count_unique(key); }
            iterator       lower_bound(const key_type& key)       { return this->t.lower_bound(key); }
            const_iterator lower_bound(const key_type& key) const { return this->t.lower_bound(key); }
            iterator       upper_bound(const key_type& key)       { return this->t.upper_bound(key); }
            const_iterator upper_bound(const key_type& key) const { return this->t.upper_bound(key); }
            pair<iterator, iterator> equal_range(const key_type& key) { return this->t.equal_range(key); }
            pair<const_iterator, const_iterator> equal_range(const key_type& key) const { return this->t.equal_range(key); }

        public:
            void swap(set& rhs) noexcept
            { this->t.swap(rhs.t);}
            friend bool operator==(const set& lhs, const set& rhs) { return lhs.t == rhs.t; }
            friend bool operator< (const set& lhs, const set& rhs) { return lhs.t <  rhs.t; }   
    };
} // namespace my_stl

#endif /* XT_STL_SET_H_ */