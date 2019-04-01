#pragma once
#include <string>
#include <utility>

class ClassTester {
public:
    ClassTester(std::string name = "Test") : m_name(std::move(name)) {
        std::cout << m_name << " => ClassTester()" << std::endl;
    }

    ~ClassTester() {
        std::cout << m_name << " => ~ClassTester()" << std::endl;
    }

    ClassTester(const ClassTester& other) {
        std::cout << other.m_name << " => ClassTester(const ClassTester&)" << std::endl;
        m_name = other.m_name;
    }

    ClassTester(ClassTester&& other) {
        std::cout << other.m_name << " => ClassTester(ClassTester&&)" << std::endl;
        m_name = other.m_name;
    }

public:
    ClassTester& operator = (const ClassTester& other) {
        std::cout << m_name << ":" << other.m_name << " => ClassTester::operator = (const ClassTester&)" << std::endl;
        m_name = other.m_name;
        return *this;
    }

    ClassTester& operator = (ClassTester&& other) {
        std::cout << m_name << ":" << other.m_name << " => ClassTester::operator = (ClassTester&&)" << std::endl;
        m_name = other.m_name;
        return *this;
    }

private:
    std::string m_name;
};
