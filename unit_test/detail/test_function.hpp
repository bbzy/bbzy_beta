#pragma once

#include <functional>
#include "bbzy_beta/function.hpp"

class C {
public:
    explicit C(int value) : m_value(value) {
    }

public:
    int foo(int a) {
        return a + m_value;
    }

    int fooC(int a, int b) const {
        return a + b + m_value;
    }

public:
    static int fooS(int a, int b) {
        return a - b;
    }

    static C& fooD(C& c) {
        return c;
    }

public:
    int m_value{};
};

void test_partial() {
    C c{4};
    const C d{8};
    assert(bbzy::function::partial(&C::foo)(&c, 4) == 8);
    assert(bbzy::function::partial(&C::foo, &c)(4) == 8);
    assert(bbzy::function::partial(&C::foo, &c, 4)() == 8);
    assert(bbzy::function::partial(&C::fooC)(&c, 3, 4) == 11);
    assert(bbzy::function::partial(&C::fooC, &c)(3, 4) == 11);
    assert(bbzy::function::partial(&C::fooC, &c, 3)(4) == 11);
    assert(bbzy::function::partial(&C::fooC, &c, 3, 4)() == 11);
    assert(bbzy::function::partial(&C::fooC)(&d, 3, 4) == 15);
    assert(bbzy::function::partial(&C::fooC, &d)(3, 4) == 15);
    assert(bbzy::function::partial(&C::fooC, &d, 3)(4) == 15);
    assert(bbzy::function::partial(&C::fooC, &d, 3, 4)() == 15);
    assert(bbzy::function::partial(&C::fooS)(3, 4) == -1);
    assert(bbzy::function::partial(&C::fooS)(3, 4) == -1);
    assert(bbzy::function::partial(&C::fooS, 3)(4) == -1);
    assert(bbzy::function::partial(&C::fooS, 3, 4)() == -1);
    auto&& xx = bbzy::function::partial(&C::fooD)(c);
    assert((std::is_same<decltype(xx), C&>::value));
    xx.m_value = 9;
    assert(c.m_value == 9);
    auto yy = xx;
    auto zz = std::move(xx);
}

void test_function() {
//    test_bind_this();
    test_partial();
    std::cout << "Test Function OK" << std::endl;
}