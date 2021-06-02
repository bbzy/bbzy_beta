#pragma once

#include <utility>
#include <type_traits>
#include "../common.hpp"
#include "../type.hpp"
#include "../function.hpp"

namespace bbzy
{
namespace automatic
{
namespace detail
{
template<class ReleaseFunctionT>
class ReleaseFunctionProperty
{
public:
    explicit ReleaseFunctionProperty(ReleaseFunctionT releaseFunction) : m_releaseFunction(std::move(releaseFunction))
    {
    }

public:
    const ReleaseFunctionT& getReleaseFunction() const
    {
        return m_releaseFunction;
    }

protected:
    ReleaseFunctionT m_releaseFunction;
};

template<class ObjectT>
class ReleaseObjectProperty
{
public:
    explicit ReleaseObjectProperty(ObjectT* object) : m_object(object)
    {
    }

public:
    ObjectT* getObject() const
    {
        return m_object;
    }

protected:
    ObjectT* m_object;
};

template<class ObjectT, class ReleaseFunctionT>
class ReleaseObjectMemberFunction :
        public ReleaseObjectProperty<ObjectT>,
        public ReleaseFunctionProperty<ReleaseFunctionT>
{
public:
    ReleaseObjectMemberFunction(ObjectT* object, ReleaseFunctionT releaseFunction) :
            ReleaseObjectProperty<ObjectT>(object),
            ReleaseFunctionProperty<ReleaseFunctionT>(std::move(releaseFunction))
    {
    }

protected:
    void release()
    {
        release(nullptr);
    }

private:
    template<
            class InnerReleaseFunctionT = ReleaseFunctionT,
            class = bbzy::EnableIf<type::IsMemberFunction<InnerReleaseFunctionT>::value>
    >
    void release(void*)
    {
        (this->m_object->*this->m_releaseFunction)();
    }

    template<
            class InnerReleaseFunctionT = ReleaseFunctionT,
            class = bbzy::EnableIf<type::IsMemberFunction<InnerReleaseFunctionT>::value == false>,
            class = bbzy::EnableIf<std::is_same<type::GetFunPT<0, InnerReleaseFunctionT>, ObjectT*>::value>
    >
    void release(char*)
    {
        this->m_releaseFunction(this->m_object);
    }

    template<
            class InnerReleaseFunctionT = ReleaseFunctionT,
            class = bbzy::EnableIf<type::IsMemberFunction<InnerReleaseFunctionT>::value == false>,
            class= bbzy::EnableIf<std::is_same<type::GetFunPT<0, InnerReleaseFunctionT>, ObjectT&>::value>
    >
    void release(short*)
    {
        this->m_releaseFunction(*this->m_object);
    }
};

class CancellableProperty
{
public:
    void cancel()
    {
        m_isCancelled = true;
    }

    bool isCancelled() const
    {
        return m_isCancelled;
    }

    void reenable()
    {
        m_isCancelled = false;
    }

protected:
    bool m_isCancelled = false;
};

template<class ReleaseFunctionT>
class Releaser : public ReleaseFunctionProperty<ReleaseFunctionT>
{
public:
    using ReleaseFunctionProperty<ReleaseFunctionT>::ReleaseFunctionProperty;

public:
    ~Releaser()
    {
        this->m_releaseFunction();
    }
};

template<class ObjectT, class ReleaseFunctionT>
class ObjectReleaser : public ReleaseObjectMemberFunction<ObjectT, ReleaseFunctionT>
{
public:
    using ReleaseObjectMemberFunction<ObjectT, ReleaseFunctionT>::ReleaseObjectMemberFunction;

public:
    ~ObjectReleaser()
    {
        this->release();
    }
};

template<class ReleaseFunctionT>
class CancellableReleaser :
        public ReleaseFunctionProperty<ReleaseFunctionT>,
        public CancellableProperty
{
public:
    using ReleaseFunctionProperty<ReleaseFunctionT>::ReleaseFunctionProperty;

public:
    ~CancellableReleaser()
    {
        if (this->m_isCancelled == false)
        {
            this->m_releaseFunction();
        }
    }

};

template<class ObjectT, class ReleaseFunctionT>
class CancellableObjectReleaser :
        public ReleaseObjectMemberFunction<ObjectT, ReleaseFunctionT>,
        public CancellableProperty
{
public:
    using ReleaseObjectMemberFunction<ObjectT, ReleaseFunctionT>::ReleaseObjectMemberFunction;

public:
    ~CancellableObjectReleaser()
    {
        if (this->m_isCancelled == false)
        {
            this->release();
        }
    }
};

}

/**
 * Create a releaser which @releaseFunction would be called as destructing.
 * @param releaseFunction Function as void(*)().
 * @return Releaser
 * @remarks When object destructing, @releaseFunction will be called as releaseFunction().
 */
template<class ReleaseFunctionT>
detail::Releaser<ReleaseFunctionT> createReleaser(ReleaseFunctionT releaseFunction)
{
    return detail::Releaser<ReleaseFunctionT>(std::move(releaseFunction));
}

/**
 * Create a object releaser.
 * @param object The first argument of function @releaseFunction or this argument of menber function @releaseFunction.
 * @param releaseFunction Function as void(*)(const ObjectT*) or member function as void(ObjectT::*)().
 * @return ObjectReleaser
 * @remarks When object releaser destructing, @releaseFunction will be called as releaseFunction(object) or object->releaseFunction.
 */
template<class ObjectT, class ReleaseFunctionT>
detail::ObjectReleaser<ObjectT, ReleaseFunctionT> createReleaser(ObjectT* object, ReleaseFunctionT releaseFunction)
{
    return detail::ObjectReleaser<ObjectT, ReleaseFunctionT>(object, std::move(releaseFunction));
}

/**
 * Create a cancellable releaser.
 * @param releaseFunction Function as void(*)()
 * @return When object destructing, @releaseFunction will be called as releaseFunction() but member function cancel() can disable the release operation.
 */
template<class ReleaseFunctionT>
detail::CancellableReleaser<ReleaseFunctionT> createCancellableReleaser(ReleaseFunctionT releaseFunction)
{
    return detail::CancellableReleaser<ReleaseFunctionT>(std::move(releaseFunction));
}

/**
 * Create a cancellable object releaser
 * @param object The first argument of function @releaseFunction or this argument of menber function @releaseFunction.
 * @param releaseFunction Function as void(*)(const ObjectT*) or member function as void(ObjectT::*)().
 * @return When object releaser destructing, @releaseFunction will be called as releaseFunction(object) or object->releaseFunction but member function cancel() can disable the release operation.
 */
template<class ObjectT, class ReleaseFunctionT>
detail::CancellableObjectReleaser<ObjectT, ReleaseFunctionT> createCancellableReleaser(ObjectT* object, ReleaseFunctionT releaseFunction)
{
    return detail::CancellableObjectReleaser<ObjectT, ReleaseFunctionT>(object, std::move(releaseFunction));
}
}
}
