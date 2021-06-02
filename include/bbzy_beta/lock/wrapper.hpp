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
    inline LockObject(T& obj, LockScopedT&& lockScoped) :
            m_object(obj),
            m_lockScoped(std::move(lockScoped))
    {}

public:
    inline T* operator->()
    {
        return &m_object;
    }

    inline const T* operator->() const
    {
        return &m_object;
    }

    inline T& operator*()
    {
        return m_object;
    }

    inline const T& operator*() const
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
    inline T& dirty()
    {
        m_dirty = true;
        return m_object;
    }

    inline const T& clear() const
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
    inline Wrapper() : m_value(new T)
    {}

    inline ~Wrapper()
    {
        auto&& lockScoped = makeLockScoped(&m_lockable);
        delete m_value;
    }

public:
    inline detail::LockObject<T, bbzy::lock::LockScoped<LockableT>> lock()
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
    inline RWWrapper() : m_value(new T)
    {}

    inline ~RWWrapper()
    {
        auto&& lockWriteScoped = makeLockWriteScoped(&m_lockable);
        delete m_value;
    }

public:
    inline detail::LockObject<const T, bbzy::lock::LockReadScoped<RWLockableT>> lockRead() const
    {
        auto&& lockReadScoped = makeLockReadScoped(&m_lockable);
        return {*m_value, std::move(lockReadScoped)};
    }

    inline detail::LockObject<T, bbzy::lock::LockWriteScoped<RWLockableT>> lockWrite()
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
    inline DirtyObject() : m_object(new detail::DirtyObject<T>)
    {}

    inline ~DirtyObject()
    {
        auto&& scoped = LockWriteScoped<RWLockableT>(&m_lock);
        delete m_object;
    }

public:
    inline detail::LockObject<T, bbzy::lock::LockWriteScoped<RWLockableT>> lockWrite()
    {
        auto&& scoped = LockWriteScoped<RWLockableT>(&m_lock);
        return detail::LockObject<T, bbzy::lock::LockWriteScoped<RWLockableT>>(
                m_object->dirty(),
                std::move(scoped)
        );
    }

    inline const T* lockRead() const
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
    inline explicit DirtyObjectRef(RWLockableT& lock) : m_lock(&lock), m_object(new detail::DirtyObject<T>)
    {}

    inline ~DirtyObjectRef()
    {
        auto&& scoped = LockWriteScoped<RWLockableT>(m_lock);
        delete m_object;
    }

public:
    inline detail::LockObject<T, bbzy::lock::LockWriteScoped<RWLockableT>> lockWrite()
    {
        auto&& scoped = LockWriteScoped<RWLockableT>(m_lock);
        return detail::LockObject<T, bbzy::lock::LockWriteScoped<RWLockableT>>(
                m_object->dirty(),
                std::move(scoped)
        );
    }

    inline const T* lockRead() const
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