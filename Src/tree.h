
#ifndef XT_STL_TREE_H
#define XT_STL_TREE_H


#include "stl_iterator.h"
#include "alloc.h"
#include "stl_contruct.h"
#include "pair.h"
#include "util.h"
namespace my_stl
{
    // 红黑节点颜色标志
    // 这里使用枚举，SGI 5使用枚举， SGI2.9和MyTintSTL使用两个bool常变量
    enum rb_tree_color { red = false, black = true };

    // 红黑树基础节点
    struct rb_tree_node_base
    {
        // 内嵌型别
        typedef rb_tree_node_base* base_ptr;
        typedef const rb_tree_node_base* const_base_ptr;

        // 数据成员
        base_ptr parent;
        base_ptr left;
        base_ptr right;
        rb_tree_color color;

        // 成员函数
        base_ptr get_base_ptr()
        {
            return &(*this);
        }
    };

    template <class _Value>
    struct rb_tree_node : public rb_tree_node_base
    {
        typedef _Value                   value_type;
        typedef rb_tree_node<_Value>*    node_ptr;

        // 数据成员 节点的键值 SGI5中考虑内存对齐
        value_type value;

        // 成员函数
        node_ptr get_node_ptr()
        {
            return &(*this);
        }
    };

    // 最左子节点即为以x为根节点的最小节点
    // 使用模板可以同时应用于 base_ptr 和 const_base_ptr
    // 但同时也有风险 当实例化与一个没有left和right的节点类型
    template <class NodePtr>
    NodePtr rb_tree_minimum(NodePtr x) noexcept
    {
        while(x->left != nullptr)
        {
            x = x->left;
        }
        return x;
    }

    template <class NodePtr>
    NodePtr rb_tree_maximum(NodePtr x) noexcept
    {
        while(x->right != nullptr)
        {
            x = x->right;
        }
        return x;
    }

    struct rb_tree_base_iterator
    {
        typedef rb_tree_node_base::base_ptr base_ptr;
        typedef bidirectional_iterator_tag iterator_category;

        // 数据成员
        base_ptr node;

        // 成员函数
        rb_tree_base_iterator(): node(nullptr) {}

        // 迭代器前进 依据二叉树节点的排列规则 比当前节点值大的最小节点
        // 该实现依赖于红黑树的header指向root，并且本身表示end()
        void increment ()
        {
            // 情况一 当前有右节点
            if(node->right != nullptr)
            {
                node = rb_tree_minimum(node->right);
            }
            else
            {
                // 向上逐步查找 当前节点是否属于某个节点的左子树
                // 若是 则这个节点即为答案 若不是则直到header
                base_ptr y = node->parent;
                while(y->right == node)
                {
                    node = y;
                    y = y->parent;
                }
                if(node->right != y)
                {
                    node = y;
                }
            }
        }
        void decrement()
        {
            // header 节点
            if(node->color == rb_tree_color::red && node->parent->parent == node)
            {
                node = node->right;
            }
            else if(node->left != nullptr)
            {
                node = rb_tree_maximum(node->left);
            }
            else 
            {
                base_ptr y = node->parent;
                while(node == y->left)
                {
                    node = y;
                    y = y->parent;
                }
                node = y;
            }
        }

        bool operator==(const rb_tree_base_iterator& rhs)       { return this->node == rhs.node;}
        bool operator==(const rb_tree_base_iterator& rhs) const { return this->node == rhs.node;} 
        bool operator!=(const rb_tree_base_iterator& rhs)       { return this->node != rhs.node;}
        bool operator!=(const rb_tree_base_iterator& rhs) const { return this->node != rhs.node;}
    };

    template <class T>
    struct rb_tree_iterator : public rb_tree_base_iterator
    {
        
        typedef T                   value_type;
        typedef T*                  pointer;
        typedef T&                  reference;
        typedef ptrdiff_t           difference_type;
    

