#pragma once

namespace bbzy {
namespace disam {
template <class RetT, class ClassT, class... ParamTs>
inline auto getMethod(RetT(ClassT::*m)(ParamTs...)) ->
RetT(ClassT::*)(ParamTs...)
{
	return m;
}

template <class RetT, class ClassT, class... ParamTs>
inline auto getCMethod(RetT(ClassT::*cm)(ParamTs...) const) ->
RetT(ClassT::*)(ParamTs...) const
{
	return cm;
}

}
}
