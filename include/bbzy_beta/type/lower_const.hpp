#pragma once
#include <type_traits>
#include "../common.hpp"
namespace bbzy {
namespace type {
namespace detail {
template <class T>
struct IsLowerConst
{
private:
	template <class HelperT>
	struct Helper
	{
		enum {value = std::is_const<HelperT>::value};
	};
	
	template <class HelperT>
	struct Helper<HelperT&>
	{
		enum {value = Helper<HelperT>::value};
	};

	template <class HelperT>
	struct Helper<HelperT&&>
	{
		enum {value = Helper<HelperT>::value};
	};

	template <class HelperT>
	struct Helper<HelperT*>
	{
		enum {value = Helper<HelperT>::value};
	};

	template <class HelperT>
	struct Helper<HelperT*const>
	{
		enum { value = Helper<HelperT>::value };
	};

public:
	enum {value = std::is_pointer<T>::value || std::is_reference<T>::value ? Helper<T>::value : 0};
};

template <class T>
struct AddLowerConst
{
private:
	template <class HelperT>
	struct Helper
	{
		using type = AddConst<HelperT>;
	};

	template <class HelperT>
	struct Helper<HelperT&>
	{
		using type = typename Helper<HelperT>::type&;
	};

	template <class HelperT>
	struct Helper<HelperT&&>
	{
		using type = typename Helper<HelperT>::type&&;
	};

	template <class HelperT>
	struct Helper<HelperT*>
	{
		using type = typename Helper<HelperT>::type*;
	};

	template <class HelperT>
	struct Helper<HelperT*const>
	{
		using type = typename Helper<HelperT>::type*const;
	};

public:
	using type = Conditional<std::is_pointer<T>::value || std::is_reference<T>::value,
		typename Helper<T>::type, T>;
};

template <class T>
struct RemoveLowerConst
{
private:
	template <class HelperT>
	struct Helper
	{
		using type = RemoveConst<HelperT>;
	};

	template <class HelperT>
	struct Helper<HelperT&>
	{
		using type = typename Helper<HelperT>::type&;
	};

	template <class HelperT>
	struct Helper<HelperT&&>
	{
		using type = typename Helper<HelperT>::type&&;
	};

	template <class HelperT>
	struct Helper<HelperT*>
	{
		using type = typename Helper<HelperT>::type*;
	};

	template <class HelperT>
	struct Helper<HelperT*const>
	{
		using type = typename Helper<HelperT>::type*const;
	};

public:
	using type =  Conditional<std::is_pointer<T>::value || std::is_reference<T>::value,
		typename Helper<T>::type, T>;
};

template <class TargetT, class SourceT>
struct CopyLowerConst
{
	using type = Conditional<IsLowerConst<SourceT>::value, typename AddLowerConst<TargetT>::type, TargetT>;
};

}

using detail::IsLowerConst;

template <class T>
using IsLC = IsLowerConst<T>;

template <class T>
using AddLowerConst = typename detail::AddLowerConst<T>::type;

template <class T>
using AddLC = AddLowerConst<T>;

template <class T>
using RemoveLowerConst = typename detail::RemoveLowerConst<T>::type;

template <class T>
using RmLC = RemoveLowerConst<T>;

template <class TargetT, class SourceT>
using CopyLowerConst = typename detail::CopyLowerConst<TargetT, SourceT>::type;

template <class TargetT, class SourceT>
using CpLC = CopyLowerConst<TargetT, SourceT>;

}
}
