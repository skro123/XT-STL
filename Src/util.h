#ifndef XTSTL_UTIL_H
#define XTSTL_UTIL_H

#include <cstddef>
#include "type_traits.h"

namespace my_stl
{


    // move
    template <class T>
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }


    // swap
    template <class Tp>
    void swap(Tp& lhs, Tp& rhs)
    {
        auto tmp(my_stl::move(lhs));
        lhs = my_stl::move(rhs);
        rhs = my_stl::move(tmp);
    }

} /* my_stl */

#endif /* XTSTL_UTIL_H */