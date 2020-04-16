#pragma once

#include "bbzy_beta/type.hpp"
#include "bbzy_beta/unify.hpp"

namespace bbzy {
    namespace function {
        namespace detail {
            template<class FuncT, class... ArgTs>
            class Partial {
            private:
                using DelegateFunctionT = decltype(unify::makeDelegateFunction(std::declval<FuncT>()));
                using ResType = type::ResType<FuncT>;

            private:
                template<int... N>
                struct RangeCall;

                template<int N, int... Indices>
                struct RangeCall<N, Indices...> {
                    using type = typename RangeCall<N - 1, N, Indices...>::type;
                };

                template<int... Indices>
                struct RangeCall<-1, Indices...> {
                    using type = RangeCall<-1, Indices...>;

                    template<class PartialObjectT, class... LeftArgTs>
                    inline static ResType invoke(PartialObjectT&& partialObject, LeftArgTs&& ... leftArgs) {
                        return std::forward<ResType>(partialObject->m_func(
                                std::forward<bbzy::type::TypeAt<Indices, ArgTs&&...>>(std::get<Indices>(partialObject->m_args))...,
                                std::forward<LeftArgTs&&>(leftArgs)...
                        ));
                    }
                };

                using RangeType = typename RangeCall<int(sizeof...(ArgTs)) - 1>::type;

            public:
                inline Partial(FuncT&& func, ArgTs&& ... args) :
                        m_func(unify::makeDelegateFunction(std::forward<FuncT&&>(func))),
                        m_args(std::forward<ArgTs&&>(args)...) {
                }

                Partial(const Partial&) = default;

                Partial(Partial&&) = default;

            public:
                template<class... LeftArgTs>
                inline ResType operator()(LeftArgTs&& ... leftArgs) {
                    return std::forward<ResType>(RangeType::invoke(this, std::forward<LeftArgTs&&>(leftArgs)...));
                }

                template<class... LeftArgTs>
                inline ResType operator()(LeftArgTs&& ... leftArgs) const {
                    return std::forward<ResType>(RangeType::invoke(this, std::forward<LeftArgTs&&>(leftArgs)...));
                }

            private:
                DelegateFunctionT m_func;
                std::tuple<ArgTs&& ...> m_args;
            };
        }

        template<class FuncT, class... ArgTs>
        detail::Partial<FuncT, ArgTs...> partial(FuncT&& func, ArgTs&& ... args) {
            return {std::forward<FuncT&&>(func), std::forward<ArgTs&&>(args)...};
        }
    }
}