#pragma once

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
 *      struct S {
 *          void foo(int);
 *          void foo(char);
 *          void foo(int) const;
 *      }
 *      and you want:
 *          &S::foo
 *      The compiler would tell you ambiguated member function reference, because the compiler does not even know which overloaded member function the code should be used.
 *      Then we could use MemberFunction<void, int>::get(&foo) to get the 1st foo()
 *      or use MemberFunction<void, char>::get(&foo) to get the 2nd foo()
 *      or use MemberFunction<void, int>::getConst(&foo) to get the 3th foo()
 */
template<class ReturnT, class... ParamTs>
struct MemberFunction
{
    template<class ClassT>
    static auto get(ReturnT(ClassT::*memberFunction)(ParamTs...)) -> ReturnT(ClassT::*)(ParamTs...)
    {
        return memberFunction;
    }

    template<class ClassT>
    static auto getConst(ReturnT(ClassT::*memberFunction)(ParamTs...) const) -> ReturnT(ClassT::*)(ParamTs...) const
    {
        return memberFunction;
    }
};
}
}
