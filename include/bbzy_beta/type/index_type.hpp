#pragma once
namespace bbzy {
namespace type {
namespace detail {
template <size_t index, class... Ts>
struct TypeAt
{
private:
	template <size_t helperIndex, class... HelperTs>
	struct Helper;

	template <size_t helperIndex, class T, class... HelperTs>
	struct Helper<helperIndex, T, HelperTs...>
	{
		typedef typename Helper<helperIndex - 1, HelperTs...>::type type;
	};

	template <class T, class... HelperTs>
	struct Helper<0, T, HelperTs...>
	{
		typedef T type;
	};

	template <class T>
	struct Helper<0, T>
	{
		typedef T type;
	};

public:
	typedef typename Helper<index, Ts...>::type type;
};

template <size_t index, class T>
struct TemplateTypeAt
{
private:
	template <class HelperT>
	struct Helper;

	template <template<class...> class HelperT, class... TemplateParamTs>
	struct Helper<HelperT<TemplateParamTs...>>
	{
		using type = typename TypeAt<index, TemplateParamTs...>::type;
	};

public:
	using type = typename Helper<T>::type;
};

}

template <size_t index, class... Ts>
using TypeAt = typename detail::TypeAt<index, Ts...>::type;

template <size_t index, class T>
using TemplateTypeAt = typename detail::TemplateTypeAt<index, T>::type;

template <size_t index, class T>
using TTypeAt = TemplateTypeAt<index, T>;

}
}