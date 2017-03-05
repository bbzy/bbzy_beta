#pragma once
#include <stdexcept>
#include <type_traits>
#include "../common.hpp"
#include "../type.hpp"
namespace bbzy {
namespace unify {
namespace detail {

template <class FunctionT>
class DelegateFunction
{
private:
	template <class HelperFunctionT, class... ArgTs>
	struct Helper
	{
	public:
		typedef type::ResultType<HelperFunctionT, ArgTs...> ReturnType;

	public:
		template <class = EnableIf<std::is_function<HelperFunctionT>::value>>
		static ReturnType call(void*, const HelperFunctionT& func, ArgTs&&... args)
		{
			if (func == nullptr)
			{
				throw std::runtime_error("Null Pointer");
			}
			return std::forward<ReturnType>(func(std::forward<ArgTs&&>(args)...));
		}

		template <class = EnableIf<std::is_member_function_pointer<HelperFunctionT>::value>>
		static ReturnType call(char*, const HelperFunctionT& func, ArgTs&&... args)
		{
			return std::forward<ReturnType>(callMethod(func, std::forward<ArgTs&&>(args)...));
		}

		template <class = EnableIf<
			std::is_function<HelperFunctionT>::value == false &&
			std::is_member_function_pointer<HelperFunctionT>::value == false>>
		static ReturnType call(short*, const HelperFunctionT& func, ArgTs&&... args)
		{
			return std::forward<ReturnType>(func(std::forward<ArgTs&&>(args)...));
		}

	private:
		template <class ObjectT, class... MemArgTs>
		static ReturnType callMethod(const HelperFunctionT& func, ObjectT&& object, MemArgTs&&... args)
		{
			return std::forward<ReturnType>((object.*func)(std::forward<MemArgTs&&>(args)...));
		}
	};

private:
	DelegateFunction(FunctionT&& func) : m_function(std::forward<FunctionT&&>(func))
	{
	}

	template <class FunctionT>
	friend DelegateFunction<FunctionT> makeDelegateFunction(FunctionT&& func);

public:
	template <class... ArgTs>
	typename Helper<FunctionT, ArgTs&&...>::ReturnType operator() (ArgTs&&... args) const
	{
		return std::forward<typename Helper<FunctionT, ArgTs&&...>::ReturnType>(
			Helper<FunctionT, ArgTs...>::call(nullptr, m_function, std::forward<ArgTs&&>(args)...));
	}

	template <class... ArgTs>
	typename Helper<FunctionT, ArgTs&&...>::ReturnType call(ArgTs&&... args) const
	{
		return std::forward<typename Helper<FunctionT, ArgTs&&...>::ReturnType>(
			Helper<FunctionT, ArgTs...>::call(nullptr, m_function, std::forward<ArgTs&&>(args)...));
	}

private:
	FunctionT m_function;
};

template <class FunctionT>
DelegateFunction<FunctionT> makeDelegateFunction(FunctionT&& func)
{
	return DelegateFunction<FunctionT>(std::forward<FunctionT>(func));
}

}

using detail::makeDelegateFunction;

}
}
