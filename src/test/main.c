#include <stdio.h>
#include <unistd.h>

#include "include/utests.h"

#ifdef BUILD_TEST

char* some_test() {
	sleep(1);
	return(0);
}
char* failed_test() {
	sleep(1);
	return("Expected 4, actual 0");
}

int main(void) {
	utests_add("Ungroup test", some_test);
	utests_group("Testing unit test framework");
	utests_add("Some test", some_test);
	utests_add("Failed test", failed_test);
	utests_group("Another group");
	utests_add("Another test", some_test);

	utests_start();

	return(127);
}

#endif
