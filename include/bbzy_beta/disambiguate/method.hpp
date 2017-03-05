#pragma once

namespace bbzy {
namespace disam {
namespace detail {
template <class ReturnT, class... MethodParamTs>
struct Method
{
	template <class ClassT>
	static auto get(ReturnT(ClassT::*method)(MethodParamTs...)) ->
		ReturnT(ClassT::*)(MethodParamTs...)
	{
		return method;
	}

	template <class ClassT>
	static auto getConst(ReturnT(ClassT::*method)(MethodParamTs...)const) ->
		ReturnT(ClassT::*)(MethodParamTs...)const
	{
		return method;
	}
};
}

using detail::Method;

}
}
