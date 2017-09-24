mkdir unit_test/cmake_build_gcc_4_9
cd unit_test &&
cd cmake_build_gcc_4_9 &&
cmake -G"Unix Makefiles" .. -DCMAKE_C_COMPILER=gcc-4.9 -DCMAKE_CXX_COMPILER=g++-4.9 &&
make &&
./test_bbzy_beta
