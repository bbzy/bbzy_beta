#pragma once
#include <utility>
namespace bbzy
{
template <class ReleaseFunctionT>
class _ReleasreBase
{
protected:
    _ReleasreBase() = default;

public:
    ReleaseFunctionT& getReleaseFunction()
    {
        return m_releaseFunction;
    }

    const ReleaseFunctionT& getReleaseFunction() const
    {
        return m_releaseFunction;
    }

protected:
    ReleaseFunctionT* detach()
    {
        return m_releaseFunction.detach();
    }

    void setReleaseFunction(ReleaseFunctionT releaseFunction)
    {
        m_releaseFunction.reset(new ReleaseFunctionT(std::move(releaseFunction)));
    }

    void resetReleaseFunction()
    {
        m_releaseFunction.reset();
    }

    void copyReleaseFunctionFrom(const _ReleasreBase& other)
    {
        if (other.m_releaseFunction)
        {
            m_releaseFunction.reset(new ReleaseFunctionT(*other.m_releaseFunction));
        }
        else
        {
            m_releaseFunction.reset();
        }
    }



private:
    _ReleasreBase(const _ReleasreBase&) = delete;
    _ReleasreBase& operator = (const _ReleasreBase&) = delete;
};

template <class ReleaseFunctionT>
class AutoReleaser
{
public:
    AutoReleaser() = default;

    AutoReleaser(ReleaseFunctionT&& releaseFunction)
    {
        m_releaseFunction = std::forward<ReleaseFunctionT&&>(releaseFunction);

    }

    AutoReleaser(AutoReleaser& autoReleaser)
    {
        copyReleaseFunctionFrom(autoReleaser);
    }

    AutoReleaser(AutoReleaser&& autoReleaser)
    {
        _ReleaserBase<ReleaseFunctionT>::copyReleaseFunctionFrom(autoReleaser);
    }

    ~AutoReleaser()
    {
        release();
    }

public:
    AutoReleaser& operator =(ReleaseFunctionT releaseFunction)
    {
        release();
        setReleaseFunction(std::move(releaseFunction));
        return *this;
    }

    AutoReleaser& operator =(AutoReleaser& autoReleaser)
    {
        release();
        copyReleaseFunctionFrom(autoReleaser);
        return *this;
    }

    AutoReleaser& operator =(AutoReleaser&& autoReleaser)
    {
        release();
        copyReleaseFunctionFrom(autoReleaser);
        return *this;
    }

public:
    /**
    Call release function and reset the function pointer.
    */
    void release()
    {
        if (_ReleaserBase<ReleaseFunctionT>::getReleaseFunction())
        {
            (*_ReleaserBase<ReleaseFunctionT>::getReleaseFunction())();
            _ReleaserBase<ReleaseFunctionT>::resetReleaseFunction();
        }
    }

    void detach()
    {
        _ReleaserBase<ReleaseFunctionT>::detach();
    }

private:
    ReleaseFunctionT m_releaseFunction = {};
    bool m_needRelease = false;
};


}
