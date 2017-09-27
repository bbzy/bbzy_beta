#pragma once
#include <stdint.h>
#include "../common.hpp"
#include "copy_reference.hpp"
#include "type_exist.hpp"
namespace bbzy {
namespace type {
namespace detail {
template <class T, template <class...> class TemplateClassT>
struct IsTemplateOf
{
private:
	template <class HelperT>
	static void help(HelperT);

	template <template <class...> class HelperT, class... TemplateParamTs,
		class = EnableIf<std::is_same<HelperT<TemplateParamTs...>, TemplateClassT<TemplateParamTs...>>::value>>
	static int help(HelperT<TemplateParamTs...>);
public:
	enum
	{
		value = std::is_same<decltype(help(std::declval<Decay<T>>())), int>::value
	};
};

template <class T1, class T2>
struct AreSameTemplate
{
private:
	template <template <class...> class TemplateT,
		class... TemplateParams1,
		class... TemplateParams2>
	static int16_t help(TemplateT<TemplateParams1...>, TemplateT<TemplateParams2...>);

	template <class HelperT1, class HelperT2>
	static int8_t help(HelperT1, HelperT2);

public:
	enum 
	{ 
		value = sizeof(decltype(help(std::declval<Decay<T1>>(), std::declval<Decay<T2>>()))) - 1 
	};
};

template <class T, template <class...> class... TemplateTs>
struct IsAnyOfTemplates
{
private:
	template <class HelperT, template <class...> class... HelperTemplateTs>
	struct Helper;

	template <class HelperT,
		template <class...> class HelperTemplateT,
		template <class...> class... LeftTemplateTs>
	struct Helper<HelperT, HelperTemplateT, LeftTemplateTs...>
	{
		typedef Conditional<IsTemplateOf<HelperT, HelperTemplateT>::value,
			int, typename Helper<HelperT, LeftTemplateTs...>::type> type;
	};

	template <class HelperT>
	struct Helper<HelperT>
	{
		typedef void type;
	};

public:
	enum
	{
		value = std::is_same<typename Helper<T, TemplateTs...>::type, int>::value
	};
};

template <class T, template <class...> class NewTemplateT>
struct ChangeTemplate
{
private:
	template <class HelperT, template <class...> class HelperNewTemplateT>
	struct Helper;

	template <
		template <class...> class OriginalTemplateT,
		template <class...> class TargetTemplateT,
		class... TemplateParamTs>
	struct Helper<OriginalTemplateT<TemplateParamTs...>, TargetTemplateT>
	{
		using type = TargetTemplateT<TemplateParamTs...>;
	};

public:
	using type = CopyProp<typename Helper<Decay<T>, NewTemplateT>::type, T>;
};

template <class T, class... NewElementTs>
struct ChangeElementTypes
{
private:
	template <class HelperT>
	struct Helper;

	template <template <class...> class HelperT, class... TemplateParamTs>
	struct Helper<HelperT<TemplateParamTs...>>
	{
		using type = HelperT<NewElementTs...>;
	};

public:
	using type = CpProp<typename Helper<Decay<T>>::type, T>;
};

}

using detail::IsTemplateOf;

template <class T, template <class...> class TemplateClassT>
using TempOf = IsTemplateOf<T, TemplateClassT>;

using detail::AreSameTemplate;

template <class T1, class T2>
using SameTemp = AreSameTemplate<T1, T2>;

template <class T, template <class...> class... TemplateTs>
using IsAnyOfTemplates = detail::IsAnyOfTemplates<T, TemplateTs...>;

template <class T, template <class...> class... TemplateTs>
using AnyOfTs = IsAnyOfTemplates<T, TemplateTs...>;

template <class T, template <class...> class... TemplateTs>
using EnableTemplateOf = EnableIf<AnyOfTs<T, TemplateTs...>::value>;

template <class T, template <class...> class... TemplateTs>
using EnableTempOf = EnableTemplateOf<T, TemplateTs...>;

template <class T, template <class...> class NewTemplateT>
using ChangeTemplate = typename detail::ChangeTemplate<T, NewTemplateT>::type;

template <class T, template <class...> class NewTemplateT>
using ChTemp = ChangeTemplate<T, NewTemplateT>;

template <class T, class... NewElementTs>
using ChangeElementTypes = typename detail::ChangeElementTypes<T, NewElementTs...>::type;

template <class T, class... NewElementTs>
using ChElemTs = ChangeElementTypes<T, NewElementTs...>;

}
}
