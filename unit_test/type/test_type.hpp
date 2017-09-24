#pragma once
#include "bbzy_beta/type.hpp"
#include <iostream>
#include <cassert>
#include <type_traits>
#include <unordered_map>
#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <list>

template <class = void>
void _test_result_type()
{
	using namespace bbzy::type;
	
	struct TestC
	{
		static int fun_1()
		{
			return 0;
		}
		static int fun_2(int) 
		{ 
			return 0;
		}
		int fun_3() 
		{ 
			return 0; 
		}
		int& fun_4(int) 
		{
			return m_mem_1;
		}
		const int& operator()(int, int) const
		{
			return m_mem_1;
		}
		int m_mem_1;
	};

	static_assert(std::is_same<ResType<decltype(&TestC::fun_1)>, int>::value, "");
	static_assert(std::is_same<ResType<decltype(&TestC::fun_2)>, int>::value, "");
	static_assert(std::is_same<ResType<decltype(&TestC::fun_3)>, int>::value, "");
	static_assert(std::is_same<ResType<decltype(&TestC::fun_4)>, int&>::value, "");
	static_assert(std::is_same<ResType<decltype(&TestC::fun_4), int, long>, int&>::value, "");
	static_assert(std::is_same<ResType<TestC, int, int>, const int&>::value, "");
	static_assert(std::is_same<ResType<int TestC::*>, int&>::value, "");
}

template <class = void>
void _test_lower_const()
{
	using namespace bbzy::type;
	static_assert(IsLC<int>::value == false, "");
	static_assert(IsLC<const int>::value == false, "");
	static_assert(IsLC<int&>::value == false, "");
	static_assert(IsLC<int&&>::value == false, "");
	static_assert(IsLC<const int&>::value, "");
	static_assert(IsLC<const int&&>::value, "");
	static_assert(IsLC<int*>::value == false, "");
	static_assert(IsLC<int*const>::value == false, "");
	static_assert(IsLC<int*****>::value == false, "");
	static_assert(IsLC<const int*>::value, "");
	static_assert(IsLC<const int*&>::value, "");
	static_assert(IsLC<const int*const>::value, "");
	static_assert(IsLC<const int*const*const>::value, "");
	static_assert(IsLC<const int****>::value, "");
	static_assert(IsLC<const int*const**const*const>::value, "");
	static_assert(IsLC<const int*const**const*const&>::value, "");

	static_assert(std::is_same<AddLC<int>, int>::value, "");
	static_assert(std::is_same<AddLC<const int>, const int>::value, "");
	static_assert(std::is_same<AddLC<int&>, const int&>::value, "");
	static_assert(std::is_same<AddLC<int&&>, const int&&>::value, "");
	static_assert(std::is_same<AddLC<const int&>, const int&>::value, "");
	static_assert(std::is_same<AddLC<const int&&>, const int&&>::value, "");
	static_assert(std::is_same<AddLC<int*>, const int*>::value, "");
	static_assert(std::is_same<AddLC<int*&>, const int*&>::value, "");
	static_assert(std::is_same<AddLC<int**>, const int**>::value, "");
	static_assert(std::is_same<AddLC<const int*>, const int*>::value, "");
	static_assert(std::is_same<AddLC<const int**>, const int**>::value, "");
	static_assert(std::is_same<AddLC<int***>, const int***>::value, "");
	static_assert(std::is_same<AddLC<int*const>, const int*const>::value, "");
	static_assert(std::is_same<AddLC<int*const*const>, const int*const*const>::value, "");
	static_assert(std::is_same<AddLC<int*const*const&>, const int*const*const&>::value, "");

	static_assert(std::is_same<RmLC<const int>, const int>::value, "");
	static_assert(std::is_same<RmLC<int&>, int&>::value, "");
	static_assert(std::is_same<RmLC<int&&>, int&&>::value, "");
	static_assert(std::is_same<RmLC<const int&>, int&>::value, "");
	static_assert(std::is_same<RmLC<const int&&>, int&&>::value, "");
	static_assert(std::is_same<RmLC<int*>, int*>::value, "");
	static_assert(std::is_same<RmLC<int*const>, int*const>::value, "");
	static_assert(std::is_same<RmLC<int*&>, int*&>::value, "");
	static_assert(std::is_same<RmLC<int**>, int**>::value, "");
	static_assert(std::is_same<RmLC<const int*>, int*>::value, "");
	static_assert(std::is_same<RmLC<const int*const&>, int*const&>::value, "");
	static_assert(std::is_same<RmLC<const int**>, int**>::value, "");
	static_assert(std::is_same<RmLC<int***>, int***>::value, "");
	static_assert(std::is_same<RmLC<int*const>, int*const>::value, "");
	static_assert(std::is_same<RmLC<const int*const*const>, int*const*const>::value, "");

	static_assert(std::is_same<CpLC<int, const int>, int>::value, "");
	static_assert(std::is_same<CpLC<int*, const int&>, const int*>::value, "");
	static_assert(std::is_same<CpLC<int*&, const int*>, const int*&>::value, "");
	static_assert(std::is_same<CpLC<const int*&, int>, const int*&>::value, "");
	static_assert(std::is_same<CpLC<int***, const int>, int***>::value, "");
	static_assert(std::is_same<CpLC<int***, const int*>, const int***>::value, "");
	static_assert(std::is_same<CpLC<int*const, const int&>, const int*const>::value, "");
	static_assert(std::is_same<CpLC<int*const*const&, const int*>, const int*const*const&>::value, "");

}

