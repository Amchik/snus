/* VIM BEST EDITOR
 * vim: ft=c
 */
#ifndef __8pu12Q_printing_h
#define __8pu12Q_printing_h

#include <sys/types.h>

/* Message part node (for internal usage) */
struct _prnt_int_msgprt {
  /*
   * ID of part (keep unique)
   * Note: id 0 will be ignored as id
   * and cannot be queried (if INT_FAIL_ZEROID)
   */
  u_int16_t id;

  /* foreground color */
  u_int8_t primary_color;
  /* background color */
  u_int8_t secondary_color;

  /* message */
  char* message_text;

  /* Next node or NULL */
  struct _prnt_int_msgprt* next;
};

/* Message part node */
typedef struct _prnt_int_msgprt* prnt_msgprt_t;

#define PRNT_INFO 0
#define _PRNT_INT_INFOP 34
#define _PRNT_INT_INFOS 0
#define PRNT_WARN 1
#define _PRNT_INT_WARNP 33
#define _PRNT_INT_WARNS 0
#define PRNT_ERRO 2
#define _PRNT_INT_ERROP 31
#define _PRNT_INT_ERROS 0
#define PRNT_DEBG 3
#define _PRNT_INT_DEBGP 32
#define _PRNT_INT_DEBGS 0
#define PRNT_BOLD 4
#define _PRNT_INT_BOLDP 1
#define _PRNT_INT_BOLDS 0
#define PRNT_CODE 5
#define _PRNT_INT_CODEP 0
#define _PRNT_INT_CODES 40
#define PRNT_SPIN 6
#define _PRNT_INT_SPINP 0
#define _PRNT_INT_SPINS 0
#define PRNT_NONE 7
#define _PRNT_INT_NONEP 0
#define _PRNT_INT_NONES 0

/* 
 * Creates new message part, pass NULL to next if not needed.
 * type param are enum, see macroses starts with PRNT_
 */
prnt_msgprt_t prnt_new(
    u_int8_t type, 
    u_int16_t id, 
    char* message_text, 
    prnt_msgprt_t next
);

prnt_msgprt_t prnt_new_info(char* message_text, prnt_msgprt_t next);
prnt_msgprt_t prnt_new_warn(char* message_text, prnt_msgprt_t next);
prnt_msgprt_t prnt_new_erro(char* message_text, prnt_msgprt_t next);
prnt_msgprt_t prnt_new_debg(char* message_text, prnt_msgprt_t next);
prnt_msgprt_t prnt_new_bold(char* message_text, prnt_msgprt_t next);
prnt_msgprt_t prnt_new_code(char* message_text, prnt_msgprt_t next);
prnt_msgprt_t prnt_new_spin(u_int16_t id, prnt_msgprt_t next);
prnt_msgprt_t prnt_new_none(char* message_text, prnt_msgprt_t next);

prnt_msgprt_t prnt_edt(prnt_msgprt_t chain, u_int16_t id);
prnt_msgprt_t prnt_edt_text(prnt_msgprt_t chain, u_int16_t id, char* text);
prnt_msgprt_t prnt_edt_remv(prnt_msgprt_t chain, u_int16_t id);
prnt_msgprt_t prnt_edt_type(prnt_msgprt_t chain, u_int16_t id, u_int8_t type);

/* Init full message or update existing and print it (without ending) */
void prnt_msg_start(prnt_msgprt_t node);

/* End existing message, removes from memory and reprint it (with ending) */
void prnt_msg_end(prnt_msgprt_t node);

/* Force place newline at end of existing message (without reprinting) */
#define prnt_msg_end_force() {putc('\n', stdout);}
/* Inline functions by @nanoqsh AKA implentation
 * in header files and -O3...
 */

#endif
