
#ifndef __XT_STL_INTERNAL_HEAP_H
#define __XT_STL_INTERNAL_HEAP_H

#include "stl_iterator.h"

namespace my_stl
{

    /********************************************************************/
    // 1 push_heap
    template <class RandomAccessIterator, class Distance, class T>
    void __push_heap(RandomAccessIterator first, Distance holeIndex, 
                Distance topIndex, T value)
    {
        // 从下标0开始的， 2 * index + 2 是右节点
        Distance parent = (holeIndex - 1) / 2;
        // 判断是否需要上浮
        while(holeIndex > topIndex && *(first + parent) < value)
        {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        // 最后找到合适的位置 放入新增元素
        *(first + holeIndex) = value;
    }

    template <class RandomAccessIterator, class Distance>
    void push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*)
    {
        // last指向最后一个元素的下一个位置
        // 因为都使用的first+index的方式，所以topIndex为0总是成立的
        __push_heap(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1));
    }

    // 新增元素已经位于最尾端
    template <class RandomAccessIterator>
    void push_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        push_heap_aux(first, last, distance_type(first));
    }

    // 重载版本，使用 comp 所指定的比较操作
    template <class RandomAccessIterator, class Distance, class T, class Compared>
    void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex,
                    T value, Compared comp)
    {
        Distance parent = (holeIndex - 1) / 2;
        while(holeIndex > topIndex && comp(*(first + parent), value))
        {
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }
    template <class RandomAccessIterator, class Distance, class Compared>
    void push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*,
                    Compared comp)
    {
        my_stl::__push_heap(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1), comp);
    }
    template <class RandomAccessIterator, class Compared>
    void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compared comp)
    {
        my_stl::push_heap_aux(first, last, distance_type(first), comp);
    }

    // 2 pop_heap
    template <class RandomAccessIterator, class T, class Distance>
    void adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
    {
        Distance topIndex = holeIndex;
        Distance rchild = 2 * holeIndex + 2;
        // 执行下溯过程，在将hole放到叶节点的过程中，梳理好子树上的节点
        // 最后再执行一次上溯过程为原来的最后一个元素找到合适位置。
        while (rchild < len)
        {
            if (*(first + rchild) < *(first + rchild - 1))
            {
                --rchild;
            }
            *(first + holeIndex) = *(first + rchild);
            holeIndex = rchild;
            rchild = 2 * holeIndex + 2;
        }
        // 该len是去掉一个节点后的长度，所以下式有机会成立
        if (rchild == len)
        {
            *(first + holeIndex) = *(first + rchild - 1);
            holeIndex = rchild - 1;
        }
        // 上溯过程：为原数组最后一个元素找到合适位置
        __push_heap(first, holeIndex, topIndex, value);
    }
    template <class RandomAccessIterator, class T, class Distance>
    void pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last,
                    RandomAccessIterator result, T value, Distance*)
    {
        *result = *first;
        adjust_heap(first, static_cast<Distance>(0), last - first, value);
    }

    template <class RandomAccessIterator>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        // 执行下列函数，将首元素放到尾部，
        // 并调整[first, last - 1)使之成为一个max-heap
        pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first));
    }

    template <class RandomAccessIterator, class T, class Distance, class Compared>
    void adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len,
                 T value, Compared comp)
    {
        Distance topIndex = holeIndex;
        Distance rchild = 2 * topIndex + 2;
        while (rchild < len)
        {
            if (comp(*(first + rchild), *(first + rchild - 1)))
            {
                --rchild;
            }
            *(first + holeIndex) = *(first + rchild);
            holeIndex = rchild;
            rchild = 2 * holeIndex + 2; 
        }
        if (rchild == len)
        {
            *(first + holeIndex) = *(first + rchild - 1);
            holeIndex = rchild - 1;
        }
        // 此处不加 my_stl 产生错误 __push_heap是ambiguous 与 std中的__push_heap 冲突
        my_stl::__push_heap(first, holeIndex, topIndex, value, comp);
    }

    template <class RandomAccessIterator, class T, class Distance, class Compared>
    void pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last,
                    RandomAccessIterator result, T value, Distance*, Compared comp)
    {
        *result = *first;
        adjust_heap(first, static_cast<Distance>(0), last - first, value, comp);
    }

    template <class RandomAccessIterator, class Compared>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compared comp)
    {
        my_stl::pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first), comp);
    }


    // 3 sort_heap
    template <class RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        // 每pop出来一个就放到尾部，全部pop出来就完成了排序
        while ( (last - first) > 1)
        {
            pop_heap(first, last);
            --last;
        }
    }
    template <class RandomAccessIterator, class Compared>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compared comp)
    {
        while ( (last - first) > 1)
        {
            my_stl::pop_heap(first, last, comp);
            --last;
        }
    }

    // 4 make_heap 线性时间复杂度
    template <class RandomAccessIterator, class Distance>
    void make_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*)
    {
        // 只有一个元素
        if ((last - first) < 2)
        {
            return;
        }

        Distance len = last - first;
        // 求父节点是 (curIndex - 1) / 2
        // 由于从0开始，所以curIndex = len - 1
        Distance parent = (len - 2) / 2;
        while(-1 != parent)
        {
            adjust_heap(first, parent, len, *(first + parent));
            --parent;
        }
    }
    template <class RandomAccessIterator>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        make_heap_aux(first, last, distance_type(first));
    }

    // 重载版本
    template <class RandomAccessIterator, class Distance, class Compared>
    void make_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, Compared comp)
    {
        if ((last - first) < 2)
        {
            return;
        }
        Distance len = last - first;
        Distance parent = (len - 2) / 2;
        while (-1 != parent)
        {
            adjust_heap(first, parent, len, *(first + parent), comp);
            --parent;
        }
    }

    template <class RandomAccessIterator, class Compared>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compared comp)
    {
        make_heap_aux(first, last, distance_type(first), comp);
    }

} /* namespace my_stl */

#endif /* __XT_STL_INTERNAL_HEAP_H */