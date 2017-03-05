#pragma once
namespace bbzy {
namespace type {
template <class... Ts>
struct TypeWrapper
{
	enum
	{
		size = sizeof...(Ts)
	};
};
}
}