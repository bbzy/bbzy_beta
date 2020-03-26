#pragma once

#include <type_traits>
#include "../common.hpp"

namespace bbzy {
    namespace disam {
        namespace detail {
            template<class ReturnT, class... FuncParamTs>
            struct Function {
                static auto get(ReturnT(*func)(FuncParamTs...)) ->
                ReturnT(*)(FuncParamTs...) {
                    return func;
                }
            };
        }

        using detail::Function;

    }
}
