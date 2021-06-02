#pragma once

#include <type_traits>

namespace bbzy
{
template<class T>
T&& constCast(T&& t)
{
    return t;
}

template<class T>
T&& constCast(const T&& t)
{
    return const_cast<T&&>(t);
}

template<class T>
T& constCast(const T& t)
{
    return const_cast<T&>(t);
}

template<class T>
T* constCast(const T* t)
{
    return const_cast<T*>(t);
}

}