template <class = void>
void _test_copy_reference()
{
	using namespace bbzy::type;
	static_assert(std::is_same<CpLRef<int, int&>, int&>::value, "");
	static_assert(std::is_same<CpLRef<int, const int&>, int&>::value, "");
	static_assert(std::is_same<CpLRef<int&, int>, int&>::value, "");
	static_assert(std::is_same<CpLRef<int, int>, int>::value, "");
	static_assert(std::is_same<CpLRef<int, const int&>, int&>::value, "");
	static_assert(std::is_same<CpLRef<const int, const int&&>, const int>::value, "");
	static_assert(std::is_same<CpRRef<int, int&&>, int&&>::value, "");
	static_assert(std::is_same<CpRRef<int, const int&&>, int&&>::value, "");
	static_assert(std::is_same<CpRRef<int&&, int>, int&&>::value, "");
	static_assert(std::is_same<CpRRef<int, int>, int>::value, "");
	static_assert(std::is_same<CpRRef<const int, const int&>, const int>::value, "");
	static_assert(std::is_same<CpRRef<int, const int&&>, int&&>::value, "");
	static_assert(std::is_same<CpRef<int, int&>, int&>::value, "");
	static_assert(std::is_same<CpRef<int, const int&>, int&>::value, "");
	static_assert(std::is_same<CpRef<int&, int>, int&>::value, "");
	static_assert(std::is_same<CpRef<int, int>, int>::value, "");
	static_assert(std::is_same<CpRef<int, int&&>, int&&>::value, "");
	static_assert(std::is_same<CpRef<int, const int&&>, int&&>::value, "");
	static_assert(std::is_same<CpRef<int&&, int>, int&&>::value, "");
	static_assert(std::is_same<CpRef<int, int>, int>::value, "");
	static_assert(std::is_same<CpRef<int, const int*&>, int&>::value, "");
	static_assert(std::is_same<CpRef<const int, const int*&>, const int&>::value, "");
	static_assert(std::is_same<CpProp<int, int*>, int>::value, "");
	static_assert(std::is_same<CpProp<int, const int*&>, const int&>::value, "");
	static_assert(std::is_same<CpProp<int, const int*const&>, const int&>::value, "");
	static_assert(std::is_same<CpProp<int*&, const int*const&>, const int*&>::value, "");
}

template <class = void>
void _test_type_exist()
{
	using namespace bbzy::type;
	static_assert(TExist<int>::value, "");
}

