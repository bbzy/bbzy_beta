#pragma once
namespace bbzy
{
namespace lock
{
namespace detail
{
template<typename LockableT, void(LockableT::*lockMethod)(), void(LockableT::*unloackMethod)()>
class LockScoped
{
public:
    inline explicit LockScoped(LockableT* lock) : m_lock(lock)
    {
        (m_lock->*lockMethod)();
    };

    inline LockScoped(LockScoped&& other) : m_lock(nullptr)
    {
        std::swap(m_lock, other.m_lock);
    }

    inline ~LockScoped()
    {
        if (m_lock)
        {
            (m_lock->*unloackMethod)();
        }
    }

private:
    LockableT* m_lock;
};
}

template<typename LockableT>
using LockScoped = detail::LockScoped<LockableT, &LockableT::lock, &LockableT::unlock>;

template<typename RWLockableT>
using LockReadScoped = detail::LockScoped<RWLockableT, &RWLockableT::lockRead, &RWLockableT::unlockRead>;

template<typename RWLockableT>
using LockWriteScoped = detail::LockScoped<RWLockableT, &RWLockableT::lockWrite, &RWLockableT::unlockWrite>;

template<typename LockableT>
inline LockScoped<LockableT> makeLockScoped(LockableT* lock)
{
    return LockScoped<LockableT>(lock);
}

template<typename LockableT>
inline LockReadScoped<LockableT> makeLockReadScoped(LockableT* lock)
{
    return LockReadScoped<LockableT>(lock);
}

template<typename LockableT>
inline LockWriteScoped<LockableT> makeLockWriteScoped(LockableT* lock)
{
    return LockWriteScoped<LockableT>(lock);
}
}
}