        typedef rb_tree_node_base*   base_ptr;
        typedef rb_tree_node<T>*    node_ptr;
        typedef rb_tree_iterator<T> iterator;
        typedef rb_tree_iterator<T> self;
        
        using rb_tree_base_iterator::node;

        rb_tree_iterator() {}
        rb_tree_iterator(base_ptr x) { node = x;}
        rb_tree_iterator(node_ptr x) { node = x;}
        rb_tree_iterator(const iterator& rhs) { node = rhs.node;}

        reference operator*() const { return node_ptr(node)->value;}
        pointer operator->() const { return &(operator*());}

        self& operator++() { increment(); return *this;}
        self operator++(int) { self tmp = *this; increment(); return tmp;}
        self& operator--() { decrement(); return *this;}
        self operator--(int) { self tmp = *this; decrement(); return tmp;}
    };

    template <class T>
    struct rb_tree_const_iterator : public rb_tree_base_iterator
    {
        
        typedef T                   value_type;
        typedef const T*            pointer;
        typedef const T&            reference;
        typedef ptrdiff_t           difference_type;
    

        typedef rb_tree_node_base*   base_ptr;
        typedef rb_tree_node<T>*    node_ptr;
        typedef rb_tree_iterator<T>         iterator;
        typedef rb_tree_const_iterator<T> const_iterator;
        typedef rb_tree_const_iterator<T> self;
        
        using rb_tree_base_iterator::node;

        rb_tree_const_iterator() {}
        rb_tree_const_iterator(base_ptr x) { node = x;}
        rb_tree_const_iterator(node_ptr x) { node = x;}
        rb_tree_const_iterator(const iterator& rhs) { node = rhs.node;}
        rb_tree_const_iterator(const const_iterator& rhs) { node = rhs.node;}

        reference operator*() const { return node_ptr(node)->value;}
        pointer operator->() const { return &(operator*());}

        self& operator++() { increment(); return *this;}
        self operator++(int) { self tmp = *this; increment(); return tmp;}
        self& operator--() { decrement(); return *this;}
        self operator--(int) { self tmp = *this; decrement(); return tmp;}
    };

