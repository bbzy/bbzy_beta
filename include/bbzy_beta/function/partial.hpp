#pragma once

#include "bbzy_beta/type.hpp"
#include "bbzy_beta/unify.hpp"

namespace bbzy
{
namespace function
{
namespace detail
{
template<class FuncT, class... ArgTs>
class Partial
{
private:
    using DelegateFunctionT = decltype(unify::makeDelegateFunction(std::declval<FuncT>()));
    using ResType = type::ResType<FuncT>;

private:
    template<int... N>
    struct RangeCall;

    template<int N, int... Indices>
    struct RangeCall<N, Indices...>
    {
        using type = typename RangeCall<N - 1, N, Indices...>::type;
    };

    template<int... Indices>
    struct RangeCall<-1, Indices...>
    {
        using type = RangeCall<-1, Indices...>;

        template<class PartialObjectT, class... LeftArgTs>
        static ResType invoke(PartialObjectT&& partialObject, LeftArgTs&& ... leftArgs)
        {
            return partialObject->m_func(
                    std::forward<bbzy::type::TypeAt<Indices, ArgTs&& ...>>(std::get<Indices>(partialObject->m_args))...,
                    std::forward<LeftArgTs&&>(leftArgs)...
            );
        }
    };

    using RangeType = typename RangeCall<int(sizeof...(ArgTs)) - 1>::type;

public:
    Partial(FuncT func, ArgTs ... args) :
            m_func(unify::makeDelegateFunction(std::move(func))),
            m_args(std::move(args)...)
    {
    }

    Partial(const Partial&) = default;

    Partial(Partial&&) = default;

    Partial& operator=(const Partial&) = default;

    Partial& operator=(Partial&&) = default;

public:
    template<class... LeftArgTs>
    ResType operator()(LeftArgTs&& ... leftArgs)
    {
        return RangeType::invoke(this, std::forward<LeftArgTs&&>(leftArgs)...);
    }

    template<class... LeftArgTs>
    ResType operator()(LeftArgTs&& ... leftArgs) const
    {
        return RangeType::invoke(this, std::forward<LeftArgTs&&>(leftArgs)...);
    }

private:
    DelegateFunctionT m_func;
    std::tuple<ArgTs...> m_args;
};
}

/**
 * Simple bind function.
 * @param func Any callable including function, member function, lambda, function object or std::function.
 * @param args Bound arguments from beginning to the end, and you might not provide all arguments to the @func.
 * @return
 * @remarks
 *      Assume that:
 *          void foo(int a, int b, int c, int d);
 *      If you want:
 *          std::bind(&foo, 1, 2, std::placeholders::_1, std::placeholders::_2);
 *      You could simply use:
 *          partial(&foo, 1, 2);
 *      The member function case:
 *          struct S {
 *              void foo(int a, int b, int c, int d) const;
 *          };
 *      You could use:
 *          S s;
 *          partial(&S::foo, &s, 1, 2);
 *      or:
 *          partial(&S::foo, s, 1, 2);
 *      The this pointer or this reference is alternated.
 */
template<class FuncT, class... ArgTs>
detail::Partial<FuncT, ArgTs...> partial(FuncT func, ArgTs ... args)
{
    return {std::move(func), std::move(args)...};
}
}
}