template <class = void>
void _test_template_class()
{
	using namespace bbzy::type;
	static_assert(TempOf<int, std::vector>::value == false, "");
	static_assert(TempOf<std::vector<int>, std::vector>::value, "");
	static_assert(TempOf<std::vector<int>, std::list>::value == false, "");
	static_assert(TempOf<std::string, std::vector>::value == false, "");
	static_assert(TempOf<std::unordered_map<int, long>, std::unordered_map>::value, "");
	static_assert(TempOf<std::unordered_map<int, long>&, std::unordered_map>::value, "");
	static_assert(TempOf<std::unordered_map<int, long>&&, std::unordered_map>::value, "");
	static_assert(TempOf<const std::unordered_map<int, long>&, std::unordered_map>::value, "");
	static_assert(TempOf<const std::unordered_map<int, long>&&, std::unordered_map>::value, "");
	static_assert(TempOf<std::unordered_map<int, long>&, std::vector>::value == false, "");
	static_assert(TempOf<std::unordered_map<int, long>&&, std::vector>::value == false, "");
	static_assert(TempOf<std::unordered_map<int, long>*, std::unordered_map>::value == false, "");
	static_assert(TempOf<const std::unordered_map<int, long>*const*&, std::unordered_map>::value == false, "");
	static_assert(TempOf<const std::unordered_map<int, long>*const*const&, std::unordered_map>::value == false, "");

	static_assert(SameTemp<int, int>::value == false, "");
	static_assert(SameTemp<std::vector<int>, std::vector<int>>::value, "");
	static_assert(SameTemp<std::vector<int>, std::vector<long>>::value, "");
	static_assert(SameTemp<std::vector<int>, std::list<int>>::value == false, "");
	static_assert(SameTemp<std::vector<int>, std::list<long>>::value == false, "");
	static_assert(SameTemp<const std::vector<std::string>&, std::vector<int>>::value, "");
	static_assert(SameTemp<const std::vector<std::string>&, std::vector<int>&&>::value, "");
	static_assert(SameTemp<std::vector<std::string>*, std::vector<int>>::value == false, "");
	static_assert(SameTemp<std::vector<std::string>&, std::vector<int>*>::value == false, "");

	static_assert(AnyOfTs<std::vector<int>, std::list>::value == false, "");
	static_assert(AnyOfTs<std::vector<int>,
		std::list, std::unordered_map, std::vector, std::unordered_multimap>::value, "");
	static_assert(AnyOfTs<std::vector<int>,
		std::list, std::unordered_map, std::unordered_multimap>::value == false, "");
	static_assert(AnyOfTs<std::vector<std::string>, std::vector, std::list>::value, "");
	static_assert(AnyOfTs<std::vector<std::string>, std::list>::value == false, "");

	static_assert(std::is_same<ChTemp<std::vector<int>, std::list>, std::list<int>>::value, "");
	static_assert(std::is_same<ChTemp<const std::vector<int>&, std::list>, const std::list<int>&>::value, "");

	static_assert(std::is_same<ChElemTs<std::vector<int>, long>, std::vector<long>>::value, "");
	static_assert(std::is_same<ChElemTs<std::unordered_map<int, long>, long, int>,
		std::unordered_map<long, int>>::value, "");
	static_assert(std::is_same<ChElemTs<std::vector<int>&, long>, std::vector<long>&>::value, "");
	static_assert(std::is_same<ChElemTs<const std::vector<int>, long>, std::vector<long>>::value, "");
	static_assert(std::is_same<ChElemTs<const std::vector<int>&, long>, const std::vector<long>&>::value, "");
	
}

template <class = void>
void _test_element_type()
{
	using namespace bbzy::type;
	static_assert(std::is_same<ElemT<std::string>, char>::value, "");
	static_assert(std::is_same<ElemT<std::map<int, long>>, std::pair<const int, long>>::value, "");
	static_assert(std::is_same<ElemT<std::string&&>, char>::value, "");
	static_assert(std::is_same<ElemT<const std::string>, char>::value, "");
	static_assert(std::is_same<ElemT<const std::string&>, char>::value, "");
	static_assert(std::is_same<ElemT<std::string*>, std::string>::value, "");
	static_assert(std::is_same<ElemT<std::string**>, std::string*>::value, "");
	static_assert(std::is_same<ElemT<std::string*const*const>, std::string*const>::value, "");
	static_assert(std::is_same<ElemT<const std::string*const*const>, const std::string*const>::value, "");
	static_assert(std::is_same<ElemT<std::string(&)[3]>, std::string>::value, "");
	static_assert(std::is_same<ElemT<const std::string(&)[5]>, const std::string>::value, "");
	
	static_assert(std::is_same<ElemRT<std::string>, char>::value, "");
	static_assert(std::is_same<ElemRT<std::map<int, long>>, std::pair<const int, long>>::value, "");
	static_assert(std::is_same<ElemRT<const std::map<int, long>&>, const std::pair<const int, long>&>::value, "");
	static_assert(std::is_same<ElemRT<std::string&&>, char&&>::value, "");
	static_assert(std::is_same<ElemRT<const std::string>, char>::value, "");
	static_assert(std::is_same<ElemRT<const std::string&>, const char&>::value, "");
	static_assert(std::is_same<ElemRT<std::string*>, std::string>::value, "");
	static_assert(std::is_same<ElemRT<std::string**>, std::string*>::value, "");
	static_assert(std::is_same<ElemRT<std::string*const*const>, std::string*const>::value, "");
	static_assert(std::is_same<ElemRT<const std::string*const*const>, const std::string*const>::value, "");
	static_assert(std::is_same<ElemRT<std::string(&)[3]>, std::string&>::value, "");
	static_assert(std::is_same<ElemRT<const std::string(&)[5]>, const std::string&>::value, "");
}

