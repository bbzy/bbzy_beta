#pragma once

#include "bbzy_beta/type.hpp"

namespace bbzy {
    namespace function {
        namespace detail {
            template<class ObjPtrT, class MethodT>
            class ThisBinder {
            public:
                ThisBinder(ObjPtrT &&objPtr, MethodT &&method) :
                        m_objectPointer(std::forward<ObjPtrT &&>(objPtr)),
                        m_method(std::forward<MethodT &&>(method)) {
                }

            public:
                template<class... ArgTs>
                type::ResType<MethodT> operator () (ArgTs&&... args) const {
                    return std::forward<type::ResType<MethodT>>(
                            (m_objectPointer->*m_method)(std::forward<ArgTs&&>(args)...)
                    );
                }

            private:
                ObjPtrT m_objectPointer;
                MethodT m_method;
            };
        }

        template<class ObjPtrT, class MethodT>
        detail::ThisBinder<ObjPtrT, MethodT> bindThis(ObjPtrT &&objPtr, MethodT &&method) {
            return {std::forward<ObjPtrT&&>(objPtr), std::forward<MethodT&&>(method)};
        }
    }
}