    // rotate left
    // x为旋转点 root为根节点 颜色转变在调用处处理
    inline void
    __rb_tree_rotate_left(rb_tree_node_base* x, rb_tree_node_base*& root)
    {
        rb_tree_node_base* y = x->right;
        // 第一处 处理 值大小在x和y中间的节点 y->left
        x->right = y->left;
        if(y->left != nullptr)
        {
            y->left->parent = x; // 节点之间双向连接
        }
        // 第二处 处理 x和y组成的子树与上层节点的关系
        y->parent = x->parent;
        if(root == x)
        { root = y; }
        else if(x == x->parent->left)
        { x->parent->left = y; }
        else
        { x->parent->right = y; }
        // 最后 处理 x和y之间的连接
        y->left = x;
        x->parent = y;
    }
    // rotate right
    inline void
    __rb_tree_rotate_right(rb_tree_node_base* x, rb_tree_node_base*& root)
    {
        rb_tree_node_base* y = x->left;
        // 第一处 处理 值大小在x和y中间的节点 y->right
        x->left = y->right;
        if(y->right != nullptr)
        { y->right->parent = x;}
        // 第二处 处理 x和y组成的子树和上层节点的关系
        y->parent = x->parent;
        if(root == x)
        { root = y;}
        else if(x == x->parent->left)
        { x->parent->left = y;}
        else
        { x->parent->right = y;}
        // 第三处 处理x和y之间的连接
        y->right = x;
        x->parent = y;
    }
    // color flip
    // 对于四节点类型 向上传递中间节点x
    inline void
    __rb_tree_color_filp(rb_tree_node_base* x)
    {
        x->color = rb_tree_color::red;
        x->left->color = rb_tree_color::black;
        x->right->color = rb_tree_color::black;
    }
    // rebalance
    inline void
    __rb_tree_rebalance(rb_tree_node_base* x, rb_tree_node_base*& root)
    {
        x->color = rb_tree_color::red;
        while( x != root && x->parent->color == rb_tree_color::red)
        {
            if( x->parent == x->parent->parent->left)
            {
                rb_tree_node_base* x_pp_r = x->parent->parent->right;
                if(x_pp_r && x_pp_r->color == rb_tree_color::black)
                {
                    // 这种情况下 x的父节点和父节点的兄弟节点都为红
                    // 所以x的父节点、祖父节点和父节点的兄弟节点会组成一个4-node
                    // 该4-node允许存在树中，并且也是对应else情况结束后的状态
                    // 但新插入节点x也为红色，会与上层的4-node组成5-node
                    // 5-node不允许出现在2-3-4树中，需要向上传递中间值做color flip
                    x = x->parent->parent;
                    __rb_tree_color_filp(x);
                }
                else // x的父节点没有兄弟节点 或者为黑
                {
                    // 如果新增节点x是一个右节点，需要将其转到与x父节点一致的方向
                    // 即偏左 左节点 使用左旋操作
                    if(x == x->parent->right)
                    {
                        x = x->parent;
                        __rb_tree_rotate_left(x, root);
                    }
                    // 这时 x与x的父节点方向一致 且都为红色
                    // 于是x和x的父节点与祖父节点组成一个4-node，但该4-node当前以一边展开
                    // 会导致树的不平衡（x所在另一边可能会比较长）需要调整至从中间向两边展开
                    // 即该4-node应该以x的父节点连接上层而非x的祖父节点
                    // 对当前展开状态的4-node使用右旋
                    x->parent->color = rb_tree_color::black;
                    x->parent->parent->color = rb_tree_color::red;
                    __rb_tree_rotate_right(x->parent->parent, root);
                    // 经过右旋后该4-node以平衡的状态展开 直到下一次被对应的if处理
                }
            }
            else // x->parent == x->parent->parent->right
            {
                // 与该else对应if 逻辑相同 操作反向
                rb_tree_node_base* x_pp_l = x->parent->parent->left;
                if(x_pp_l && x_pp_l->color == rb_tree_color::red)
                {
                    x = x->parent->parent;
                    __rb_tree_color_filp(x);
                }
                else
                {
                    if(x == x->parent->left)
                    {
                        x = x->parent;
                        __rb_tree_rotate_right(x, root);
                    }
                    x->parent->color = rb_tree_color::black;
                    x->parent->parent->color = rb_tree_color::red;
                    __rb_tree_rotate_left(x->parent->parent, root);
                }
            }
        }
        // 如果root的颜色由下层传递color flip改变了，实际上对应的2-3-4树经历了一次节点分裂
        // root左右两边高度各加一 所以可以维持各子树高度完美平衡
        // 新增节点意味着子树高度改变 但这个新增节点总发生在根节点 于是传递下去所有子树高度都加一
        // 2-3-4树延迟的新增节点的操作 将其扩增节点的需求不断向上传递 累积于根节点处
        // 而通常的AVL树在叶节点层新增节点 需要重复地向上调整 以应对新增节点的影响
        // 而2-3-4树所具化的红黑树 使用color filp消除一部分影响
        root->color = rb_tree_color::black; 
    }
            
