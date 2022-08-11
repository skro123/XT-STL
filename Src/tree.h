
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
                while(node = y->left)
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

        typedef rb_tree_node_base   base_ptr;
        typedef rb_tree_node<T>*    node_ptr;
        typedef rb_tree_iterator<T> iterator;

        using rb_tree_base_iterator::node;

        rb_tree_iterator() {}
        rb_tree_iterator(base_ptr x) { node = x;}
        rb_tree_iterator(node_ptr x) { node = x;}
        rb_tree_iterator(const iterator& rhs) { node = rhs.node;}

        reference operator*() const { return node_ptr(x)->value;}
        pointer operator->() const { return &(operator*());}

        

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
            
    template <class T, class Compare>
    class rb_tree
    {

        typedef T           value_type;
        typedef size_t      size_type;

        typedef rb_tree_node_base   base_type;
        typedef rb_tree_node_base*  base_ptr;
        typedef rb_tree_node        node_type;
        typedef rb_tree_node<T>*    node_ptr;

        typedef Compare             key_compare;

        typedef my_stl::simple_alloc<base_type, alloc>  base_allocator;
        typedef my_stl::simple_alloc<node_type, alloc>  node_allocator;
        
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

            rb_tree(const rb_tree& rhs)
            {
                rb_tree_init();

            }
            rb_tree(rb_tree&& rhs) noexcept;

            rb_tree& operator=(rb_tree& rhs);
            rb_tree& operator=(const rb_tree& rhs);

            ~rb_tree() {clear();}

        private:
            void rb_tree_init()
            {
                _M_header = base_allocator::allocate(1);
                _M_header->color = rb_tree_color::red;
                root() = nullptr;
                leftmost() = nullptr;
                rightmost() = nullptr;
                _M_node_count = 0;
            }

            node_ptr create_node(value_type value)
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

            node_ptr clone_node(base_prt x)
            {

            }


        public:
        void clear();




    }
}



#endif