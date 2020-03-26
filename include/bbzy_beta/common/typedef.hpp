#pragma once

#include <type_traits>

namespace bbzy {
    template<bool condition, class T = void>
    using EnableIf = typename std::enable_if<condition, T>::type;

    template<class T>
    using Decay = typename std::decay<T>::type;

    template<class T>
    using ResultOf = typename std::result_of<T>::type;

    template<bool condition, class T1, class T2>
    using Conditional = typename std::conditional<condition, T1, T2>::type;

    template<class T>
    using RemoveConst = typename std::remove_const<T>::type;

    template<class T>
    using AddLValueReference = typename std::add_lvalue_reference<T>::type;

    template<class T>
    using AddRValueReference = typename std::add_rvalue_reference<T>::type;

    template<class T>
    using AddConst = typename std::add_const<T>::type;

}
