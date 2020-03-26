#pragma once

#include <tuple>

namespace bbzy {
    namespace type {
        template<class... Ts>
        struct TypeWrapper {
            enum {
                size = sizeof...(Ts)
            };
            using tuple = std::tuple<Ts...>;
        };
    }
}