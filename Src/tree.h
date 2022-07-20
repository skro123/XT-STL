
#ifndef __XT_STL_INTERNAL_TREE_H
#define __XT_STL_INTERNAL_TREE_H


namespace my_stl
{
    // 红黑节点颜色标志
    // 这里使用枚举，SGI 5使用枚举， SGI2.9和MyTintSTL使用两个bool常变量
    enum rb_tree_color { red = false, black = true };

    // 红黑树节点

    struct rb_tree_node_base
    {
        typedef rb_tree_node_base* base_ptr;
        typedef const rb_tree_node_base* const_base_ptr;

        // 数据成员
        base_ptr parent;
        base_ptr left;
        base_ptr right;
        rb_tree_color color;

        // 函数成员 
        // noexcept P474 拷贝控制
        static base_ptr minimum(base_ptr x) noexcept
        {
            while (x->left != nullptr) 
            {
                x = x->left;
            }
            return x;
        }
        static const_base_ptr minimum(const_base_ptr x) noexcept
        {
            while (x->left != nullptr)
            {
                x = x->left;
            }
            return x;
        }
        static base_ptr maximum(base_ptr x) noexcept
        {
            while (x->right != nullptr)
            {
                x = x->right;
            }
            return x;
        }
        static const_base_ptr maximum(const_base_ptr x) noexcept
        {
            while (x->right != nullptr)
            {
                x = x->right;
            }
            return x;
        }
    };

    template <class Val>
    struct rb_tree_node : public rb_tree_node_base
    {
        typedef rb_tree_node<Val>* link_type;

        // 数据成员 节点的键值 SGI5中考虑内存对齐
        Val value;

        Val* valptr()
        {
            return &value;
        }
        const Val* valptr() const
        {
            return &value;
        }

    };

    struct rb_teee_base_iterator
    {

    };
}

// 迭代器
template <class T>
struct rb_tree_iterator_base
{
    typedef rb_tree_node_base::base_ptr base_prt;
    
};


#endif