#include "bbzy_beta/repeat.hpp"

#include <vector>

void test_repeat() {
#define DEF(n) const int var_##n = n
    BBZY_STATEMENT_REPEAT_5(DEF);
#undef DEF

    static_assert(var_0 == 0, "");
    static_assert(var_1 == 1, "");
    static_assert(var_2 == 2, "");
    static_assert(var_3 == 3, "");
    static_assert(var_4 == 4, "");

    constexpr int v[] = {BBZY_PARAM_REPEAT_5(int)};
    static_assert(v[0] == 0, "");
    static_assert(v[1] == 1, "");
    static_assert(v[2] == 2, "");
    static_assert(v[3] == 3, "");
    static_assert(v[4] == 4, "");

    constexpr int va[] = {-1 BBZY_APPEND_PARAM_REPEAT_5(int)};
    static_assert(va[0] == -1, "");
    static_assert(va[1] == 0, "");
    static_assert(va[2] == 1, "");
    static_assert(va[3] == 2, "");
    static_assert(va[4] == 3, "");
    static_assert(va[5] == 4, "");
}