    // erase rebalance
    inline void
    __rb_tree_erase_rebalance(rb_tree_node_base* z, rb_tree_node_base*& root, rb_tree_node_base*& leftmost, rb_tree_node_base*& rightmost)
    {
        rb_tree_node_base* y = z;
        rb_tree_node_base* x = 0;
        rb_tree_node_base* x_parent = 0;
        if (y->left == 0)            
            x = y->right;            
        else
            if (y->right == 0)         
            x = y->left;             
            else {          
            y = y->right;   
            while (y->left != 0)
                y = y->left;
            x = y->right;
            }
        if (y != z) {   
            z->left->parent = y; 
            y->left = z->left;
            if (y != z->right) {
            x_parent = y->parent;
            if (x) x->parent = y->parent;
            y->parent->left = x;
            y->right = z->right;
            z->right->parent = y;
            }
            else
            x_parent = y;  
            if (root == z)
            root = y;
            else if (z->parent->left == z)
            z->parent->left = y;
            else 
            z->parent->right = y;
            y->parent = z->parent;
            my_stl::swap(y->color, z->color);
            y = z;

        }
        else {          
            x_parent = y->parent;
            if (x) x->parent = y->parent;   
            if (root == z)
                {root = x;}
            else 
            {
                if (z->parent->left == z)
                {z->parent->left = x;}
                else
                {
                    z->parent->right = x;
                }
            }
            if (leftmost == z) 
            {
                if (z->right == 0)        
                {
                    leftmost = z->parent;
                }
                else
                {
                    leftmost = rb_tree_minimum(x);
                }
            }
            if (rightmost == z)  
            {
                if (z->left == 0)       
                    rightmost = z->parent;  
                else                    
                    rightmost = rb_tree_maximum(x);
            }
        }
        if (y->color != rb_tree_color::red) { 
            while (x != root && (x == 0 || x->color == rb_tree_color::black))
            if (x == x_parent->left) {
                rb_tree_node_base* w = x_parent->right;
                if (w->color == rb_tree_color::red) {
                w->color = rb_tree_color::black;
                x_parent->color = rb_tree_color::red;
                __rb_tree_rotate_left(x_parent, root);
                w = x_parent->right;
                }
                if ((w->left == 0 || w->left->color == rb_tree_color::black) &&
                    (w->right == 0 || w->right->color == rb_tree_color::black)) {
                w->color = rb_tree_color::red;
                x = x_parent;
                x_parent = x_parent->parent;
                } else {
                if (w->right == 0 || w->right->color == rb_tree_color::black) {
                    if (w->left) w->left->color = rb_tree_color::black;
                    w->color = rb_tree_color::red;
                    __rb_tree_rotate_right(w, root);
                    w = x_parent->right;
                }
                w->color = x_parent->color;
                x_parent->color = rb_tree_color::black;
                if (w->right) w->right->color = rb_tree_color::black;
                __rb_tree_rotate_left(x_parent, root);
                break;
                }
            } else {                  // same as above, with right <-> left.
                rb_tree_node_base* w = x_parent->left;
                if (w->color == rb_tree_color::red) {
                w->color = rb_tree_color::black;
                x_parent->color = rb_tree_color::red;
                __rb_tree_rotate_right(x_parent, root);
                w = x_parent->left;
                }
                if ((w->right == 0 || w->right->color == rb_tree_color::black) &&
                    (w->left == 0 || w->left->color == rb_tree_color::black)) {
                w->color = rb_tree_color::red;
                x = x_parent;
                x_parent = x_parent->parent;
                } else {
                if (w->left == 0 || w->left->color == rb_tree_color::black) {
                    if (w->right) w->right->color = rb_tree_color::black;
                    w->color = rb_tree_color::red;
                    __rb_tree_rotate_left(w, root);
                    w = x_parent->left;
                }
                w->color = x_parent->color;
                x_parent->color = rb_tree_color::black;
                if (w->left) w->left->color = rb_tree_color::black;
                __rb_tree_rotate_right(x_parent, root);
                break;
                }
            }
            if (x) x->color = rb_tree_color::black;
        }
    }
    
    /* 
        红黑树类模板，第一模板参数为红黑树节点的值域类型，第二参数为键选择函数对象，对于set为identity，对于map为selectfirst
       第三模板参数为键比较函数对象。
    */
    template <class _Value, class _KeyOfValue, class Compare>
    class rb_tree
    {

