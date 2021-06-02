#pragma once

#pragma once

#include "scoped.hpp"

namespace bbzy
{
namespace lock
{
namespace detail
{
template<typename T, typename LockScopedT>
class LockObject
{
public:
    LockObject(T& obj, LockScopedT&& lockScoped) :
            m_object(obj),
            m_lockScoped(std::move(lockScoped))
    {
    }

public:
    T* operator->()
    {
        return &m_object;
    }

    const T* operator->() const
    {
        return &m_object;
    }

    T& operator*()
    {
        return m_object;
    }

    const T& operator*() const
    {
        return m_object;
    }

private:
    T& m_object;
    LockScopedT m_lockScoped;
};

template<typename T>
class DirtyObject
{
public:
    T& dirty()
    {
        m_dirty = true;
        return m_object;
    }

    const T& clear() const
    {
        if (m_dirty)
        {
            m_stableObject = m_object;
            m_dirty = false;
        }
        return m_stableObject;
    }

private:
    T m_object{};
    mutable T m_stableObject{};
    mutable bool m_dirty = false;
};
}

template<typename T, typename LockableT>
class Wrapper
{
public:
    Wrapper() : m_value(new T)
    {
    }

    ~Wrapper()
    {
        auto&& lockScoped = makeLockScoped(&m_lockable);
        delete m_value;
    }

public:
    detail::LockObject<T, bbzy::lock::LockScoped<LockableT>> lock()
    {
        auto&& lockScoped = makeLockScoped(&m_lockable);
        return {*m_value, std::move(lockScoped)};
    }

private:
    mutable LockableT m_lockable{};
    T* m_value;
};

template<typename T, typename RWLockableT>
class RWWrapper
{
public:
    RWWrapper() : m_value(new T)
    {
    }

    ~RWWrapper()
    {
        auto&& lockWriteScoped = makeLockWriteScoped(&m_lockable);
        delete m_value;
    }

public:
    detail::LockObject<const T, bbzy::lock::LockReadScoped<RWLockableT>> lockRead() const
    {
        auto&& lockReadScoped = makeLockReadScoped(&m_lockable);
        return {*m_value, std::move(lockReadScoped)};
    }

    detail::LockObject<T, bbzy::lock::LockWriteScoped<RWLockableT>> lockWrite()
    {
        auto&& lockWriteScoped = makeLockWriteScoped(&m_lockable);
        return {*m_value, std::move(lockWriteScoped)};
    }

private:
    mutable RWLockableT m_lockable{};
    T* m_value;
};

template<typename T, typename RWLockableT>
class DirtyObject
{
public:
    DirtyObject() : m_object(new detail::DirtyObject<T>)
    {
    }

    ~DirtyObject()
    {
        auto&& scoped = LockWriteScoped<RWLockableT>(&m_lock);
        delete m_object;
    }

public:
    detail::LockObject<T, bbzy::lock::LockWriteScoped<RWLockableT>> lockWrite()
    {
        auto&& scoped = LockWriteScoped<RWLockableT>(&m_lock);
        return detail::LockObject<T, bbzy::lock::LockWriteScoped<RWLockableT>>(
                m_object->dirty(),
                std::move(scoped)
        );
    }

    const T* lockRead() const
    {
        auto&& scopedLock = LockReadScoped<RWLockableT>(&m_lock);
        return &m_object->clear();
    }

private:
    mutable RWLockableT m_lock;
    detail::DirtyObject<T>* m_object;
};

template<typename T, typename RWLockableT>
class DirtyObjectRef
{
public:
    explicit DirtyObjectRef(RWLockableT& lock) : m_lock(&lock), m_object(new detail::DirtyObject<T>)
    {}

    ~DirtyObjectRef()
    {
        auto&& scoped = LockWriteScoped<RWLockableT>(m_lock);
        delete m_object;
    }

public:
    detail::LockObject<T, bbzy::lock::LockWriteScoped<RWLockableT>> lockWrite()
    {
        auto&& scoped = LockWriteScoped<RWLockableT>(m_lock);
        return detail::LockObject<T, bbzy::lock::LockWriteScoped<RWLockableT>>(
                m_object->dirty(),
                std::move(scoped)
        );
    }

    const T* lockRead() const
    {
        auto&& scopedLock = LockReadScoped<RWLockableT>(m_lock);
        return &m_object->clear();
    }

private:
    mutable RWLockableT* m_lock;
    detail::DirtyObject<T>* m_object;
};
}
}