mkdir unit_test/cmake_build_default_clang
cd unit_test &&
cd cmake_build_default_clang &&
cmake -G"Unix Makefiles" .. &&
make &&
./test_bbzy_beta
