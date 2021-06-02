#pragma once
#include <string>
#include "bbzy_beta/swapper.hpp"

void test_swapper()
{
    struct S
    {
        std::string m_s;
        int m_a;
        long m_l;
    };

    S s1;
    s1.m_s = "123";
    s1.m_a = 123;
    s1.m_l = 456;
    S s2;
    s2.m_s = "abc";
    s2.m_a = 'a';
    s2.m_l = 'l';
    bbzy::swapper::swapObjects(s1, s2, &s1.m_s, &s1.m_a, &s1.m_l);
    assert(s1.m_s == "abc");
    assert(s1.m_a == 'a');
    assert(s1.m_l == 'l');
    assert(s2.m_s == "123");
    assert(s2.m_a == 123);
    assert(s2.m_l == 456);

    std::cout << "Test Swapper OK" << std::endl;
}