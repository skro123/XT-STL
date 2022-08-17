#ifndef XT_STL_MAP_H_
#define XT_STL_MAP_H_

#include "../Src/tree.h"
#include "../Src/functional.h"

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
                private:
                    Compare comp;
                    value_compare(Compare c) : comp(c) {}
                public:
                    bool operator()(const value_type& lhs, const value_type& rhs) const
                    {
                        return comp(lhs.first, rhs.first);
                    }
            };

            private:
                typedef my_stl::rb_tree<value_type, key_compare> rep_type;
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
                // map(InputIter first, InputIter last)
                //  :_M_t()
                //  { t.insert_unique(first, last);}
                // 拷贝构造
                map(const map& rhs)
                 :_M_t(rhs._M_t)
                {}
                // 移动构造
                map(map&& rhs) noexcept
                 :_M_t(my_stl::move(rhs._M_t))
                {}

                // 赋值
                map& operator=
                // 移动赋值



            

    };


}

#endif