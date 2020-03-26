#pragma once

#include <stdexcept>
#include <type_traits>
#include "../common.hpp"
#include "../type.hpp"

namespace bbzy {
    namespace unify {
        namespace detail {

            template<class FunctionT>
            class DelegateFunction {
            private:
                template<class HelperFunctionT, class... ArgTs>
                struct Helper {
                public:
                    typedef type::ResultType<HelperFunctionT &&, ArgTs &&...> ReturnType;

                public:
                    template<class CallFunctionT = HelperFunctionT,
                            class = EnableIf<type::IsFunction<CallFunctionT &&>::value>>
                    static ReturnType call(void *, CallFunctionT &&func, ArgTs &&... args) {
                        if (type::isPointerEmpty(func)) {
                            throw std::runtime_error("Null Pointer");
                        }
                        return std::forward<ReturnType>(func(std::forward<ArgTs &&>(args)...));
                    }

                    template<class CallFunctionT = HelperFunctionT,
                            class = EnableIf<type::IsMemberFunction<CallFunctionT &&>::value>>
                    static ReturnType call(char *, CallFunctionT &&func, ArgTs &&... args) {
                        if (type::isPointerEmpty(func)) {
                            throw std::runtime_error("Null Pointer");
                        }
                        return std::forward<ReturnType>(callMemberFunction(
                                std::forward<HelperFunctionT &&>(func), std::forward<ArgTs &&>(args)...));
                    }

                    template<class CallFunctionT = HelperFunctionT,
                            class = EnableIf<
                                    type::IsFunction<CallFunctionT &&>::value == false &&
                                            type::IsMemberFunction<CallFunctionT &&>::value == false>>
                    static ReturnType call(short *, CallFunctionT &&func, ArgTs &&... args) {
                        if (type::isPointerEmpty(func)) {
                            throw std::runtime_error("Null Pointer");
                        }
                        return std::forward<ReturnType>(func(std::forward<ArgTs &&>(args)...));
                    }

                private:
                    template<class ObjectT, class... MemArgTs>
                    static ReturnType
                    callMemberFunction(HelperFunctionT &&func, ObjectT &&object, MemArgTs &&... args) {
                        return std::forward<ReturnType>((object.*func)(std::forward<MemArgTs &&>(args)...));
                    }
                };

            private:
                DelegateFunction(FunctionT &&func) : m_function(std::forward<FunctionT &&>(func)) {
                }

                template<class FriendFunctionT>
                friend DelegateFunction<FriendFunctionT &&> makeDelegateFunction(FriendFunctionT &&func);

            public:
                template<class... ArgTs>
                typename Helper<FunctionT &&, ArgTs &&...>::ReturnType operator()(ArgTs &&... args) {
                    return std::forward<typename Helper<FunctionT &&, ArgTs &&...>::ReturnType>(
                            Helper<FunctionT &&, ArgTs &&...>::call(
                                    nullptr, std::forward<FunctionT &&>(m_function), std::forward<ArgTs &&>(args)...));
                }

                template<class... ArgTs>
                typename Helper<FunctionT &&, ArgTs &&...>::ReturnType operator()(ArgTs &&... args) const {
                    return std::forward<typename Helper<FunctionT &&, ArgTs &&...>::ReturnType>(
                            Helper<FunctionT &&, ArgTs &&...>::call(
                                    nullptr, std::forward<FunctionT &&>(m_function), std::forward<ArgTs &&>(args)...));
                }

                template<class... ArgTs>
                typename Helper<FunctionT &&, ArgTs &&...>::ReturnType call(ArgTs &&... args) {
                    return std::forward<typename Helper<FunctionT &&, ArgTs &&...>::ReturnType>(
                            Helper<FunctionT &&, ArgTs &&...>::call(
                                    nullptr, std::forward<FunctionT &&>(m_function), std::forward<ArgTs &&>(args)...));
                }

                template<class... ArgTs>
                typename Helper<FunctionT &&, ArgTs &&...>::ReturnType call(ArgTs &&... args) const {
                    return std::forward<typename Helper<FunctionT &&, ArgTs &&...>::ReturnType>(
                            Helper<FunctionT &&, ArgTs &&...>::call(
                                    nullptr, std::forward<FunctionT &&>(m_function), std::forward<ArgTs &&>(args)...));
                }

            private:
                FunctionT &&m_function;
            };

            template<class FunctionT>
            DelegateFunction<FunctionT &&> makeDelegateFunction(FunctionT &&func) {
                return DelegateFunction<FunctionT &&>(std::forward<FunctionT &&>(func));
            }

        }

        using detail::makeDelegateFunction;

    }
}
