#pragma once

#include <stdexcept>
#include <type_traits>
#include <functional>
#include "../common.hpp"
#include "../type.hpp"

namespace bbzy {
    namespace unify {
        namespace detail {
            template<class FunctionT>
            class DelegateFunction {
            private:
                using ResType = type::ResType<FunctionT>;

            public:
                inline DelegateFunction(FunctionT func) : m_function(std::move(func)) {
                }

                DelegateFunction(const DelegateFunction&) = default;

                DelegateFunction(DelegateFunction&&) = default;

                DelegateFunction& operator=(const DelegateFunction&) = default;

                DelegateFunction& operator=(DelegateFunction&&) = default;

            public:
                template<class... ArgTs>
                inline ResType operator()(ArgTs&& ... args) const {
                    return helper(nullptr, std::forward<ArgTs&&>(args)...);
                }

            private:
                template<class... ArgTs, class HelperFuncT = FunctionT, class = EnableIf<std::is_member_function_pointer<HelperFuncT>::value>>
                inline ResType helper(void*, ArgTs&& ... args) const {
                    return std::mem_fn(m_function)(std::forward<ArgTs&&>(args)...);
                }

                template<class... ArgTs, class HelperFuncT = FunctionT, class = EnableIf<!std::is_member_function_pointer<HelperFuncT>::value>>
                inline ResType helper(char*, ArgTs&& ... args) const {
                    return m_function(std::forward<ArgTs&&>(args)...);
                }

            private:
                FunctionT m_function;
            };
        }

        template<class FunctionT>
        detail::DelegateFunction<FunctionT> makeDelegateFunction(FunctionT func) {
            return {std::move(func)};
        }
    }
}
