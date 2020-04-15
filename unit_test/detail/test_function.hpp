#pragma once

#include <functional>
#include "bbzy_beta/function.hpp"

class C {
public:
    explicit C(int value) : m_value(value) {
    }

public:
    int foo(int a, int b) const {
        return a + b + m_value;
    }

private:
    int m_value{};
};

void test_bind_this() {
    C c{3};
    auto &&functor = bbzy::function::bindThis(&c, &C::foo);
    assert(functor(1, 9) == 13);
    std::function<int(int, int)> functorContainer{functor};
    assert(functorContainer(7, 5) == 15);
}

void test_function() {
    test_bind_this();
    std::cout << "Test Function OK" << std::endl;
}