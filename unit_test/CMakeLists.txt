cmake_minimum_required(VERSION 3.1)
project(test_bbzy_beta)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES test.cpp)
add_executable(test_bbzy_beta ${SOURCE_FILES})
target_include_directories(test_bbzy_beta PRIVATE "../include")