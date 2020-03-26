#pragma once

#include "bbzy_beta/disambiguate.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cassert>

template<class = void>
void _test_disambiguate_function() {
    using namespace bbzy::disam;
    struct TestC {
        static int fun() {
            return 0;
        }

        static int &fun(long, int &a) {
            return a;
        }

        static int fun(int) {
            return 0;
        }
    };
    auto a = Function<int>::get(&TestC::fun);
    auto b = Function<int &, long, int &>::get(&TestC::fun);
    auto c = Function<int, int>::get(&TestC::fun);
    assert((void *) a != (void *) b);
    assert((void *) b != (void *) c);
    assert((void *) a != (void *) c);
}

template<class = void>
void _test_disambiguate_member_function() {
    using namespace bbzy::disam;
    struct TestC {
        int fun() {
            return 0;
        }

        int fun() const {
            return 0;
        }

        int fun(int) const {
            return 0;
        }

        int *fun(long) {
            return nullptr;
        }
    };

    auto a = MemberFunction<int>::get(&TestC::fun);
    auto b = MemberFunction<int>::getConst(&TestC::fun);
    auto c = MemberFunction<int, int>::getConst(&TestC::fun);
    auto d = MemberFunction<int *, long>::get(&TestC::fun);
}

template<class = void>
void test_disambiguate() {
    _test_disambiguate_function();
    _test_disambiguate_member_function();
    std::cout << "Test Disambiguate OK" << std::endl;
}