        public:
            typedef _Value                              value_type;
            // 对于STL中set和map 分别使用identity和selectfirst函数对象都是pulic继承unarg_function
            // 这里使用一元函数对象unarg_function的返回值类型 定义 键类型
            // 刚好够用 避免SGI中使用两个模板参数分别定义键和值的类型 更具统一性
            typedef typename _KeyOfValue::result_type   key_type;
            typedef size_t                              size_type;
            typedef ptrdiff_t                           difference_type;
            typedef _Value*                             pointer;
            typedef _Value&                             reference;
            typedef const _Value*                       const_pointer;
            typedef const _Value&                       const_reference;

            typedef rb_tree_node_base                   base_type;
            typedef rb_tree_node_base*                  base_ptr;
            typedef rb_tree_node<_Value>                node_type;
            typedef rb_tree_node<_Value>*               node_ptr;

            typedef Compare                             key_compare;

            typedef my_stl::simple_alloc<_Value, alloc>          data_allocator;
            typedef my_stl::simple_alloc<base_type, alloc>  base_allocator;
            typedef my_stl::simple_alloc<node_type, alloc>  node_allocator;

            typedef rb_tree_iterator<_Value>         iterator;
            typedef rb_tree_const_iterator<_Value>   const_iterator;

        
        // 三个数据成员 表示一个RB Tree
        private:
            base_ptr    _M_header;
            size_type   _M_node_count;
            key_compare _M_key_compare;

        // 三个成员函数 取得根、最小和最大节点
        private:
            base_ptr& root()        const { return _M_header->parent;}
            base_ptr& leftmost()    const { return _M_header->left;}
            base_ptr& rightmost()   const { return _M_header->right;}

            static base_ptr& left(base_ptr x)
            { return x->left;}
            static base_ptr& right(base_ptr x)
            { return x->right;}
            static base_ptr parent(base_ptr x)
            { return x->parent;}
            static reference value(base_ptr x)
            { return node_ptr(x)->value;}
            static const key_type& key(base_ptr x)
            { return _KeyOfValue()(value(x));}
        // 构造、复制和析构函数
        public:

            rb_tree() { rb_tree_init();}

            rb_tree(const rb_tree& rhs);
            rb_tree(rb_tree&& rhs) noexcept;

            rb_tree& operator=(const rb_tree& rhs);
            rb_tree& operator=(rb_tree&& rhs);

            ~rb_tree() {clear();}

        // 辅助函数
        private:
            void rb_tree_init();
            node_ptr create_node(value_type value);
            node_ptr clone_node(base_ptr x);
            base_ptr copy_from(base_ptr x, base_ptr p);
            void destroy_node(node_ptr node)
            {
                destroy(&node->value);
                node_allocator::deallocate(node);
            }


        // 迭代器相关操作
        public:
            iterator begin()    noexcept
            { return leftmost();}
            iterator end()      noexcept
            { return this->_M_header;}

        // 容量相关操作
        public:
            bool empty()            const   noexcept
            { return _M_node_count == 0;}
            size_type size()        const   noexcept
            { return _M_node_count;}
            size_type max_size()    const   noexcept
            { return size_type(-1);}

        // 插入
        public:
            // 可重复插入
            iterator insert_equal(const value_type& v);
            // 不可重复插入
            my_stl::pair<iterator, bool> insert_unique(const value_type& v);
            template <class InputIter>
            void insert_unique(InputIter first, InputIter last)
            {
                size_type n = my_stl::distance(first, last);
                for(; n > 0; --n, ++first)
                {
                    this->insert_unique(*first);
                }
            }
        
        // 删除
        public:
            iterator erase(iterator position);
            size_type erase(const key_type& key);
            void erase(iterator first, iterator last);
            void clear();
        // 查找
        iterator find(const value_type& value);
        iterator lower_bound(const key_type& key);
        iterator upper_bound(const key_type& key);
        my_stl::pair<iterator, iterator> equal_range(const key_type& k);
        void swap(rb_tree& rhs) noexcept;

        size_type count_unique(const key_type& key) const
        {
            return find(key) ? 1 : 0;
        }

        private:
            iterator __insert(base_ptr x, const value_type& value, bool insert_left);
            void erase_since(base_ptr x);
    };

