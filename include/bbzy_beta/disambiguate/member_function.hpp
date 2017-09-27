#pragma once

namespace bbzy {
namespace disam {
namespace detail {
template <class ReturnT, class... ParamTs>
struct MemberFunction
{
	template <class ClassT>
	static auto get(ReturnT(ClassT::*memberFunction)(ParamTs...)) ->
		ReturnT(ClassT::*)(ParamTs...)
	{
		return memberFunction;
	}

	template <class ClassT>
	static auto getConst(ReturnT(ClassT::*memberFunction)(ParamTs...)const) ->
		ReturnT(ClassT::*)(ParamTs...)const
	{
		return memberFunction;
	}
};
}

using detail::MemberFunction;

}
}
