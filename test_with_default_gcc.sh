mkdir unit_test/cmake_build_default_gcc
cd unit_test &&
cd cmake_build_default_gcc &&
cmake -G"Unix Makefiles" .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ &&
make &&
./test_bbzy_beta
