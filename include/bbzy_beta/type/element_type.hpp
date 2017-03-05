#pragma once
#include "../common.hpp"
#include "copy_reference.hpp"

namespace bbzy {
namespace type {
namespace detail {
template <class T>
struct ElementType
{
private:
	template <class ContainerT>
	struct Helper
	{
		typedef typename ContainerT::value_type type;
	};

	template <class PtrT>
	struct Helper<PtrT*>
	{
		typedef PtrT type;
	};

public:
	typedef typename Helper<Decay<T>>::type type;
};

}

template <class T>
using ElementType = typename detail::ElementType<T>::type;

template <class T>
using ElemT = ElementType<T>;

template <class T>
using ElementReferenceType = CopyProp<ElemT<T>, T>;

template <class T>
using ElemRT = ElementReferenceType<T>;

}
}
