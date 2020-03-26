#pragma once

#include <type_traits>
#include "../common.hpp"

namespace bbzy {
    namespace type {
        namespace detail {
            template<class T, class = EnableIf <std::is_pointer<T>::value>>
            constexpr bool isPointerEmpty(T value, void *) {
                return value == nullptr;
            };

            template<class T, class = EnableIf < std::is_pointer<T>::value == false>>

            constexpr bool isPointerEmpty(T, char *) {
                return false;
            };

        }

        template<class T>
        constexpr bool isPointerEmpty(T value) {
            return detail::isPointerEmpty(value, nullptr);
        }
    }
}