    template <class _Value, class _KeyOfValue, class Compare>
    void
    rb_tree<_Value, _KeyOfValue, Compare>::clear()
    {
        if(this->_M_node_count != 0)
        {
            this->erase_since(this->root());
            this->leftmost() = this->_M_header;
            this->rightmost() = this->_M_header;
            this->root() = nullptr;
            this->_M_node_count = 0;
        }
    }
    
    template <class _Value, class _KeyOfValue, class Compare>
    void
    rb_tree<_Value, _KeyOfValue, Compare>::erase_since(base_ptr x)
    {
        while( nullptr != x)
        {
            // 主循环在左子树上循环删除
            // 并将右子树递归删除
            // 可以避免重复删除
            erase_since(x->right);
            auto y = x->left;
            destroy_node(node_ptr(x));
            x = y;
        }
    }

    template <class _Value, class _KeyOfValue, class Compare>
    void
    rb_tree<_Value, _KeyOfValue, Compare>::swap(rb_tree& rhs) noexcept
    {
        if (this != &rhs)
        {
            my_stl::swap(this->_M_header, rhs._M_header);
            my_stl::swap(this->_M_node_count, rhs._M_node_count);
            my_stl::swap(this->_M_key_compare, rhs._M_key_compare);
        }
    }
    // 执行插入操作
    template <class _Value, class _KeyOfValue, class Compare>
    typename rb_tree<_Value, _KeyOfValue, Compare>::iterator
    rb_tree<_Value, _KeyOfValue, Compare>::__insert(base_ptr x, const value_type& value, bool insert_left)
    {
        node_ptr node = create_node(value);
        node->parent = x;
        if(x == this->_M_header)
        {
            this->root() = node;
            this->rightmost() = node;
            this->leftmost() = node;
        }
        else if(insert_left)
        {
            x->left = node;
            if(this->leftmost() == x)
            {
                this->leftmost() = node;
            }
        }
        else
        {
            x->right = node;
            if(this->rightmost() == x)
            {
                this->rightmost() = node;
            }
        }
        // 重新平衡
        __rb_tree_rebalance(node, root());
        ++this->_M_node_count;
        return iterator(node);
    }


    // 插入 可重复
    template <class _Value, class _KeyOfValue, class Compare>
    typename rb_tree<_Value, _KeyOfValue, Compare>::iterator
    rb_tree<_Value, _KeyOfValue, Compare>::insert_equal(const value_type& value)
    {
        base_ptr x_p = this->_M_header;
        base_ptr x = root();
        bool insert_left = true; 
        while(x != nullptr)
        {
            x_p = x;
            insert_left = _M_key_compare(_KeyOfValue()(value), key(x));
            x = insert_left ? x->left : x->right;
        }
        return this->__insert(x_p, value, insert_left);
    }

    // 插入 不可重复
    template <class _Value, class _KeyOfValue, class Compare>
    my_stl::pair<typename rb_tree<_Value, _KeyOfValue, Compare>::iterator, bool>
    rb_tree<_Value, _KeyOfValue, Compare>::insert_unique(const value_type& value)
    {
        auto x = root();
        auto y = this->_M_header;
        bool insert_left = true;
        while(x != nullptr)
        {
            y = x;
            insert_left = this->_M_key_compare(_KeyOfValue()(value), key(x));
            x = insert_left ? x->left : x->right;
        }
        iterator j = iterator(y);
        if(insert_left)
        {
            if(j == begin() || y == this->_M_header)
            {
                return my_stl::make_pair(this->__insert(y, value, insert_left), true);
            }
            else
            { --j;}
        }
        if(this->_M_key_compare(_KeyOfValue()(*j), _KeyOfValue()(value)))
        {
            return my_stl::make_pair(this->__insert(y, value, insert_left), true);
        }
        return my_stl::make_pair(j, false);

    }

