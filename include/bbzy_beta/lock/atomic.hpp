#pragma once

#include "scoped.hpp"

namespace bbzy
{
namespace lock
{
namespace detail
{
template<
        typename T,
        typename LockableT,
        typename LockReadScopedT,
        LockReadScopedT(* makeReadScoped)(LockableT* lock),
        typename LockWriteScopedT,
        LockWriteScopedT(* makeWriteScoped)(LockableT* lock)
>
class Atomic
{
public:
    Atomic() : m_value(new T)
    {
    }

    ~Atomic()
    {
        auto&& lockWriteScoped = makeWriteScoped(&m_lockable);
        delete m_value;
    }

public:
    T load() const
    {
        auto&& lockReadScoped = makeReadScoped(&m_lockable);
        return *m_value;
    }

    void store(T other)
    {
        auto&& lockWriteScoped = makeWriteScoped(&m_lockable);
        *m_value = std::move(other);
    }

private:
    mutable LockableT m_lockable{};
    T* m_value;
};
}

template<typename T, typename LockableT>
using Atomic = detail::Atomic<
        T, LockableT,
        LockScoped<LockableT>,
        &makeLockScoped<LockableT>,
        LockScoped<LockableT>,
        &makeLockScoped<LockableT>
>;

template<typename T, typename LockableT>
using RWAtomic = detail::Atomic<
        T, LockableT,
        LockReadScoped<LockableT>,
        &makeLockReadScoped,
        LockWriteScoped<LockableT>,
        &makeLockWriteScoped
>;
}
}