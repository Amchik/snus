#ifndef __IQmSFV_UTESTS_H
#define __IQmSFV_UTESTS_H

void utests_group(char* name);
void utests_add(char* name, char* (*test_function)());

void utests_start();

#endif
