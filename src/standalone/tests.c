#include "tests.h"
#include "simulator.h"
#include <stdio.h>
#define SELECT(__test_number__, __select__) {if(__select__ >= 0 && __test_number__ != __select__) return;}

void _test_0(int select)
{
	SELECT(0, select);
	printf("executing test 0\n");
	churn(1, 60);
	printf("test 0 complete\n");
}

void run_tests(int select) {
	printf("Running tests.\n");
	_test_0(select);
}