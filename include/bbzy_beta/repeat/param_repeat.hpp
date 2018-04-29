#pragma once

#define BBZY_PARAM_REPEAT_0(PARAM_MACRO)
#define BBZY_PARAM_REPEAT_1(PARAM_MACRO) PARAM_MACRO(0)
#define BBZY_PARAM_REPEAT_2(PARAM_MACRO) BBZY_PARAM_REPEAT_1(PARAM_MACRO), PARAM_MACRO(1)
#define BBZY_PARAM_REPEAT_3(PARAM_MACRO) BBZY_PARAM_REPEAT_2(PARAM_MACRO), PARAM_MACRO(2)
#define BBZY_PARAM_REPEAT_4(PARAM_MACRO) BBZY_PARAM_REPEAT_3(PARAM_MACRO), PARAM_MACRO(3)
#define BBZY_PARAM_REPEAT_5(PARAM_MACRO) BBZY_PARAM_REPEAT_4(PARAM_MACRO), PARAM_MACRO(4)
#define BBZY_PARAM_REPEAT_6(PARAM_MACRO) BBZY_PARAM_REPEAT_5(PARAM_MACRO), PARAM_MACRO(5)
#define BBZY_PARAM_REPEAT_7(PARAM_MACRO) BBZY_PARAM_REPEAT_6(PARAM_MACRO), PARAM_MACRO(6)
#define BBZY_PARAM_REPEAT_8(PARAM_MACRO) BBZY_PARAM_REPEAT_7(PARAM_MACRO), PARAM_MACRO(7)
#define BBZY_PARAM_REPEAT_9(PARAM_MACRO) BBZY_PARAM_REPEAT_8(PARAM_MACRO), PARAM_MACRO(8)
#define BBZY_PARAM_REPEAT_10(PARAM_MACRO) BBZY_PARAM_REPEAT_9(PARAM_MACRO), PARAM_MACRO(9)

#define BBZY_APPEND_PARAM_REPEAT_0(PARAM_MACRO)
#define BBZY_APPEND_PARAM_REPEAT_1(PARAM_MACRO) BBZY_APPEND_PARAM_REPEAT_0(PARAM_MACRO), PARAM_MACRO(0)
#define BBZY_APPEND_PARAM_REPEAT_2(PARAM_MACRO) BBZY_APPEND_PARAM_REPEAT_1(PARAM_MACRO), PARAM_MACRO(1)
#define BBZY_APPEND_PARAM_REPEAT_3(PARAM_MACRO) BBZY_APPEND_PARAM_REPEAT_2(PARAM_MACRO), PARAM_MACRO(2)
#define BBZY_APPEND_PARAM_REPEAT_4(PARAM_MACRO) BBZY_APPEND_PARAM_REPEAT_3(PARAM_MACRO), PARAM_MACRO(3)
#define BBZY_APPEND_PARAM_REPEAT_5(PARAM_MACRO) BBZY_APPEND_PARAM_REPEAT_4(PARAM_MACRO), PARAM_MACRO(4)
#define BBZY_APPEND_PARAM_REPEAT_6(PARAM_MACRO) BBZY_APPEND_PARAM_REPEAT_5(PARAM_MACRO), PARAM_MACRO(5)
#define BBZY_APPEND_PARAM_REPEAT_7(PARAM_MACRO) BBZY_APPEND_PARAM_REPEAT_6(PARAM_MACRO), PARAM_MACRO(6)
#define BBZY_APPEND_PARAM_REPEAT_8(PARAM_MACRO) BBZY_APPEND_PARAM_REPEAT_7(PARAM_MACRO), PARAM_MACRO(7)
#define BBZY_APPEND_PARAM_REPEAT_9(PARAM_MACRO) BBZY_APPEND_PARAM_REPEAT_8(PARAM_MACRO), PARAM_MACRO(8)
#define BBZY_APPEND_PARAM_REPEAT_10(PARAM_MACRO) BBZY_APPEND_PARAM_REPEAT_9(PARAM_MACRO), PARAM_MACRO(9)
