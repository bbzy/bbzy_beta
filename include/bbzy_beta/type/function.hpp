#pragma once

#include <type_traits>
#include "../common.hpp"
#include "index_type.hpp"
#include "element_type.hpp"
#include "type_wrapper.hpp"
#include "template_class.hpp"

namespace bbzy {
    namespace type {
        namespace detail {
            template<class FunctionT>
            struct IsFunction {
            private:
                using DecayedFunctionT = Decay<FunctionT>;

            private:
                template<class HelperT>
                struct Helper {
                    enum {
                        value = 0
                    };
                };

                template<class RetT, class... ParamTs>
                struct Helper<RetT(*)(ParamTs...)> {
                    enum {
                        value = 1
                    };
                };

            public:
                enum {
                    value = Helper<DecayedFunctionT>::value
                };
            };

            template<class MemberFunctionT>
            struct IsMemberFunction {
            private:
                using DecayedMemberFunctionT = Decay<MemberFunctionT>;

            private:
                template<class HelperT>
                struct Helper {
                    enum {
                        value = 0
                    };
                };

                template<class RetT, class ClassT, class... ParamTs>
                struct Helper<RetT(ClassT::*)(ParamTs...)> {
                    enum {
                        value = 1
                    };
                };

                template<class RetT, class ClassT, class... ParamTs>
                struct Helper<RetT(ClassT::*)(ParamTs...) const> {
                    enum {
                        value = 1
                    };
                };

            public:
                enum {
                    value = Helper<DecayedMemberFunctionT>::value
                };
            };

            template<class FunctionT>
            struct UnifiedFunctionType {
            private:
                template<class HelperFunT>
                struct Helper;

                template<class RetT, class... ParamTs>
                struct Helper<RetT(*)(ParamTs...)> {
                    typedef RetT(*type)(ParamTs...);
                };

                template<class RetT, class ClassT, class... ParamTs>
                struct Helper<RetT(ClassT::*)(ParamTs...)> {
                    typedef RetT(*type)(ClassT *, ParamTs...);
                };

                template<class RetT, class ClassT, class... ParamTs>
                struct Helper<RetT(ClassT::*)(ParamTs...) const> {
                    typedef RetT(*type)(const ClassT *, ParamTs...);
                };

            public:
                typedef typename Helper<Decay<FunctionT>>::type type;
            };

            template<class FunctionT>
            struct GetFunctionParamWrapper {
            private:
                template<class HelperFunctionT>
                struct Helper;

                template<class RetT, class... ParamTs>
                struct Helper<RetT(*)(ParamTs...)> {
                    using type = TypeWrapper<ParamTs...>;
                };

            public:
                using type = typename Helper<typename UnifiedFunctionType<FunctionT>::type>::type;
            };

            template<size_t index, class FunctionT>
            struct GetFunctionParamType {
            private:
                template<class HelperFunctionT>
                struct Helper;

                template<class RetT, class... ParamTs>
                struct Helper<RetT(*)(ParamTs...)> {
                    using type = typename TypeAt<index, ParamTs...>::type;
                };

            public:
                using type = typename Helper<typename UnifiedFunctionType<FunctionT>::type>::type;
            };

            template<class MemberFunctionT>
            struct GetMemberFunctionClassType {
                using type = EnableIf<IsMemberFunction<MemberFunctionT>::value,
                        ElemT<typename GetFunctionParamType<0, MemberFunctionT>::type>>;
            };

            template<class FunctionT, class... NewParamTs>
            struct ChangeFunctionParamsType {
            private:
                template<class HelperFunctionT>
                struct Helper;

                template<class RetT, class... HelperParamTs>
                struct Helper<RetT(HelperParamTs...)> {
                    using type = RetT(NewParamTs...);
                };

                template<class RetT, class... HelperParamTs>
                struct Helper<RetT(&)(HelperParamTs...)> {
                    using type = RetT(&)(NewParamTs...);
                };

                template<class RetT, class... HelperParamTs>
                struct Helper<RetT(*)(HelperParamTs...)> {
                    using type = RetT(*)(NewParamTs...);
                };

                template<class RetT, class ClsT, class... HelperParamTs>
                struct Helper<RetT(ClsT::*)(HelperParamTs...)> {
                    using type = RetT(ClsT::*)(NewParamTs...);
                };

                template<class RetT, class ClsT, class... HelperParamTs>
                struct Helper<RetT(ClsT::*)(HelperParamTs...) const> {
                    using type = RetT(ClsT::*)(NewParamTs...) const;
                };

            public:
                using type = typename Helper<FunctionT>::type;
            };

        }
        template<class FunctionT>
        using IsFunction = detail::IsFunction<FunctionT>;

        template<class MemberFunctionT>
        using IsMemberFunction = detail::IsMemberFunction<MemberFunctionT>;

        template<class FunctionT>
        using UnifiedFunctionType = typename detail::UnifiedFunctionType<FunctionT>::type;

        template<class FunctionT>
        using UniFunT = UnifiedFunctionType<FunctionT>;

        template<class FunctionT>
        using GetFunctionParamWrapper = typename detail::GetFunctionParamWrapper<FunctionT>::type;

        template<class FunctionT>
        using GetFunPW = GetFunctionParamWrapper<FunctionT>;

        template<size_t index, class FunctionT>
        using GetFunctionParamType = typename detail::GetFunctionParamType<index, FunctionT>::type;

        template<size_t index, class FunctionT>
        using GetFunPT = GetFunctionParamType<index, FunctionT>;

        template<class MemberFunctionT>
        using GetMemberFunctionClassType = typename detail::GetMemberFunctionClassType<MemberFunctionT>::type;

        template<class FunctionT, class... NewParamTs>
        using ChangeFunctionParamsType = typename detail::ChangeFunctionParamsType<FunctionT, NewParamTs...>::type;

        template<class FunctionT, class... NewParamTs>
        using ChFuncPT = ChangeFunctionParamsType<FunctionT, NewParamTs...>;

    }
}
