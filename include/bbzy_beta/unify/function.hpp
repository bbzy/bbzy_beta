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
                using DecayedFunctionT = Decay<FunctionT>;
                using ResType = type::ResType<DecayedFunctionT>;

            public:
                inline DelegateFunction(FunctionT&& func) : m_function(std::forward<FunctionT&&>(func)) {
                }

            public:
                template<class... ArgTs>
                inline ResType operator()(ArgTs&& ... args) const {
                    return std::forward<ResType>(helper(nullptr, std::forward<ArgTs&&>(args)...));
                }

            private:
                template<class... ArgTs, class HelperFuncT = DecayedFunctionT, class = EnableIf<std::is_member_function_pointer<HelperFuncT>::value>>
                inline ResType helper(void*, ArgTs&& ... args) const {
                    return std::forward<ResType>(std::mem_fn(m_function)(std::forward<ArgTs&&>(args)...));
                }

                template<class... ArgTs, class HelperFuncT = DecayedFunctionT, class = EnableIf<!std::is_member_function_pointer<HelperFuncT>::value>>
                inline ResType helper(char*, ArgTs&& ... args) const {
                    return std::forward<ResType>(m_function(std::forward<ArgTs&&>(args)...));
                }

            private:
                FunctionT&& m_function;
            };
        }

        template<class FunctionT>
        detail::DelegateFunction<FunctionT&&> makeDelegateFunction(FunctionT&& func) {
            return {std::forward<FunctionT&&>(func)};
        }
    }
}
