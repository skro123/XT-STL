
#ifndef __XT_STL_INTERNAL_TREE_H
#define __XT_STL_INTERNAL_TREE_H


#include "stl_iterator.h"
#include "stl_alloc.h"
#include "stl_contruct.h"
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

    template <class T>
    struct rb_tree_node : public rb_tree_node_base
    {
        typedef T                   value_type;
        typedef rb_tree_node<T>*    node_ptr;

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
        typedef bidirectional_iterator_tag iteartor_category;

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
    };

    template <class T>
    struct rb_tree_iterator : public rb_tree_base_iterator
    {
        
        typedef T                   value_type;
        typedef T*                  pointer;
        typedef T&                  reference;

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
            
    template <class T, class Compare>
    class rb_tree
    {

        typedef T           value_type;
        typedef size_t      size_type;

        typedef rb_tree_node_base   base_type;
        typedef rb_tree_node_base*  base_ptr;
        typedef rb_tree_node<T>     node_type;
        typedef rb_tree_node<T>*    node_ptr;

        typedef Compare             key_compare;

        typedef my_stl::simple_alloc<base_type, alloc>  base_allocator;
        typedef my_stl::simple_alloc<node_type, alloc>  node_allocator;

        typedef rb_tree_iterator<T>     iterator;
        
        // 三个数据成员 表示一个RB Tree
        private:
            size_type   _M_node_count;
            base_ptr    _M_header;
            key_compare _M_key_compare;

        // 三个成员函数 取得根、最小和最大节点
        private:
            base_ptr& root()        const { return _M_header->parent;}
            base_ptr& leftmost()    const { return _M_header->left;}
            base_ptr& rightmost()   const { return _M_header->right;}

        // 构造、复制和析构函数
        public:

            rb_tree() { rb_tree_init();}

            rb_tree(const rb_tree& rhs);
            rb_tree(rb_tree&& rhs) noexcept;

            rb_tree& operator=(rb_tree& rhs);
            rb_tree& operator=(const rb_tree& rhs);

            ~rb_tree() {clear();}

        // 辅助函数
        private:
            void rb_tree_init();
            node_ptr create_node(value_type value);
            node_ptr clone_node(base_ptr x);
            base_ptr copy_from(base_ptr x, base_ptr p);


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

        // 插入删除相关操作
        public:
            iterator insert_equal(const value_type& v);
            iterator insert_unique(const value_type& v);

        private:
            iterator __insert(base_ptr x, const value_type& value, bool insert_left);

            void clear() {}
            void erase_since(const base_ptr x) {}
    };


    // 执行插入操作
    template <class T, class Compare>
    typename rb_tree<T, Compare>::iterator
    rb_tree<T, Compare>::__insert(base_ptr x, const value_type& value, bool insert_left)
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
    template <class T, class Compare>
    typename rb_tree<T, Compare>::iterator
    rb_tree<T, Compare>::insert_equal(const value_type& value)
    {
        base_ptr x_p = this->_M_header;
        base_ptr x = root();
        bool insert_left = true; 
        while(x != nullptr)
        {
            x_p = x;
            insert_left = _M_key_compare(value, node_ptr(x)->value);
            x = insert_left ? x->left : x->right;
        }
        return this->__insert(x_p, value, insert_left);
    }

    // 插入 不可重复
    template <class T, class Compare>
    typename rb_tree<T, Compare>::iterator
    rb_tree<T, Compare>::insert_unique(const value_type& value)
    {

    }

    // 拷贝构造函数
    template <class T, class Compare>
    rb_tree<T, Compare>::rb_tree(const rb_tree& rhs)
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

    template <class T, class Compare>
    void
    rb_tree<T, Compare>::rb_tree_init()
    {
        _M_header = base_allocator::allocate(1);
        _M_header->color = rb_tree_color::red;
        root() = nullptr;
        leftmost() = nullptr;
        rightmost() = nullptr;
        _M_node_count = 0;
    }

    template <class T, class Compare>
    typename rb_tree<T, Compare>::node_ptr
    rb_tree<T, Compare>::create_node(value_type value)
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

    template <class T, class Compare>
    typename rb_tree<T, Compare>::node_ptr
    rb_tree<T, Compare>::clone_node(base_ptr x)
    {
        node_ptr tmp = create_node(node_ptr(x)->value);
        tmp->color = x->color;
        tmp->left = nullptr;
        tmp->right = nullptr;
        return tmp;
    }

    template <class T, class Compare>
    typename rb_tree<T, Compare>::base_ptr
    rb_tree<T, Compare>::copy_from(base_ptr x, base_ptr p)
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

#endif /* __XT_STL_INTERNAL_TREE_H */