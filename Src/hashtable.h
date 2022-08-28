#ifndef XT_STL_HASHTABLE_H
#define XT_STL_HASHTABLE_H

#include "algobase.h"
#include "alloc.h"
#include "contruct.h"
#include "algo.h"
#include "uninitialized.h"
#include "functional.h"
#include "vector.h"
#include "util.h"
#include <algo.h>
#include "pair.h"
namespace my_stl
{

    // bucket节点
    template <class _Value>
    struct hashtable_node
    {
        hashtable_node* _M_next;
        _Value _M_val;
    };  

    // 模板类的前向声明
    template <class _Value, class _Key, class _HashFcn, class _KeyOfValue, class _Compare, class _Alloc = alloc>
    class hashtable;

    template <class _Value, class _Key, class _HashFcn, class _KeyOfValue, class _Compare, class _Alloc>
    struct hashtable_iterator;

    template <class _Value, class _Key, class _HashFcn, class _KeyOfValue, class _Compare, class _Alloc>
    struct hashtable_const_iterator;

    // 定义专属迭代器
    template <class _Value, class _Key, class _HashFcn, class _KeyOfValue, class _Compare, class _Alloc>
    struct hashtable_iterator 
    {
        // 内嵌型别声明 
        // 迭代器相关
        typedef forward_iterator_tag iterator_category;
        typedef _Value value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef _Value& reference;
        typedef _Value* pointer;

        typedef hashtable_iterator<_Value, _Key, _HashFcn, _KeyOfValue, _Compare, _Alloc> iterator;
        typedef hashtable_const_iterator<_Value, _Key, _HashFcn, _KeyOfValue, _Compare, _Alloc> const_iterator;

        typedef hashtable<_Value, _Key, _HashFcn, _KeyOfValue, _Compare, _Alloc> hashtable_type;
        typedef hashtable_type* hashtable_pointer;
        typedef hashtable_node<_Value> node_type;
        typedef node_type* node_pointer;

        // 数据成员 
        node_pointer _M_cur; // 迭代器所指节点
        hashtable_pointer _M_hashtable; // 节点所属的容器

        // 构造函数
        hashtable_iterator() = default;
        hashtable_iterator(node_pointer node, hashtable_pointer table) : _M_cur(node), _M_hashtable(table) {}

        // 重载运算符 表现迭代器的特征
        reference operator*() const { return this->_M_cur->_M_val; }
        pointer operator->() const { return &(operator*()); }
        iterator& operator++()
        {
            const node_type* old = this->_M_cur;
            this->_M_cur = this->_M_cur->_M_next;
            if (!this->_M_cur) {
                size_type bucket = this->_M_hashtable->bkt_num(old->_M_val);
                while (!this->_M_cur && ++bucket < this->_M_hashtable->_M_buckets.size())
                this->_M_cur = this->_M_hashtable->_M_buckets[bucket];
            }
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp = *this;
            ++*this;
            return tmp;
        }
        bool operator==(const iterator& rhs) const { return this->_M_cur == rhs._M_cur; }
        bool operator!=(const iterator& rhs) const { return this->_M_cur != rhs._M_cur; }
    };
    // const 迭代器类型
    template <class _Value, class _Key, class _HashFcn, class _KeyOfValue, class _Compare, class _Alloc>
    struct hashtable_const_iterator 
    {
        // 内嵌型别声明 
        // 迭代器相关
        typedef forward_iterator_tag iterator_category;
        typedef _Value value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef const _Value& reference;
        typedef const _Value* pointer;

        typedef hashtable_iterator<_Value, _Key, _HashFcn, _KeyOfValue, _Compare, _Alloc> iterator;
        typedef hashtable_const_iterator<_Value, _Key, _HashFcn, _KeyOfValue, _Compare, _Alloc> const_iterator;

        typedef hashtable<_Value, _Key, _HashFcn, _KeyOfValue, _Compare, _Alloc> hashtable_type;
        typedef hashtable_type* hashtable_pointer;
        typedef hashtable_node<_Value> node_type;
        typedef node_type* node_pointer;