template <class = void>
void _test_index_type()
{
	using namespace bbzy::type;
	static_assert(std::is_same<TypeAt<0, char, short, int, long>, char>::value, "");
	static_assert(std::is_same<TypeAt<1, char, short, int, long>, short>::value, "");
	static_assert(std::is_same<TypeAt<2, char, short, int, long>, int>::value, "");
	static_assert(std::is_same<TypeAt<3, char, short, int, long>, long>::value, "");
	static_assert(std::is_same<TypeAt<0, char>, char>::value, "");

	static_assert(std::is_same<TTypeAt<0, std::tuple<char, short, int, long>>, char>::value, "");
	static_assert(std::is_same<TTypeAt<1, std::tuple<char, short, int, long>>, short>::value, "");
	static_assert(std::is_same<TTypeAt<2, std::tuple<char, short, int, long>>, int>::value, "");
	static_assert(std::is_same<TTypeAt<3, std::tuple<char, short, int, long>>, long>::value, "");
	static_assert(std::is_same<TTypeAt<0, std::tuple<char>>, char>::value, "");
}

template <class = void>
void _test_type_function()
{
	using namespace bbzy::type;
	struct TestC;
	static_assert(std::is_same<UniFunT<int (int, int)>, int(*)(int, int)>::value, "");
	static_assert(std::is_same<UniFunT<int (*)(int, int)>, int(*)(int, int)>::value, "");
	static_assert(std::is_same<UniFunT<int (TestC::*)(int, int)>, int(*)(TestC*, int, int)>::value, "");
	static_assert(std::is_same<UniFunT<int (TestC::*)(int, int)const>, int(*)(const TestC*, int, int)>::value, "");
	static_assert(std::is_same<UniFunT<int (int, int)>, int(*)(int, int)>::value, "");

	static_assert(std::is_same<GetFunPW<int (int, long, short)>, TypeWrapper<int, long, short>> ::value, "");
	static_assert(std::is_same<GetFunPW<int (TestC::*)(int, long, short)>, TypeWrapper<TestC*, int, long, short>>::value, "");
	static_assert(std::is_same<GetFunPW<int ()>, TypeWrapper<>> ::value, "");
	static_assert(std::is_same<GetFunPW<int (TestC::*)()const>, TypeWrapper<const TestC*>>::value, "");

	static_assert(std::is_same<GetFunPT<1, int (int, long, short)>, long>::value, "");
	static_assert(std::is_same<GetFunPT<1, int (TestC::*)(int, long, short)>, int>::value, "");

	static_assert(std::is_same<GetMethodClassType<int (TestC::*)(int, long, short)>, TestC>::value, "");
	static_assert(std::is_same<GetMethodClassType<int (TestC::*)(int, long, short)const>, const TestC>::value, "");

	static_assert(IsFunction<int ()>::value, "");
	static_assert(IsFunction<int (int, int&, int&&, int*, int**, int**&&)>::value, "");
	static_assert(IsFunction<int (*)()>::value, "");
	static_assert(IsFunction<int (*)(int, int&, int&&, int*, int**, int**&&)>::value, "");
	static_assert(IsFunction<int (&)()>::value, "");
	static_assert(IsFunction<int (&)(int, int&, int&&, int*, int**, int**&&)>::value, "");
	static_assert(IsFunction<int (&&)()>::value, "");
	static_assert(IsFunction<int (&&)(int, int&, int&&, int*, int**, int**&&)>::value, "");
	static_assert(IsFunction<int (*&&)()>::value, "");
	static_assert(IsFunction<int (*&&)(int, int&, int&&, int*, int**, int**&&)>::value, "");
	static_assert(IsFunction<int (**)()>::value == false, "");
	static_assert(IsFunction<int (**)(int, int&, int&&, int*, int**, int**&&)>::value == false, "");
	static_assert(IsFunction<int (TestC::*)()>::value == false, "");
	static_assert(IsFunction<int (TestC::*)(int, int&, int&&, int*, int**, int**&&)>::value == false, "");

	static_assert(IsMemberFunction<int (TestC::*)()>::value, "");
	static_assert(IsMemberFunction<int (TestC::*)(int, int&, int&&, int*, int**, int**&&)>::value, "");
	static_assert(IsMemberFunction<int (TestC::**)()>::value == false, "");
	static_assert(IsMemberFunction<int (TestC::**)(int, int&, int&&, int*, int**, int**&&)>::value == false, "");
	static_assert(IsMemberFunction<int (TestC::*&)()>::value, "");
	static_assert(IsMemberFunction<int (TestC::*&)(int, int&, int&&, int*, int**, int**&&)>::value, "");
	static_assert(IsMemberFunction<int (TestC::*&&)()>::value, "");
	static_assert(IsMemberFunction<int (TestC::*&&)(int, int&, int&&, int*, int**, int**&&)>::value, "");
	static_assert(IsMemberFunction<int (TestC::**&&)()>::value == false, "");
	static_assert(IsMemberFunction<int (TestC::**&&)(int, int&, int&&, int*, int**, int**&&)>::value == false, "");
	static_assert(IsMemberFunction<int (TestC::***)()>::value == false, "");
	static_assert(IsMemberFunction<int (TestC::***)(int, int&, int&&, int*, int**, int**&&)>::value == false, "");
	static_assert(IsMemberFunction<int (*)()>::value == false, "");
	static_assert(IsMemberFunction<int (*)(int, int&, int&&, int*, int**, int**&&)>::value == false, "");
	static_assert(IsMemberFunction<int (&)()>::value == false, "");
	static_assert(IsMemberFunction<int (&)(int, int&, int&&, int*, int**, int**&&)>::value == false, "");
	static_assert(IsMemberFunction<int ()>::value == false, "");
	static_assert(IsMemberFunction<int (int, int&, int&&, int*, int**, int**&&)>::value == false, "");
	static_assert(IsMemberFunction<int (TestC::*)() const>::value, "");
	static_assert(IsMemberFunction<int (TestC::*)(int, int&, int&&, int*, int**, int**&&) const>::value, "");
	static_assert(IsMemberFunction<int (TestC::**)() const>::value == false, "");
	static_assert(IsMemberFunction<int (TestC::**)(int, int&, int&&, int*, int**, int**&&) const>::value == false, "");
	static_assert(IsMemberFunction<int (TestC::*&)() const>::value, "");
	static_assert(IsMemberFunction<int (TestC::*&)(int, int&, int&&, int*, int**, int**&&) const>::value, "");
	static_assert(IsMemberFunction<int (TestC::*&&)() const>::value, "");
	static_assert(IsMemberFunction<int (TestC::*&&)(int, int&, int&&, int*, int**, int**&&) const>::value, "");
	static_assert(IsMemberFunction<int (TestC::**&&)() const>::value == false, "");
	static_assert(IsMemberFunction<int (TestC::**&&)(int, int&, int&&, int*, int**, int**&&) const>::value == false, "");
	static_assert(IsMemberFunction<int (TestC::***)() const>::value == false, "");
	static_assert(IsMemberFunction<int (TestC::***)(int, int&, int&&, int*, int**, int**&&) const>::value == false, "");
}

template <class = void>
void _test_type_wrapper()
{
	using namespace bbzy::type;
	static_assert(TypeWrapper<int, long>::size == 2, "");
	static_assert(TypeWrapper<>::size == 0, "");
}

template <class = void>
void _test_type_empty() {
	using namespace bbzy::type;
	int a = 0;
	int& b = a;
	int* c = &a;
	int* d = nullptr;
	int** e = nullptr;
	int(*f)() = nullptr;

	assert(isPointerEmpty(a) == false);
	assert(isPointerEmpty(b) == false);
	assert(isPointerEmpty(c) == false);
	assert(isPointerEmpty(d));
	assert(isPointerEmpty(e));
	assert(isPointerEmpty(f));
}

template <class = void>
void test_type()
{
	_test_result_type();
	_test_lower_const();
	_test_copy_reference();
	_test_type_exist();
	_test_template_class();
	_test_element_type();
	_test_index_type();
	_test_type_function();
	_test_type_wrapper();
	_test_type_empty();
	std::cout << "Test Type OK" << std::endl;
}