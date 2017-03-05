#pragma once
namespace bbzy {
namespace disam {
template <class RetT, class... FunArgTs>
inline auto getFunction(RetT(*f)(FunArgTs...))->
RetT(*)(FunArgTs...)
{
	return f;
}

}
}