        // 数据成员 
        // 错误一：修正这里应该用 const修饰
        // 错误二：如果使用const node_pointer 修饰 这个变量会变成node_pointer这种类型的常量不可修改 operator++中报错
        //        将*号明确的表示出来才是指向常量的指针
        // 错误三：错误二修改后应该同时修改构造函数的的形参 否则和数据成员类型不一致
        //         也就是 const node_pointer 与 const node_type* 是两种类型 为什么呢？
        const node_type* _M_cur; // 迭代器所指节点
        const hashtable_type* _M_hashtable; // 节点所属的容器

        // 构造函数
        hashtable_const_iterator() = default;
        hashtable_const_iterator(const node_type* node, const hashtable_type* table) : _M_cur(node), _M_hashtable(table) {}
        hashtable_const_iterator(const iterator& rhs) : _M_cur(rhs._M_cur), _M_hashtable(rhs._M_hashtable) {}

        // 重载运算符 表现迭代器的特征
        reference operator*() const { return this->_M_cur->_M_val; }
        pointer operator->() const { return &(operator*()); }
        const_iterator& operator++()
        {
            const node_type* old = this->_M_cur;
            this->_M_cur = this->_M_cur->_M_next;
            if (!this->_M_cur) {
                size_type bucket = this->_M_hashtable->bkt_num(old->_M_val);
                while (!this->_M_cur && ++bucket < this->_M_hashtable->_M_buckets.size())
                this->_M_cur = this->_M_hashtable->_M_buckets[bucket];
            }
            return *this;
        }
        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            ++*this;
            return tmp;
        }
        bool operator==(const const_iterator& rhs) const { return this->_M_cur == rhs._M_cur; }
        bool operator!=(const const_iterator& rhs) const { return this->_M_cur != rhs._M_cur; }
    };


    // bucket 使用的大小 参考 https://github.com/Alinshans/MyTinySTL/blob/master/MyTinySTL/hashtable.h#L415
    #if (_MSC_VER && _WIN64) || ((__GNUC__ || __clang__) &&__SIZEOF_POINTER__ == 8)
    #define SYSTEM_64 1
    #else
    #define SYSTEM_32 1
    #endif

    #ifdef SYSTEM_64

    #define PRIME_NUM 99

    // 1. start with p = 101
    // 2. p = next_prime(p * 1.7)
    // 3. if p < (2 << 63), go to step 2, otherwise, go to step 4
    // 4. end with p = prev_prime(2 << 63 - 1)
    static constexpr size_t ht_prime_list[] = {
    101ull, 173ull, 263ull, 397ull, 599ull, 907ull, 1361ull, 2053ull, 3083ull,
    4637ull, 6959ull, 10453ull, 15683ull, 23531ull, 35311ull, 52967ull, 79451ull,
    119179ull, 178781ull, 268189ull, 402299ull, 603457ull, 905189ull, 1357787ull,
    2036687ull, 3055043ull, 4582577ull, 6873871ull, 10310819ull, 15466229ull,
    23199347ull, 34799021ull, 52198537ull, 78297827ull, 117446801ull, 176170229ull,
    264255353ull, 396383041ull, 594574583ull, 891861923ull, 1337792887ull,
    2006689337ull, 3010034021ull, 4515051137ull, 6772576709ull, 10158865069ull,
    15238297621ull, 22857446471ull, 34286169707ull, 51429254599ull, 77143881917ull,
    115715822899ull, 173573734363ull, 260360601547ull, 390540902329ull, 
    585811353559ull, 878717030339ull, 1318075545511ull, 1977113318311ull, 
    2965669977497ull, 4448504966249ull, 6672757449409ull, 10009136174239ull,
    15013704261371ull, 22520556392057ull, 33780834588157ull, 50671251882247ull,
    76006877823377ull, 114010316735089ull, 171015475102649ull, 256523212653977ull,
    384784818980971ull, 577177228471507ull, 865765842707309ull, 1298648764060979ull,
    1947973146091477ull, 2921959719137273ull, 4382939578705967ull, 6574409368058969ull,
    9861614052088471ull, 14792421078132871ull, 22188631617199337ull, 33282947425799017ull,
    49924421138698549ull, 74886631708047827ull, 112329947562071807ull, 168494921343107851ull,
    252742382014661767ull, 379113573021992729ull, 568670359532989111ull, 853005539299483657ull,
    1279508308949225477ull, 1919262463423838231ull, 2878893695135757317ull, 4318340542703636011ull,
    6477510814055453699ull, 9716266221083181299ull, 14574399331624771603ull, 18446744073709551557ull
    };

    #else

    #define PRIME_NUM 44

    // 1. start with p = 101
    // 2. p = next_prime(p * 1.7)
    // 3. if p < (2 << 31), go to step 2, otherwise, go to step 4
    // 4. end with p = prev_prime(2 << 31 - 1)
    static constexpr size_t ht_prime_list[] = {
    101u, 173u, 263u, 397u, 599u, 907u, 1361u, 2053u, 3083u, 4637u, 6959u, 
    10453u, 15683u, 23531u, 35311u, 52967u, 79451u, 119179u, 178781u, 268189u,
    402299u, 603457u, 905189u, 1357787u, 2036687u, 3055043u, 4582577u, 6873871u,
    10310819u, 15466229u, 23199347u, 34799021u, 52198537u, 78297827u, 117446801u,
    176170229u, 264255353u, 396383041u, 594574583u, 891861923u, 1337792887u,
    2006689337u, 3010034021u, 4294967291u,
    };

    #endif
    // 找出最接近并大于等于 n 的那个质数
    inline size_t ht_next_prime(size_t n)
    {
    const size_t* first = ht_prime_list;
    const size_t* last = ht_prime_list + PRIME_NUM;
    const size_t* pos = std::lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
    }

    // hashtable
    // 第一模板参数值类型，第二模板参数为键类型，第三模板参数为hash function的类型，第四参数为从元素中取出键值的函数
    // 第五参数为判断键值是否相等，第六参数为空间配置器类型 默认使用带内存池的分配器 my_stl::alloc
    template <class _Value, class _Key, class _HashFcn,class _KeyOfValue, class _Compare, class _Alloc>
    class hashtable 
    {
        public:
            typedef _Key key_type;
            typedef _Value value_type;
            typedef _HashFcn hasher;
            typedef _Compare key_equal;

            typedef size_t            size_type;
            typedef ptrdiff_t         difference_type;
            typedef value_type*       pointer;
            typedef const value_type* const_pointer;
            typedef value_type&       reference;
            typedef const value_type& const_reference;

            typedef hashtable_iterator<_Value, _Key, _HashFcn, _KeyOfValue, _Compare, _Alloc> iterator;
            typedef hashtable_const_iterator<_Value, _Key, _HashFcn, _KeyOfValue, _Compare, _Alloc> const_iterator;


            typedef hashtable_node<_Value> node;
            typedef simple_alloc<node, _Alloc> node_allocator;

        public:
            friend struct hashtable_iterator<_Value, _Key, _HashFcn, _KeyOfValue, _Compare, _Alloc>;
            friend struct hashtable_const_iterator<_Value, _Key, _HashFcn, _KeyOfValue, _Compare, _Alloc>;

                
            hasher hash_funct() const { return _M_hash; }
            key_equal key_eq() const { return _M_equals; }

        // 数据成员
        private:
            hasher _M_hash;
            key_equal _M_equals;
            _KeyOfValue _M_get_key;
            vector<node*,_Alloc> _M_buckets;
            size_type _M_num_elements;



        // 构造、析构和赋值
        public:
            hashtable(size_type n, const _HashFcn& hf = _HashFcn(), const _Compare& eql = _Compare(), const _KeyOfValue& ext = _KeyOfValue())
            :_M_hash(hf), _M_equals(eql), _M_get_key(ext), _M_num_elements(0)
            { this->initialize_buckets(n); }

            // hashtable(size_type n, const _HashFcn& hf, const _Compare& eql)
            // :_M_hash(hf), _M_equals(eql), _M_get_key(_KeyOfValue()), _M_num_elements(0)
            // { this->initialize_buckets(n); }

            hashtable(const hashtable& rhs)
            :_M_hash(rhs._M_hash), _M_equals(rhs._M_equals), _M_get_key(rhs._M_get_key), _M_num_elements(0)
            { this->copy_from(rhs);}

            hashtable& operator= (const hashtable& rhs)
            {
                if (&rhs != this) 
                {
                    this->clear();
                    _M_hash = rhs._M_hash;
                    _M_equals = rhs._M_equals;
                    _M_get_key = rhs._M_get_key;
                    this->copy_from(rhs);
                }
                return *this;
            }
            
            ~hashtable() { this->clear(); }

        // 容量相关
        public:
            size_type size() const { return _M_num_elements; }
            size_type max_size() const { return size_type(-1); }
            bool empty() const { return size() == 0; }
            size_type bucket_count() const { return _M_buckets.size(); }
            size_type max_bucket_count() const { return ht_prime_list[PRIME_NUM - 1]; } 
            size_type elems_in_bucket(size_type bucket) const
            {
                size_type result = 0;
                for (node* cur = _M_buckets[bucket]; cur; cur = cur->_M_next)
                {
                    result += 1;
                }
                return result;
            }
            void resize(size_type num_elements_hint);

        // 迭代器相关
        public:
            iterator begin()
            {
                // 遍历_M_buckets找到第一个不为空的位置即为begin
                for (size_type n = 0; n < _M_buckets.size(); ++n)
                {
                    if(_M_buckets[n])
                    {
                        return iterator(_M_buckets[n], this);
                    }
                }
                return end();
            }
            iterator end() { return iterator(nullptr, this); }
            const_iterator begin() const
            {
                for (size_type n = 0; n < _M_buckets.size(); ++n)
                {
                    if (_M_buckets[n])
                    {
                        return const_iterator(_M_buckets[n], this);
                    }
                }
                return end();
            }
            const_iterator end() const { return const_iterator(nullptr, this); }

        public:
            // 插入 不可重复
            pair<iterator, bool> insert_unique(const value_type& obj)
            {
                // 判断是否需要重整bucket vector
                resize(_M_num_elements + 1);
                return insert_unique_noresize(obj);
            }
            // 插入 可重复
            iterator insert_equal(const value_type& obj)
            {
                resize(_M_num_elements + 1);
                return insert_equal_noresize(obj);
            }
            // 具体执行插入操作
            pair<iterator, bool> insert_unique_noresize(const value_type& obj);
            iterator insert_equal_noresize(const value_type& obj);
            // 范围插入 键值不可重复
            template <class InputIterator>
            void insert_unique(InputIterator first, InputIterator last)
            {
                insert_unique(first, last, iterator_category(first));
            }
            // 范围插入 键值可重复
            template <class InputIterator>
            void insert_equal(InputIterator first, InputIterator last)
            {
                insert_equal(first, last, iterator_category(first));
            }
            // 执行范围插入 input_iterator_tag版本 键值不可重复
            // input_iterator_tag类型迭代器无法计算出距离 需要一个一个插入
            // 每次调用insert_unique 需要每次都进行resize检查
            template <class InputIterator>
            void insert_unique(InputIterator first, InputIterator last,
                                input_iterator_tag)
            {
                for ( ; first != last; ++first)
                {
                    insert_unique(*first);
                }
            }
            // 执行范围插入 input_iterator_tag版本 键值可重复
            template <class InputIterator>
            void insert_equal(InputIterator first, InputIterator last,
                                input_iterator_tag)
            {
                for ( ; first != last; ++first)
                {
                    insert_equal(*first);
                }
            }
            // 执行范围插入 forward_iterator_tag 键值不可重复
            // forward_iterator_tag类型迭代器可以计算出距离 不需要每次都进行resize检查
            template <class ForwardIterator>
            void insert_unique(ForwardIterator first, ForwardIterator last,
                                forward_iterator_tag)
            {
                size_type n = 0;
                my_stl::distance(first, last, n);
                resize(_M_num_elements + n);
                for ( ; n > 0; --n, ++first)
                {
                    insert_unique_noresize(*first);
                }
            }
            // 执行范围插入 forward_iterator_tag 键值可重复
            template <class ForwardIterator>
            void insert_equal(ForwardIterator first, ForwardIterator last,
                                forward_iterator_tag)
            {
                size_type n = 0;
                my_stl::distance(first, last, n);
                resize(_M_num_elements + n);
                for ( ; n > 0; --n, ++first)
                {
                    insert_equal_noresize(*first);
                }
            }


            // 查找 或 插入
            // 如果没有找到就插入 
            reference find_or_insert(const value_type& obj);

            // 查找
            iterator find(const key_type& key) 
            {
                // 找到落在哪一个bucket 即计算哈希值
                size_type n = bkt_num_key(key);
                node* first;
                // 从bucket对应链表逐一对比 成功就跳出
                for ( first = _M_buckets[n]; 
                        first && !_M_equals(_M_get_key(first->_M_val), key); 
                        first = first->_M_next)
                {}
                // 查找成功返回对应位置 否则是nullptr
                return iterator(first, this);
            } 
            // const对象重载
            const_iterator find(const key_type& key) const
            {
                size_type n = bkt_num_key(key);
                const node* first;
                for ( first = _M_buckets[n];
                        first && !_M_equals(_M_get_key(first->_M_val), key);
                        first = first->_M_next)
                {}
                return const_iterator(first, this);
            } 

            pair<iterator, iterator> equal_range(const key_type& key);
            pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

            // 查找计数
            size_type count(const key_type& key) const
            {
                const size_type n = bkt_num_key(key);
                size_type result = 0;

                for (const node* cur = _M_buckets[n]; cur; cur = cur->_M_next)
                {
                    if (_M_equals(_M_get_key(cur->_M_val), key))
                    {
                        ++result;
                    }
                }
                return result;
            }
            // 删除 通过键
            size_type erase(const key_type& key);
            // 通过迭代器删除 单个位置删除
            void erase(const iterator& it);
            // 通过迭代器删除 范围删除
            void erase(iterator first, iterator last);
            // const 重载
            void erase(const const_iterator& it);
            void erase(const_iterator first, const_iterator last);

            // 清除
            void clear();

        private:
            size_type next_size(size_type n) const { return ht_next_prime(n); }

            void initialize_buckets(size_type n)
            {
                const size_type n_buckets = next_size(n);
                _M_buckets.reserve(n_buckets);
                _M_buckets.insert(_M_buckets.end(), n_buckets, (node*) 0);
                _M_num_elements = 0;
            }

            size_type bkt_num_key(const key_type& key) const
            {
                return bkt_num_key(key, _M_buckets.size());
            }

            size_type bkt_num(const value_type& obj) const
            {
                return bkt_num_key(_M_get_key(obj));
            }

            size_type bkt_num_key(const key_type& key, size_t n) const
            {
                return _M_hash(key) % n;
            }

            size_type bkt_num(const value_type& obj, size_t n) const
            {
                return bkt_num_key(_M_get_key(obj), n);
            }

            node* new_node(const value_type& obj)
            {
                node* n = node_allocator::allocate();
                n->_M_next = 0;
                try 
                {
                    construct(&n->_M_val, obj);
                    return n;
                }
                catch(...)
                {
                    node_allocator::deallocate(n);
                }
            }
            
            void delete_node(node* n)
            {
                destroy(&n->_M_val);
                node_allocator::deallocate(n);
            }

            void erase_bucket(const size_type n, node* first, node* last);
            void erase_bucket(const size_type n, node* last);

            void copy_from(const hashtable& ht);

            void swap(hashtable& ht)
            {
                my_stl::swap(_M_hash, ht._M_hash);
                my_stl::swap(_M_equals, ht._M_equals);
                my_stl::swap(_M_get_key, ht._M_get_key);
                _M_buckets.swap(ht._M_buckets);
                my_stl::swap(_M_num_elements, ht._M_num_elements);
            }
            bool operator== (const hashtable& rhs)
            {
                if (this->_M_buckets.size() != rhs._M_buckets.size())
                { 
                    return false;
                }
                for (int n = 0; n < this->_M_buckets.size(); ++n) 
                {
                    node* cur1 = this->_M_buckets[n];
                    node* cur2 = rhs._M_buckets[n];
                    for ( ; cur1 && cur2 && cur1->val == cur2->val; cur1 = cur1->_M_next, cur2 = cur2->_M_next)
                    {}
                    if (cur1 || cur2)
                    { 
                        return false;
                    }
                }
                return true;
            }

    };

    // 重载比较运算符
    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    bool operator==(const hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>& lhs,
                    const hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>& rhs)
    {
        return lhs = rhs;
    }  
    // 重载swap 针对hashtable类型
    template <class Val, class _Key, class _HashFcn, class Extract, class EqKey, class _Alloc>
    inline void swap(hashtable<Val, _Key, _HashFcn, Extract, EqKey, _Alloc>& lhs,
                    hashtable<Val, _Key, _HashFcn, Extract, EqKey, _Alloc>& rhs) 
    {
        lhs.swap(rhs);
    }


    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    pair<typename hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::iterator, bool> 
    hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::insert_unique_noresize(const value_type& obj)
    {
        const size_type n = bkt_num(obj);
        node* first = _M_buckets[n];
        // 遍历bucket对应的链表
        for (node* cur = first; cur; cur = cur->_M_next) 
        {
            // 发现该键值已经插入 则直接返回
            if (_M_equals(_M_get_key(cur->_M_val), _M_get_key(obj)))
            {
                return pair<iterator, bool>(iterator(cur, this), false);
            }
        }
        // 链表为空或者链表中不存在要插入的键 就执行插入操作
        node* tmp = new_node(obj);
        tmp->_M_next = first;
        _M_buckets[n] = tmp;
        ++_M_num_elements;
        return pair<iterator, bool>(iterator(tmp, this), true);
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    typename hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::iterator 
    hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::insert_equal_noresize(const value_type& obj)
    {
        const size_type n = bkt_num(obj);
        node* first = _M_buckets[n];
        // 遍历bucket对应的链表
        for (node* cur = first; cur; cur = cur->_M_next) 
        {
            // 如果找到相同的键值 则直接插入
            if (_M_equals(_M_get_key(cur->_M_val), _M_get_key(obj))) 
            {
                node* tmp = new_node(obj);
                tmp->_M_next = cur->_M_next;
                cur->_M_next = tmp;
                ++_M_num_elements;
                return iterator(tmp, this);
            }
        }
        // 链表为空或者链表中不存在要插入的键 就执行插入操作
        node* tmp = new_node(obj);
        tmp->_M_next = first;
        _M_buckets[n] = tmp;
        ++_M_num_elements;
        return iterator(tmp, this);
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    typename hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::reference 
    hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::find_or_insert(const value_type& obj)
    {
        resize(_M_num_elements + 1);

        size_type n = bkt_num(obj);
        node* first = _M_buckets[n];

        for (node* cur = first; cur; cur = cur->_M_next)
        {
            if (_M_equals(_M_get_key(cur->_M_val), _M_get_key(obj)))
            {
                return cur->_M_val;
            }
        }

        node* tmp = new_node(obj);
        tmp->_M_next = first;
        _M_buckets[n] = tmp;
        ++_M_num_elements;
        return tmp->_M_val;
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    pair<typename hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::iterator,
        typename hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::iterator> 
    hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::equal_range(const key_type& key)
    {
        typedef pair<iterator, iterator> pii;
        const size_type n = bkt_num_key(key);

        for (node* first = _M_buckets[n]; first; first = first->_M_next) 
        {
            if (_M_equals(_M_get_key(first->_M_val), key)) 
            {
                for (node* cur = first->_M_next; cur; cur = cur->_M_next)
                {
                    if (!_M_equals(_M_get_key(cur->_M_val), key))
                    {
                        return pii(iterator(first, this), iterator(cur, this));
                    }
                }
                for (size_type m = n + 1; m < _M_buckets.size(); ++m)
                {
                    if (_M_buckets[m])
                    {
                        return pii(iterator(first, this), iterator(_M_buckets[m], this));
                    }
                }
                return pii(iterator(first, this), end());
            }
        }
        return pii(end(), end());
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    pair<typename hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::const_iterator, 
        typename hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::const_iterator> 
    hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::equal_range(const key_type& key) const
    {
        typedef pair<const_iterator, const_iterator> pii;
        const size_type n = bkt_num_key(key);

        for (const node* first = _M_buckets[n] ; first; first = first->_M_next) 
        {
            if (_M_equals(_M_get_key(first->_M_val), key)) 
            {
                for (const node* cur = first->_M_next; cur; cur = cur->_M_next)
                {
                    if (!_M_equals(_M_get_key(cur->_M_val), key))
                    {
                        return pii(const_iterator(first, this), const_iterator(cur, this));
                    }
                }
                for (size_type m = n + 1; m < _M_buckets.size(); ++m)
                {
                    if (_M_buckets[m])
                    {
                        return pii(const_iterator(first, this), const_iterator(_M_buckets[m], this));
                    }
                }
                return pii(const_iterator(first, this), end());
            }
        }
        return pii(end(), end());
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    typename hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::size_type 
    hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::erase(const key_type& key)
    {
        size_type erased = 0;
        const size_type n = bkt_num_key(key);
        node* first = _M_buckets[n];
        if (first) 
        {
            node* cur = first;
            node* next = cur->_M_next;
            while (next) 
            {
                if (_M_equals(_M_get_key(next->_M_val), key)) 
                {
                    cur->_M_next = next->_M_next;
                    delete_node(next);
                    next = cur->_M_next;
                    ++erased;
                    --_M_num_elements;
                }
                else 
                {
                    cur = next;
                    next = cur->_M_next;
                }
            }
            if (_M_equals(_M_get_key(first->_M_val), key)) 
            {
                _M_buckets[n] = first->_M_next;
                delete_node(first);
                ++erased;
                --_M_num_elements;
            }
        }
        return erased;
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    void hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::erase(const iterator& it)
    {
        if (node* const p = it._M_cur) 
        {
            const size_type n = bkt_num(p->_M_val);
            node* cur = _M_buckets[n];

            if (cur == p) 
            {
                _M_buckets[n] = cur->_M_next;
                delete_node(cur);
                --_M_num_elements;
            }
            else 
            {
                node* next = cur->_M_next;
                while (next) 
                {
                    if (next == p) 
                    {
                        cur->_M_next = next->_M_next;
                        delete_node(next);
                        --_M_num_elements;
                        break;
                    }
                    else 
                    {
                        cur = next;
                        next = cur->_M_next;
                    }
                }
            }
        }
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    void hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::erase(iterator first, iterator last)
    {
        size_type f_bucket = first._M_cur ? bkt_num(first._M_cur->_M_val) : _M_buckets.size();
        size_type l_bucket = last._M_cur ? bkt_num(last._M_cur->_M_val) : _M_buckets.size();

        if (first._M_cur == last._M_cur)
        {
            return;
        }
        else if (f_bucket == l_bucket)
        {
            erase_bucket(f_bucket, first._M_cur, last._M_cur);
        }
        else 
        {
            erase_bucket(f_bucket, first._M_cur, 0);
            for (size_type n = f_bucket + 1; n < l_bucket; ++n)
            {
                erase_bucket(n, 0);
            }
            if (l_bucket != _M_buckets.size())
            {
                erase_bucket(l_bucket, last._M_cur);
            }
        }
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    inline void
    hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::erase(const_iterator first,
                                        const_iterator last)
    {
        erase(iterator(const_cast<node*>(first.cur), const_cast<hashtable*>(first.ht)),
                iterator(const_cast<node*>(last.cur), const_cast<hashtable*>(last.ht)));
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    inline void
    hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::erase(const const_iterator& it)
    {
        erase(iterator(const_cast<node*>(it.cur),const_cast<hashtable*>(it.ht)));
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    void hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::resize(size_type num_elements_hint)
    {
        const size_type old_n = _M_buckets.size();
        if (num_elements_hint > old_n) 
        {
            // 这里找到下一个质数
            const size_type n = next_size(num_elements_hint);
            if (n > old_n) 
            {
                vector<node*, _Alloc> tmp(n, (node*) 0); // 设立一个新的buckets
                try 
                {
                    // 依次处理每一个bucket
                    for (size_type bucket = 0; bucket < old_n; ++bucket) 
                    {
                        node* first = _M_buckets[bucket];
                        while (first) 
                        {
                            size_type new_bucket = bkt_num(first->_M_val, n);
                            // 从bucket对应的链表中取出第一个节点
                            _M_buckets[bucket] = first->_M_next;
                            // 将该节点插入新的bucket中
                            first->_M_next = tmp[new_bucket];
                            tmp[new_bucket] = first;
                            // 回到旧bucket 准备再次取出第一个节点
                            first = _M_buckets[bucket];          
                        }
                    }
                    _M_buckets.swap(tmp); // 直接交换两个vector
                }
                catch(...) 
                {
                    // 异常处理 栈中的tmp vector对象安全释放资源
                    for (size_type bucket = 0; bucket < tmp.size(); ++bucket) 
                    {
                        while (tmp[bucket]) 
                        {
                            node* next = tmp[bucket]->_M_next;
                            delete_node(tmp[bucket]);
                            tmp[bucket] = next;
                        }
                    }
                    throw;
                }
            }
        }
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    void hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::erase_bucket(const size_type n, 
                                                    node* first, node* last)
    {
        node* cur = _M_buckets[n];
        if (cur == first)
        {
            erase_bucket(n, last);
        }
        else 
        {
            node* next;
            for (next = cur->_M_next; next != first; cur = next, next = cur->_M_next)
            {}
            while (next) 
            {
                cur->_M_next = next->_M_next;
                delete_node(next);
                next = cur->_M_next;
                --_M_num_elements;
            }
        }
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    void 
    hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::erase_bucket(const size_type n, node* last)
    {
        node* cur = _M_buckets[n];
        while (cur != last) 
        {
            node* next = cur->_M_next;
            delete_node(cur);
            cur = next;
            _M_buckets[n] = cur;
            --_M_num_elements;
        }
    }

    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    void hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::clear()
    {
        for (size_type i = 0; i < _M_buckets.size(); ++i) 
        {
            node* cur = _M_buckets[i];
            while (cur != 0) 
            {
                node* next = cur->_M_next;
                delete_node(cur);
                cur = next;
            }
            _M_buckets[i] = 0;
        }
        _M_num_elements = 0;
    }

        
    template <class _Value, class _Key, class _HashFcn, class Ex, class Eq, class _Alloc>
    void hashtable<_Value, _Key, _HashFcn, Ex, Eq, _Alloc>::copy_from(const hashtable& ht)
    {
        _M_buckets.clear();
        _M_buckets.reserve(ht._M_buckets.size());
        _M_buckets.insert(_M_buckets.end(), ht._M_buckets.size(), (node*) 0);
        try {
            for (size_type i = 0; i < ht._M_buckets.size(); ++i) 
            {
                if (const node* cur = ht._M_buckets[i]) 
                {
                    node* copy = new_node(cur->_M_val);
                    _M_buckets[i] = copy;

                    for (node* next = cur->_M_next; next; cur = next, next = cur->_M_next) 
                    {
                        copy->_M_next = new_node(next->_M_val);
                        copy = copy->_M_next;
                    }
                }
            }
            _M_num_elements = ht._M_num_elements;
        }
        catch(...)
        {
            clear();
        }
    }
} // namespace my_stl

#endif /* XT_STL_HASHTABLE_H */