    // 删除
    template <class _Value, class _KeyOfValue, class Compare>
    typename rb_tree<_Value, _KeyOfValue, Compare>::iterator
    rb_tree<_Value, _KeyOfValue, Compare>::erase(iterator position)
    {
        iterator next(position);
        ++next;

        __rb_tree_erase_rebalance(position.node, root(), leftmost(), rightmost());
        destroy_node(node_ptr(position.node));
        --this->_M_node_count;
        return next;
    }

    template <class _Value, class _KeyOfValue, class Compare>
    typename rb_tree<_Value, _KeyOfValue, Compare>::size_type 
    rb_tree<_Value, _KeyOfValue, Compare>::erase(const key_type& key)
    {
        my_stl::pair<iterator, iterator> p = equal_range(key);
        auto n = my_stl::distance(p.first, p.second);
        erase(p.first, p.second);
        return n;
    }

    template <class _Value, class _KeyOfValue, class Compare>
    void
    rb_tree<_Value, _KeyOfValue, Compare>::erase(iterator first, iterator last)
    {
        if(first == begin() && last == end())
        {
            clear();
        }
        else
        {
            while(first != last)
            {
                erase(first++);
            }
        }
    }

    // find
    template <class _Value, class _KeyOfValue, class Compare>
    typename rb_tree<_Value, _KeyOfValue, Compare>::iterator
    rb_tree<_Value, _KeyOfValue, Compare>::find(const value_type& value)
    {
        auto x = root();
        auto y = this->_M_header;
        while(x != nullptr)
        {
            if(!this->_M_key_compare(node_ptr(x)->value, value))
            {
                y = x;
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }
        iterator j = iterator(y);
        return (j == end() || this->_M_key_compare(value, *j)) ? end() : j;
    }
    
    // lower_bound
    template <class _Value, class _KeyOfValue, class Compare>
    typename rb_tree<_Value, _KeyOfValue, Compare>::iterator
    rb_tree<_Value, _KeyOfValue, Compare>::lower_bound(const key_type& key)
    {
        auto x = root();
        auto y = this->_M_header;
        while(x != nullptr)
        {
            // !(x < key) == x >= key
            if(!this->_M_key_compare(this->key(x), key))
            {
                y = x;
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }
        return iterator(y);
    }

    // upper_bound
    template <class _Value, class _KeyOfValue, class Compare>
    typename rb_tree<_Value, _KeyOfValue, Compare>::iterator
    rb_tree<_Value, _KeyOfValue, Compare>::upper_bound(const key_type& key)
    {
        auto x = root();
        auto y = this->_M_header;
        while(x != nullptr)
        {
            // x > key == key < x
            if(this->_M_key_compare(key, this->key(x)))
            {
                y = x;
                x = x->left;
            }
            else
            {
                x = x->right;
            }
        }
        return iterator(y);        
    }

    // equal_range
    template <class _Value, class _KeyOfValue, class Compare>
    my_stl::pair<typename rb_tree<_Value, _KeyOfValue, Compare>::iterator,
                typename rb_tree<_Value, _KeyOfValue, Compare>::iterator>
    rb_tree<_Value, _KeyOfValue, Compare>::equal_range(const key_type& key)
    {
        return pair<iterator, iterator>(lower_bound(key), upper_bound(key));
    }

    // 拷贝构造函数
    template <class _Value, class _KeyOfValue, class Compare>
    rb_tree<_Value, _KeyOfValue, Compare>::rb_tree(const rb_tree& rhs)
    {
        rb_tree_init();
        if (rhs._M_node_count != 0)
        {
            root() = copy_from(rhs.root(), this->_M_header);
            leftmost() = rb_tree_minimum(root());
            rightmost() = rb_tree_maximum(root());
        }
        this->_M_node_count = rhs._M_node_count;
        this->_M_key_compare = rhs._M_key_compare;
    }

    // 移动构造函数
    // 注意成员初始化顺序要和声明顺序一致 否则出现警告  warning "will be initialized after [-Wreorder]
    template <class _Value, class _KeyOfValue, class Compare>
    rb_tree<_Value, _KeyOfValue, Compare>::rb_tree(rb_tree&& rhs) noexcept
    : _M_header(my_stl::move(rhs._M_header)),
    _M_node_count(rhs._M_node_count),
    _M_key_compare(rhs._M_key_compare)
    {
        rhs._M_header = nullptr;
        rhs._M_node_count = 0;
    }

    // 复制赋值函数
    template <class _Value, class _KeyOfValue, class Compare>
    rb_tree<_Value, _KeyOfValue, Compare>&
    rb_tree<_Value, _KeyOfValue, Compare>::operator=(const rb_tree& rhs)
    {
        if(this != &rhs)
        {
            // 安全清除当前资源
            clear();
            if(0 != rhs._M_node_count)
            {
                root() = copy_from(rhs.root(), this->_M_header);
                leftmost() = rb_tree_minimum(root());
                rightmost() = rb_tree_maximum(root());
            }
            this->_M_node_count = rhs._M_node_count;
            this->_M_key_compare = rhs._M_key_compare;
        }
        return *this;
    }

    // 移动赋值函数
    template <class _Value, class _KeyOfValue, class Compare>
    rb_tree<_Value, _KeyOfValue, Compare>&
    rb_tree<_Value, _KeyOfValue, Compare>::operator=(rb_tree&& rhs)
    {
        this->clear();
        this->_M_header = my_stl::move(rhs._M_header);
        this->_M_node_count = rhs._M_node_count;
        this->_M_key_compare = rhs._M_key_compare;
        rhs._M_header = nullptr;
        rhs._M_node_count = 0;
        return *this;
    }

    // 初始化辅助函数
    template <class _Value, class _KeyOfValue, class Compare>
    void
    rb_tree<_Value, _KeyOfValue, Compare>::rb_tree_init()
    {
        _M_header = base_allocator::allocate(1);
        _M_header->color = rb_tree_color::red;
        root() = nullptr;
        leftmost() = nullptr;
        rightmost() = nullptr;
        _M_node_count = 0;
    }

    template <class _Value, class _KeyOfValue, class Compare>
    typename rb_tree<_Value, _KeyOfValue, Compare>::node_ptr
    rb_tree<_Value, _KeyOfValue, Compare>::create_node(value_type value)
    {
        auto tmp = node_allocator::allocate(1);
        try
        {
            construct(&(tmp->value), value);
            tmp->left = nullptr;
            tmp->right = nullptr;
            tmp->parent = nullptr;
        }
        catch(...)
        {
            node_allocator::deallocate(tmp);
            throw;
        }
        return tmp;
    }

    template <class _Value, class _KeyOfValue, class Compare>
    typename rb_tree<_Value, _KeyOfValue, Compare>::node_ptr
    rb_tree<_Value, _KeyOfValue, Compare>::clone_node(base_ptr x)
    {
        node_ptr tmp = create_node(node_ptr(x)->value);
        tmp->color = x->color;
        tmp->left = nullptr;
        tmp->right = nullptr;
        return tmp;
    }

    template <class _Value, class _KeyOfValue, class Compare>
    typename rb_tree<_Value, _KeyOfValue, Compare>::base_ptr
    rb_tree<_Value, _KeyOfValue, Compare>::copy_from(base_ptr x, base_ptr p)
    {
        auto top = clone_node(x);
        top->parent = p;
        try
        {
            if (x->right)
            top->right = copy_from(x->right, top);
            p = top;
            x = x->left;
            while (x != nullptr)
            {
            auto y = clone_node(x);
            p->left = y;
            y->parent = p;
            if (x->right)
                y->right = copy_from(x->right, y);
            p = y;
            x = x->left;
            }
        }
        catch (...)
        {
            erase_since(top);
            throw;
        }
        return top;
    }
    
} /* my_stl */

#endif /* XT_STL_TREE_H */