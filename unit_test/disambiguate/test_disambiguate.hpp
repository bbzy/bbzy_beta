#pragma once
#include "bbzy_beta/disambiguate.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cassert>

template <class = void>
void _test_disambiguate_function()
{
	using namespace bbzy::disam;
	struct TestC
	{
		static int fun();
		static int& fun();
		static int fun(int);
	};
	auto a = getFunction<int, void>(&TestC::fun);
	auto b = getFunction<int&, void>(&TestC::fun);
	auto c = getFunction<int, int>(&TestC::fun);
	assert(a != b);
	assert(b != c);
	assert(a != c);
}

template <class = void>
void _test_disambiguate_method()
{
	using namespace bbzy::disam;
	struct TestC
	{
		int fun();
		int fun() const;
		int fun(int) const;
		int& fun();
	};
	auto a = getMethod<int>(&TestC::fun);
	auto b = getCMethod<int, TestC, void>(&TestC::fun);
	auto c = getCMethod<int, TestC, int>(&TestC::fun);
	auto d = getCMethod<int&>(&TestC::fun);
	assert(a != b);
	assert(a != c);
	assert(a != d);
	assert(b != c);
	assert(b != d);
	assert(c != d);
}

template <class = void>
void test_disambiguate()
{
	_test_disambiguate_function();
	_test_disambiguate_method();
	std::cout << "Test Disambiguate OK" << std::endl;
}