#pragma once

#include <type_traits>
#include "../common.hpp"

namespace bbzy
{
namespace disam
{
/**
 * Disambiguate from overloaded functions.
 * @tparam ReturnT Return type.
 * @tparam FuncParamTs Parameter types.
 * @remarks
 *      When you have:
 *          void foo(int);
 *          void foo(char);
 *      and you want:
 *          &foo
 *      The compile would tell you ambiguated function reference, because the compile does not even know which overloaded function the code should be used.
 *      Then we can use Function<void, int>::get(&foo) to get the 1st foo() or use Function<void, char>::get(&foo) to get the 2nd foo()
 */
template<class ReturnT, class... FuncParamTs>
struct Function
{
    static auto get(ReturnT(* func)(FuncParamTs...)) -> ReturnT(*)(FuncParamTs...)
    {
        return func;
    }
};
}
}
