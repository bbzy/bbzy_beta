#pragma once

#include <iostream>
#include <vector>
#include "bbzy_beta/lock.hpp"

enum class LockType {
    Lock,
    Unlock,
    LockRead,
    UnlockRead,
    LockWrite,
    UnlockWrite,
};

std::vector<LockType> g_lockTypeSeq;

class DummyLock {
public:
    void lock() {
        g_lockTypeSeq.emplace_back(LockType::Lock);
    }

    void unlock() {
        g_lockTypeSeq.emplace_back(LockType::Unlock);
    }

    void lockRead() {
        g_lockTypeSeq.emplace_back(LockType::LockRead);
    }

    void unlockRead() {
        g_lockTypeSeq.emplace_back(LockType::UnlockRead);
    }

    void lockWrite() {
        g_lockTypeSeq.emplace_back(LockType::LockWrite);
    }

    void unlockWrite() {
        g_lockTypeSeq.emplace_back(LockType::UnlockWrite);
    }

    static bool check(const std::vector<LockType> &otherLockTypeSeq) {
        bool res = g_lockTypeSeq == otherLockTypeSeq;
        g_lockTypeSeq.clear();
        return res;
    }
};

template<class = void>
void test_scoped() {
    DummyLock lock;
    {
        auto &&scoped = bbzy::lock::makeLockScoped(&lock);
        auto &&scopedCopy = std::move(scoped);
    }
    assert(lock.check({LockType::Lock, LockType::Unlock}));

    {
        auto &&scoped = bbzy::lock::makeLockReadScoped(&lock);
    }
    assert(lock.check({LockType::LockRead, LockType::UnlockRead}));

    {
        auto &&scoped = bbzy::lock::makeLockWriteScoped(&lock);
    }
    assert(lock.check({LockType::LockWrite, LockType::UnlockWrite}));
}


template<class = void>
void test_atomic() {
    {
        bbzy::lock::Atomic<std::pair<int, int>, DummyLock> v;
        v.store({1, 3});
        assert(v.load() == std::make_pair(1, 3));
    }
    assert(DummyLock::check({
            LockType::Lock, LockType::Unlock,  // For storing
            LockType::Lock, LockType::Unlock,  // For loading
            LockType::Lock, LockType::Unlock,  // For destructing
    }));
    {
        bbzy::lock::RWAtomic<std::pair<int, int>, DummyLock> v;
        v.store({1, 3});
        assert(v.load() == std::make_pair(1, 3));
    }
    assert(DummyLock::check({
            LockType::LockWrite, LockType::UnlockWrite,  // For storing
            LockType::LockRead, LockType::UnlockRead,  // For loading
            LockType::LockWrite, LockType::UnlockWrite,  // For destructing
    }));
}

template<class = void>
void test_wrapper() {
    {
        bbzy::lock::Wrapper<std::string, DummyLock> v;
        {
            auto &&u = v.lock();
            (*u) = "3";
            u->push_back('4');
        }
        assert((*v.lock()) == "34");
    }
    assert(DummyLock::check({
            LockType::Lock, LockType::Unlock,  // For writing
            LockType::Lock, LockType::Unlock,  // For reading
            LockType::Lock, LockType::Unlock,  // For destructing
    }));

    {
        bbzy::lock::RWWrapper<std::string, DummyLock> v;
        {
            auto &&u = v.lockWrite();
            (*u) = "3";
            u->push_back('4');
        }
        {
            auto &&u = v.lockRead();
            assert((*u) == "34");
        }
    }
    assert(DummyLock::check({
            LockType::LockWrite, LockType::UnlockWrite,  // For writing
            LockType::LockRead, LockType::UnlockRead,  // For reading
            LockType::LockWrite, LockType::UnlockWrite,  // For destructing
    }));

    {
        bbzy::lock::RWWrapper<std::unique_ptr<int>, DummyLock> obj;
    }
    g_lockTypeSeq.clear();

    {
        bbzy::lock::DirtyObject<std::string, DummyLock> obj;
        (*obj.lockWrite()) = "2333";
        const std::string *p = nullptr;
        {
            auto &&x = obj.lockRead();
            assert(*x == "2333");
            p = x;
        }
        assert(p == obj.lockRead());
    }
    assert(DummyLock::check({
            LockType::LockWrite, LockType::UnlockWrite,  // For writing
            LockType::LockRead, LockType::UnlockRead,  // For reading
            LockType::LockRead, LockType::UnlockRead,  // For assert reading
            LockType::LockWrite, LockType::UnlockWrite,  // For destructing
    }));

    {
        DummyLock lock;
        bbzy::lock::DirtyObjectRef<std::string, DummyLock> obj(lock);
        (*obj.lockWrite()) = "2333";
        const std::string *p = nullptr;
        {
            auto &&x = obj.lockRead();
            assert(*x == "2333");
            p = x;
        }
        assert(p == obj.lockRead());
    }
    assert(DummyLock::check({
            LockType::LockWrite, LockType::UnlockWrite,  // For writing
            LockType::LockRead, LockType::UnlockRead,  // For reading
            LockType::LockRead, LockType::UnlockRead,  // For assert reading
            LockType::LockWrite, LockType::UnlockWrite,  // For destructing
    }));
}

template<class = void>
void test_lock() {
    test_scoped();
    test_atomic();
    test_wrapper();
    std::cout << "Test Lock OK" << std::endl;
}