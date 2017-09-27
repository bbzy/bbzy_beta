mkdir unit_test/cmake_build_clang_3_8
cd unit_test &&
cd cmake_build_clang_3_8 &&
cmake -G"Unix Makefiles" .. -DCMAKE_C_COMPILER=clang-3.8 -DCMAKE_CXX_COMPILER=clang++-3.8 &&
make &&
./test_bbzy_beta
