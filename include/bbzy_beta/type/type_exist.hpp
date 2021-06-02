#pragma once

#include <type_traits>

namespace bbzy
{
namespace type
{
namespace detail
{
template<class T>
struct IsTypeExist
{
    enum
    {
        value = std::is_same<T, T>::value
    };
};
}

using detail::IsTypeExist;

template<class T>
using TExist = IsTypeExist<T>;

}
}