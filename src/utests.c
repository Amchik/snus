#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "include/utests.h"
#include "include/printing.h"

#ifndef UTESTS_ARRAY_LENGTH
#define UTESTS_ARRAY_LENGTH 32
#endif

typedef struct {
  u_int32_t group_id;
  char* name;
  char* (*test_function)(void);
} testfn_t;

static char** GROUPS = 0;
static u_int32_t GROUPS_COUNT = 0;
static u_int32_t GROUPS_ALLOCATED = 0;

static testfn_t* TESTFNS = 0;
static u_int32_t TESTFNS_COUNT = 0;
static u_int32_t TESTFNS_ALLOCATED = 0;

void utests_init() {
  GROUPS_ALLOCATED = sizeof(char*) * UTESTS_ARRAY_LENGTH;
  GROUPS = malloc(GROUPS_ALLOCATED);

  TESTFNS_ALLOCATED = sizeof(testfn_t) * UTESTS_ARRAY_LENGTH;
  TESTFNS = malloc(TESTFNS_ALLOCATED);
}
void utests_extend() {
  GROUPS_ALLOCATED += sizeof(char*) * UTESTS_ARRAY_LENGTH;
  GROUPS = realloc(GROUPS, GROUPS_ALLOCATED);

  TESTFNS_ALLOCATED += sizeof(testfn_t) * UTESTS_ARRAY_LENGTH;
  TESTFNS = realloc(TESTFNS, TESTFNS_ALLOCATED);
}

void utests_group(char* name) {
  if (!GROUPS) {
    utests_init();
  }

  GROUPS[GROUPS_COUNT] = name;
  GROUPS_COUNT++;
}
void utests_add(char* name, char* (*test_function)()) {
  testfn_t el;

  if (!TESTFNS) {
    utests_init();
  }

  el.group_id = GROUPS_COUNT - 1;
  el.name = name;
  el.test_function = test_function;

  TESTFNS[TESTFNS_COUNT] = el;
  TESTFNS_COUNT++;
}

void utests_start() {
  prnt_msgprt_t chain, el;
  char* result;
  u_int32_t i, gid, failed;
  testfn_t cursor;

  gid = -1;
  failed = 0;

  for(i = 0; i < TESTFNS_COUNT; i++) {
    cursor = TESTFNS[i];
    if (gid != cursor.group_id) {
      gid = cursor.group_id;
      prnt_msg_end(prnt_new_none("Group: ",
            prnt_new_code(GROUPS[gid], 0)));
    }
    chain = prnt_new_none("  ",
#ifndef INT_NO_THREADING
      prnt_new_spin(1,
#else
      prnt_new(PRNT_NONE, 1, " ",
#endif
        prnt_new_none(" => ",
          prnt_new_code(cursor.name,
            prnt_new_none(" ",
              prnt_new(PRNT_NONE, 2, "running", 0))))));
#ifndef INT_NO_THREADING
    prnt_msg_tstart(chain);
#else
    prnt_msg_start(chain);
#endif

    result = cursor.test_function();
    if (!result) {
      el = prnt_edt_type(chain, 1, PRNT_DEBG);
      el->message_text = "✓";
      el = prnt_edt_type(chain, 2, PRNT_DEBG);
      el->message_text = "passed";
    } else {
      el = prnt_edt_type(chain, 1, PRNT_ERRO);
      el->message_text = "✗";
      el = prnt_edt_type(chain, 2, PRNT_ERRO);
      el->message_text = result;
      failed++;
    }
#ifndef INT_NO_THREADING
    prnt_msg_tend();
#else
    prnt_msg_end(chain);
#endif
  }

  printf("Total %d tests, \033[%dm%d\033[0m passed, \033[%dm%d\033[0m failed\n", 
      TESTFNS_COUNT, failed == 0 ? 32 : 0, TESTFNS_COUNT - failed, failed != 0 ? 31 : 0, failed);

  if (failed != 0) {
    exit(1);
  } else {
    exit(0);
  }
}
