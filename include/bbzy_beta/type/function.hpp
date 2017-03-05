#pragma once
#include <type_traits>
#include "../common.hpp"
#include "index_type.hpp"
#include "element_type.hpp"
#include "type_wrapper.hpp"

namespace bbzy {
namespace type {
namespace detail {
template <class FunctionT>
struct UnifiedFunctionType
{
private:
	template <class HelperFunT>
	struct Helper;

	template <class RetT, class... ParamTs>
	struct Helper<RetT(*)(ParamTs...)>
	{
		typedef RetT(*type)(ParamTs...);
	};

	template <class RetT, class ClassT, class... ParamTs>
	struct Helper<RetT(ClassT::*)(ParamTs...)>
	{
		typedef RetT(*type)(ClassT*, ParamTs...);
	};

	template <class RetT, class ClassT, class... ParamTs>
	struct Helper<RetT(ClassT::*)(ParamTs...)const>
	{
		typedef RetT(*type)(const ClassT*, ParamTs...);
	};

public:
	typedef typename Helper<Decay<FunctionT>>::type type;
};

//template <class FunctionPointerT>
//struct FunctionUnwrapper
//{
//private:
//
//	typedef UnifiedFunctionType<FunctionPointerT> UtifiedFunT;
//
//	template <class HelperFunT>
//	struct Helper
//	{
//		using return_type = typename HelperFunT::result_type;
//		using class_type = void;
//	};
//
//	//template <class RetT, class... ParamTs>
//	//struct Helper<RetT(ParamTs...)>
//	//{
//	//	using return_type = RetT;
//	//	using class_type = void;
//
//	//	using param_types = TypeWrapper<ParamTs...>;
//
//	//	template <size_t index>
//	//	using param_type = GetIndexType<index, ParamTs...>;
//	//	enum { param_count = sizeof...(ParamTs) };
//	//};
//
//	//template <class RetT, class ClassT, class... ParamTs>
//	//struct Helper<RetT(ClassT::*)(ParamTs...)>
//	//{
//	//	using return_type = RetT;
//	//	using class_type = ClassT;
//
//	//	using param_types = TypeWrapper<ParamTs...>;
//
//	//	template <size_t index>
//	//	using param_type = GetIndexType<index, ParamTs...>;
//
//	//	enum { param_count = sizeof...(ParamTs) };
//	//};
//
//	//template <class RetT, class ClassT, class... ParamTs>
//	//struct Helper<RetT(ClassT::*)(ParamTs...)const>
//	//{
//	//	using return_type = RetT;
//	//	using class_type = ClassT;
//
//	//	using param_types = TypeWrapper<ParamTs...>;
//
//	//	template <size_t index>
//	//	using param_type = GetIndexType<index, ParamTs...>;
//	//	enum { param_count = sizeof...(ParamTs) };
//	//};
//
//public:
//	typedef Helper<UtifyFunT> fun_type;
//
//	typedef typename Helper<UtifyFunT>::return_type return_type;
//	typedef typename Helper<UtifyFunT>::class_type class_type;
//
//	enum { param_count = Helper<UtifyFunT>::param_count };
//
//	using param_types = typename Helper<UtifyFunT>::param_types;
//
//	template <size_t index>
//	using param_type = typename Helper<UtifyFunT>::template param_type<index>;
//};

template <class FunctionT>
struct GetFunctionParamWrapper
{
private:
	template <class HelperFunctionT>
	struct Helper;

	template <class RetT, class... ParamTs>
	struct Helper<RetT(*)(ParamTs...)>
	{
		using type = TypeWrapper<ParamTs...>;
	};

public:
	using type = typename Helper<typename UnifiedFunctionType<FunctionT>::type>::type;
};

template <size_t index, class FunctionT>
struct GetFunctionParamType
{
private:
	template <class HelperFunctionT>
	struct Helper;

	template <class RetT, class... ParamTs>
	struct Helper<RetT(*)(ParamTs...)>
	{
		using type = typename TypeAt<index, ParamTs...>::type;
	};

public:
	using type = typename Helper<typename UnifiedFunctionType<FunctionT>::type>::type;
};

template <class MethodT>
struct GetMethodClassType
{
	using type = EnableIf<std::is_member_function_pointer<MethodT>::value, 
		ElemT<typename GetFunctionParamType<0, MethodT>::type>>;
};

}

template <class FunctionT>
using UnifiedFunctionType = typename detail::UnifiedFunctionType<FunctionT>::type;

template <class FunctionT>
using UniFunT = UnifiedFunctionType<FunctionT>;

template <class FunctionT>
using GetFunctionParamWrapper = typename detail::GetFunctionParamWrapper<FunctionT>::type;

template <class FunctionT>
using GetFunPW = GetFunctionParamWrapper<FunctionT>;

template <size_t index, class FunctionT>
using GetFunctionParamType = typename detail::GetFunctionParamType<index, FunctionT>::type;

template <size_t index, class FunctionT>
using GetFunPT = GetFunctionParamType<index, FunctionT>;

template <class MethodT>
using GetMethodClassType = typename detail::GetMethodClassType<MethodT>::type;

}
}
