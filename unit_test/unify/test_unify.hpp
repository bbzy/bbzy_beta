#pragma once
#include "bbzy_beta/unify.hpp"
#include <iostream>
#include <cassert>

template <class = void>
void _test_function()
{
	using namespace bbzy::unify;
	int a = 1;
	struct TestC
	{
		static int fun_1() { return 3; }
		static int& fun_2(int& v) { return v; }
		int fun_3() { return 4; }
		int operator()() const { return 5; }
		int& fun_4(int& v) const { return v; }
		int* fun_5() { return nullptr; }
		int&& fun_6() { int v = 0; return std::move(v); }
	};

	static_assert(std::is_same<bbzy::type::ResType<decltype(makeDelegateFunction(&TestC::fun_1))>, int>::value, "");
	auto f_1 = makeDelegateFunction(&TestC::fun_1);
	assert(f_1() == 3);

	static_assert(std::is_same<bbzy::type::ResType<decltype(makeDelegateFunction(&TestC::fun_2))>, int&>::value, "");
	auto f_2 = makeDelegateFunction(&TestC::fun_2);
	f_2(a) = 2;
	assert(a == 2);

	TestC tc;
	static_assert(std::is_same<bbzy::type::ResType<decltype(makeDelegateFunction(&TestC::fun_3))>, int>::value, "");
	auto f_3 = makeDelegateFunction(&TestC::fun_3);
	assert(f_3(tc) == 4);

	static_assert(std::is_same<bbzy::type::ResType<decltype(makeDelegateFunction(&TestC::fun_4))>, int&>::value, "");
	auto f_4 = makeDelegateFunction(&TestC::fun_4);
	f_4(tc, a) = 3;
	assert(a == 3);

	static_assert(std::is_same<bbzy::type::ResType<decltype(makeDelegateFunction(&TestC::fun_5))>, int*>::value, "");
	auto f_5 = makeDelegateFunction(tc);
	assert(f_5() == 5);

	static_assert(std::is_same<bbzy::type::ResType<decltype(makeDelegateFunction(&TestC::fun_6))>, int&&>::value, "");
}

template <class = void>
void test_unity()
{
	_test_function();
	std::cout << "Test Unify OK" << std::endl;
}
