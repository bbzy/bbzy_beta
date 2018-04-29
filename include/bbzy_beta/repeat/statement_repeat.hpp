#pragma once

#define BBZY_STATEMENT_REPEAT_0(STATEMENT_MACRO)
#define BBZY_STATEMENT_REPEAT_1(STATEMENT_MACRO)\
	BBZY_STATEMENT_REPEAT_0(STATEMENT_MACRO);\
	STATEMENT_MACRO(0);
#define BBZY_STATEMENT_REPEAT_2(STATEMENT_MACRO)\
	BBZY_STATEMENT_REPEAT_1(STATEMENT_MACRO);\
	STATEMENT_MACRO(1);
#define BBZY_STATEMENT_REPEAT_3(STATEMENT_MACRO)\
	BBZY_STATEMENT_REPEAT_2(STATEMENT_MACRO);\
	STATEMENT_MACRO(2);
#define BBZY_STATEMENT_REPEAT_4(STATEMENT_MACRO)\
	BBZY_STATEMENT_REPEAT_3(STATEMENT_MACRO);\
	STATEMENT_MACRO(3);
#define BBZY_STATEMENT_REPEAT_5(STATEMENT_MACRO)\
	BBZY_STATEMENT_REPEAT_4(STATEMENT_MACRO);\
	STATEMENT_MACRO(4);
#define BBZY_STATEMENT_REPEAT_6(STATEMENT_MACRO)\
	BBZY_STATEMENT_REPEAT_5(STATEMENT_MACRO);\
	STATEMENT_MACRO(5);
#define BBZY_STATEMENT_REPEAT_7(STATEMENT_MACRO)\
	BBZY_STATEMENT_REPEAT_6(STATEMENT_MACRO);\
	STATEMENT_MACRO(6);
#define BBZY_STATEMENT_REPEAT_8(STATEMENT_MACRO)\
	BBZY_STATEMENT_REPEAT_7(STATEMENT_MACRO);\
	STATEMENT_MACRO(7);
#define BBZY_STATEMENT_REPEAT_9(STATEMENT_MACRO)\
	BBZY_STATEMENT_REPEAT_8(STATEMENT_MACRO);\
	STATEMENT_MACRO(8);
#define BBZY_STATEMENT_REPEAT_10(STATEMENT_MACRO)\
	BBZY_STATEMENT_REPEAT_9(STATEMENT_MACRO);\
	STATEMENT_MACRO(9);