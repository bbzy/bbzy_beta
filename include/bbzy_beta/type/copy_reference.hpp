#pragma once
#include "../common.hpp"
#include "lower_const.hpp"
namespace bbzy {
namespace type {
namespace detail {
template <class TargetT, class SourceT>
struct CopyLReference
{
	typedef Conditional<std::is_lvalue_reference<SourceT>::value,
		AddLValueReference<TargetT>, TargetT> type;
};

template <class TargetT, class SourceT>
struct CopyRReference
{
	typedef Conditional<std::is_rvalue_reference<SourceT>::value,
		AddRValueReference<TargetT>, TargetT> type;
};
}

template <class TargetT, class SourceT>
using CopyLReference = typename detail::CopyLReference<TargetT, SourceT>::type;

template <class TargetT, class SourceT>
using CpLRef = CopyLReference<TargetT, SourceT>;

template <class TargetT, class SourceT>
using CopyRReference = typename detail::CopyRReference<TargetT, SourceT>::type;

template <class TargetT, class SourceT>
using CpRRef = CopyRReference<TargetT, SourceT>;

template <class TargetT, class SourceT>
using CopyReference = CpRRef<CpLRef<TargetT, SourceT>, SourceT>;

template <class TargetT, class SourceT>
using CpRef = CopyReference<TargetT, SourceT>;

template <class TargetT, class SourceT>
using CopyProp = CpLC<CpRef<TargetT, SourceT>, SourceT>;

template <class TargetT, class SourceT>
using CpProp = CopyProp<TargetT, SourceT>;


}
}