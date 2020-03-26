#pragma once

namespace bbzy {
    namespace type {
        namespace detail {
            template<class... Ts>
            struct ResultType {
            private:
                template<class ObjectT, class... ArgTs>
                static auto
                help(ObjectT &&, ArgTs &&...) -> decltype(std::declval<ObjectT &&>()(std::declval<ArgTs &&>()...));

                template<class RetT, class... ArgTs>
                static RetT help(RetT(*)(ArgTs...), ...);

                template<class RetT, class... ArgTs>
                static RetT help(RetT(&)(ArgTs...), ...);

                template<class RetT, class ClassT, class... ArgTs>
                static RetT help(RetT(ClassT::*)(ArgTs...), ...);

                template<class RetT, class ClassT, class... ArgTs>
                static RetT help(RetT(ClassT::*)(ArgTs...) const, ...);

                template<class VarT, class ClassT>
                static VarT &help(VarT ClassT::*, ...);

            public:
                typedef decltype(help(std::declval<Ts>()...)) type;
            };

        }

/**
Extract return type.
Supported T:
1. Function Pointer;
2. Function Reference;
3. Member Function;
4. Constant Member Function;
5. Member Pointer(Returns reference);
6. Callable Type with its param types;
*/
        template<class... Ts>
        using ResultType = typename detail::ResultType<Ts...>::type;

        template<class... Ts>
        using ResType = ResultType<Ts...>;

    }
}
