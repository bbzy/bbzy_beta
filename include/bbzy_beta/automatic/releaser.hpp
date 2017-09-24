#pragma once
#include <utility>
#include <type_traits>
#include "../common.hpp"
#include "../type.hpp"
namespace bbzy {
namespace automatic {
namespace detail {
template <class ReleaseFunctionT>
class ReleaseFunctionProperty
{
public:
	ReleaseFunctionProperty(ReleaseFunctionT&& releaseFunction) :
		m_releaseFunction(std::forward<ReleaseFunctionT&&>(releaseFunction))
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

template <class ObjectT>
class ReleaseObjectProperty
{
public:
	ReleaseObjectProperty(ObjectT* object) : m_object(object)
	{}

public:
	ObjectT* getObject() const
	{
		return m_object;
	}

protected:
	ObjectT* m_object;
};

template <class ObjectT, class ReleaseFunctionT>
class ReleaseObjectMemberFunction :
	public ReleaseObjectProperty<ObjectT>,
	public ReleaseFunctionProperty<ReleaseFunctionT>
{
public:
	ReleaseObjectMemberFunction(ObjectT* object, ReleaseFunctionT&& releaseFunction) :
		ReleaseObjectProperty<ObjectT>(object),
		ReleaseFunctionProperty<ReleaseFunctionT>(std::forward<ReleaseFunctionT&&>(releaseFunction))
	{
	}

protected:
	void release()
	{
		release(nullptr);
	}

private:
	template <class InnerReleaseFunctionT = ReleaseFunctionT,
		class = EnableIf<type::IsMemberFunction<InnerReleaseFunctionT>::value>>
	void release(void*)
	{
		(this->m_object->*this->m_releaseFunction)();
	}

	template <class InnerReleaseFunctionT = ReleaseFunctionT,
		class = EnableIf<type::IsMemberFunction<InnerReleaseFunctionT>::value == false>,
		class = EnableIf<std::is_same<type::GetFunPT<0, InnerReleaseFunctionT>, ObjectT*>::value>
	>
	void release(char*)
	{
		this->m_releaseFunction(this->m_object);
	}

	template <class InnerReleaseFunctionT = ReleaseFunctionT,
		class = EnableIf<type::IsMemberFunction<InnerReleaseFunctionT>::value == false>,
		class = EnableIf<std::is_same<type::GetFunPT<0, InnerReleaseFunctionT>, ObjectT&>::value>
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

template <class ReleaseFunctionT>
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

template <class ObjectT, class ReleaseFunctionT>
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

template <class ReleaseFunctionT>
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

template <class ObjectT, class ReleaseFunctionT>
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

template <class ReleaseFunctionT>
inline detail::Releaser<ReleaseFunctionT> createReleaser(ReleaseFunctionT&& releaseFunction)
{
	return detail::Releaser<ReleaseFunctionT>(
		std::forward<ReleaseFunctionT&&>(releaseFunction));
}

template <class ObjectT, class ReleaseFunctionT>
inline detail::ObjectReleaser<ObjectT, ReleaseFunctionT> createReleaser(
	ObjectT* object, ReleaseFunctionT&& releaseFunction)
{
	return detail::ObjectReleaser<ObjectT, ReleaseFunctionT>(
		object, std::forward<ReleaseFunctionT&&>(releaseFunction));
}

template <class ReleaseFunctionT>
inline detail::CancellableReleaser<ReleaseFunctionT> createCancellableReleaser(ReleaseFunctionT&& releaseFunction)
{
	return detail::CancellableReleaser<ReleaseFunctionT>(
		std::forward<ReleaseFunctionT&&>(releaseFunction));
}

template <class ObjectT, class ReleaseFunctionT>
inline detail::CancellableObjectReleaser<ObjectT, ReleaseFunctionT> createCancellableReleaser(
	ObjectT* object, ReleaseFunctionT&& releaseFunction)
{
	return detail::CancellableObjectReleaser<ObjectT, ReleaseFunctionT>(
		object, std::forward<ReleaseFunctionT&&>(releaseFunction));
}
}
}
