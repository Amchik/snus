#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include "include/printing.h"

/* I can calculate end space and print spaces ok, but
 * im so lazy for it and this define here
 */
#ifndef INT_CALCULATE_SPACES
/* yeah, i can't use INT_CALCULATE_SPACES macros
 * in code:
 */
#define _PRNT_INT_NO_CALCULATE_SPACES
#else

u_int16_t _prnt_int_termcols();

#ifndef INT_CALCULATE_SPACES_GETENV

#include <sys/ioctl.h>

u_int16_t _prnt_int_termcols() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  /* 15iq fixes: */
#ifdef INT_CALCULATE_SPACES_THREE
  return(w.ws_col / 3 * 2);
#elif defined(INT_CALCULATE_SPACES_HALF)
  return(w.ws_col / 2);
#else
  return(w.ws_col);
#endif
}

#else

/* 
 * NOTE: if enviroment variable $COLUMNS
 * not exported by shell it will be
 * throw SEGFAULT, but gdb works fine (nice)
 */

/* #include <signal.h> */

u_int16_t _prnt_int_termcols() {
  char* s;

  /* Prevent any error: */
  /* raise(9); */

  s = getenv("COLUMNS");
/*if (!s) return(0);    no fallback calculating here, umm..., 
                        do not use INT_CALCULATE_SPACES_GETENV */
  if (!s) {
#ifdef INT_REPORT_RUNTIME
    /* may be remove this message? */
    puts("[INT_REPORT_RUNTIME@_prnt_int_termcols(), INT_CALCULATE_SPACES_GETENV] "
        "I see you idiot, recompile this library without INT_CALCULATE_SPACES_GETENV "
        "because there no fallback function. If you need broke you'r terminal do "
        "not pass INT_CALCULATE_SPACES flag while compilation (see makefile)");
    puts("[INT_REPORT_RUNTIME@_prnt_int_termcols(), INT_CALCULATE_SPACES_GETENV] "
        "Notice: $COLUMNS enviroment variable not set. Returning 0");
#endif
    return(0);
  }

  /* 27iq fixes: */
#ifdef INT_CALCULATE_SPACES_THREE
  return(atoi(s) / 3 * 2);
#elif defined(INT_CALCULATE_SPACES_HALF)
  return(atoi(s) / 2);
#else
  return(atoi(s));
#endif
}

#endif /* something */

#endif /* um... */

/* beautiful inline-functions in C89 and -O0: */

#define _PRNT_INT_NEW(text, primaryColor, secondaryColor, _id) \
  {                                                            \
    prnt_msgprt_t answer;                                      \
    answer = malloc(sizeof(struct _prnt_int_msgprt));          \
    answer->message_text = text;                               \
    answer->id = _id;                                          \
    answer->next = next;                                       \
    answer->primary_color = primaryColor;                      \
    answer->secondary_color = secondaryColor == 0 ?            \
      primaryColor : secondaryColor;                           \
    return(answer);                                            \
  } /* may be check for secondaryColor == 0 useless, hmm */

#define _PRNT_INT_SPIN_NEXT                                    \
  {                                                            \
    switch (*_prnt_int_spin_state) {                           \
      case '|':                                                \
        *_prnt_int_spin_state = '/';                           \
        break;                                                 \
      case '/':                                                \
        *_prnt_int_spin_state = '-';                           \
        break;                                                 \
      case '-':                                                \
        *_prnt_int_spin_state = '\\';                          \
        break;                                                 \
      case '\\':                                               \
      default:                                                 \
        *_prnt_int_spin_state = '|';                           \
        break;                                                 \
    }                                                          \
  }

static char _prnt_int_spin_state[2] = {'|', 0}; /* yeah... */

/* le cringe */

prnt_msgprt_t prnt_new(
    u_int8_t type,
    u_int16_t id,
    char* message_text,
    prnt_msgprt_t next
) {
  switch (type) {
    case PRNT_INFO:
      _PRNT_INT_NEW(message_text, _PRNT_INT_INFOP, _PRNT_INT_INFOS, id);
    case PRNT_WARN:
      _PRNT_INT_NEW(message_text, _PRNT_INT_WARNP, _PRNT_INT_WARNS, id);
    case PRNT_ERRO:
      _PRNT_INT_NEW(message_text, _PRNT_INT_ERROP, _PRNT_INT_ERROS, id);
    case PRNT_DEBG:
      _PRNT_INT_NEW(message_text, _PRNT_INT_DEBGP, _PRNT_INT_DEBGS, id);
    case PRNT_BOLD:
      _PRNT_INT_NEW(message_text, _PRNT_INT_BOLDP, _PRNT_INT_BOLDS, id);
    case PRNT_CODE:
      _PRNT_INT_NEW(message_text, _PRNT_INT_CODEP, _PRNT_INT_CODES, id);
    case PRNT_SPIN:
      _PRNT_INT_NEW(_prnt_int_spin_state, _PRNT_INT_SPINP, _PRNT_INT_SPINS, id);
    case PRNT_NONE:
      _PRNT_INT_NEW(message_text, _PRNT_INT_NONEP, _PRNT_INT_NONES, id);
    default: return(0);
  }
}

/* DRY: Do Repeat Youself
 *
 * Hmm, may be use -macroses- INLINE FUNCTIONS 
 *   in header file?
 */

prnt_msgprt_t prnt_new_info(char* message_text, prnt_msgprt_t next)
  _PRNT_INT_NEW(message_text, _PRNT_INT_INFOP, _PRNT_INT_INFOS, 0)
prnt_msgprt_t prnt_new_warn(char* message_text, prnt_msgprt_t next)
  _PRNT_INT_NEW(message_text, _PRNT_INT_WARNP, _PRNT_INT_WARNS, 0)
prnt_msgprt_t prnt_new_erro(char* message_text, prnt_msgprt_t next)
  _PRNT_INT_NEW(message_text, _PRNT_INT_ERROP, _PRNT_INT_ERROS, 0)
prnt_msgprt_t prnt_new_debg(char* message_text, prnt_msgprt_t next)
  _PRNT_INT_NEW(message_text, _PRNT_INT_DEBGP, _PRNT_INT_DEBGS, 0)
prnt_msgprt_t prnt_new_bold(char* message_text, prnt_msgprt_t next)
  _PRNT_INT_NEW(message_text, _PRNT_INT_BOLDP, _PRNT_INT_BOLDS, 0)
prnt_msgprt_t prnt_new_code(char* message_text, prnt_msgprt_t next)
  _PRNT_INT_NEW(message_text, _PRNT_INT_CODEP, _PRNT_INT_CODES, 0)
prnt_msgprt_t prnt_new_spin(u_int16_t id, prnt_msgprt_t next)
  _PRNT_INT_NEW(_prnt_int_spin_state, _PRNT_INT_SPINP, _PRNT_INT_SPINS, id)
prnt_msgprt_t prnt_new_none(char* message_text, prnt_msgprt_t next)
  _PRNT_INT_NEW(message_text, _PRNT_INT_NONEP, _PRNT_INT_NONES, 0)

prnt_msgprt_t prnt_edt(prnt_msgprt_t chain, u_int16_t id) {
  prnt_msgprt_t cursor;

#ifdef INT_FAIL_ZEROID
  if (id == 0) {
#ifdef INT_REPORT_RUNTIME
    fprintf(stderr, "[INT_REPORT_RUNTIME@prnt_edt(%p, %u)] "
        "Failed due INT_FAIL_ZEROID set\n",
        (void*)chain, id);
#endif
    return(0); /* error: id == 0 (INT_FAIL_ZEROID) */
  }
#endif

  for (cursor = chain; cursor != 0; cursor = cursor->next) {
    if (cursor->id == id) {
      return(cursor);
    }
  }

  return(0); /* return zero as not found */
}
prnt_msgprt_t prnt_edt_text(prnt_msgprt_t chain, u_int16_t id, char* text) {
  /* TODO: rewrite this to prnt_edt() call (for example see prnt_edt_type()) */
  /* !!! DO NOT FORGET BROKE ANY OTHER FUNCTION !!! */
  prnt_msgprt_t cursor;

#ifdef INT_FAIL_ZEROID
  if (id == 0) {
#ifdef INT_REPORT_RUNTIME
    fprintf(stderr, "[INT_REPORT_RUNTIME@prnt_edt_text(%p, %u)] "
        "Failed due INT_FAIL_ZEROID set\n",
        (void*)chain, id);
#endif
    return(0); /* error: id == 0 (INT_FAIL_ZEROID) */
  }
#endif

  for (cursor = chain; cursor != 0; cursor = cursor->next) {
    if (cursor->id == id) {
      cursor->message_text = text;
      return(cursor);
    }
  }

  return(0); /* return zero as not found */
}
prnt_msgprt_t prnt_edt_remv(prnt_msgprt_t chain, u_int16_t id) {
  prnt_msgprt_t cursor, old;

  old = 0;
#ifdef INT_FAIL_ZEROID
  if (id == 0) {
#ifdef INT_REPORT_RUNTIME
    fprintf(stderr, "[INT_REPORT_RUNTIME@prnt_edt_remv(%p, %u)] "
        "Failed due INT_FAIL_ZEROID set\n",
        (void*)chain, id);
#endif
    return(0); /* error: id == 0 (INT_FAIL_ZEROID) */
  }
#endif

  for (cursor = chain; cursor != 0; cursor = cursor->next) {
    if (cursor->id == id) {
      if (!old) {
#ifdef INT_REPORT_RUNTIME
        fprintf(stderr, "[INT_REPORT_RUNTIME@prnt_edt_remv(%p, %u)] "
            "Attemtp to remove first node from chain. "
            "Try to use chain->next as first node.\n",
            (void*)chain, id);
#endif
        return(0); /* error: attemtp to remove first node */
      }
      old->next = cursor->next;
      free(cursor); /* possibly SIGIOT for stack, just ignore */
      return(cursor); /* return disposed object... lol */
    }
    old = cursor;
  }

  return(0); /* return zero as not found */
}
prnt_msgprt_t prnt_edt_type(prnt_msgprt_t chain, u_int16_t id, u_int8_t type) {
  prnt_msgprt_t node;

  node = prnt_edt(chain, id);
  if (!node) /* node == 0 */
    return(node); /* return(0); */
  switch (type) {
    case PRNT_INFO: /* um... secondary color can be possibly 0, just ignore */
      node->primary_color = _PRNT_INT_INFOP;
      node->secondary_color = _PRNT_INT_INFOS;
      break;
    case PRNT_WARN:
      node->primary_color = _PRNT_INT_WARNP;
      node->secondary_color = _PRNT_INT_WARNS;
      break;
    case PRNT_ERRO:
      node->primary_color = _PRNT_INT_ERROP;
      node->secondary_color = _PRNT_INT_ERROS;
      break;
    case PRNT_DEBG:
      node->primary_color = _PRNT_INT_DEBGP;
      node->secondary_color = _PRNT_INT_DEBGS;
      break;
    case PRNT_BOLD:
      node->primary_color = _PRNT_INT_BOLDP;
      node->secondary_color = _PRNT_INT_BOLDS;
      break;
    case PRNT_CODE:
      node->primary_color = _PRNT_INT_CODEP;
      node->secondary_color = _PRNT_INT_CODES;
      break;
    case PRNT_SPIN:
      node->primary_color = _PRNT_INT_SPINP;
      node->secondary_color = _PRNT_INT_SPINS;
      break;
    case PRNT_NONE:
      node->primary_color = _PRNT_INT_NONEP;
      node->secondary_color = _PRNT_INT_NONES;
      break;
    default: return(0); /* why? */
  }

  return(node);
}

void prnt_msg_start(prnt_msgprt_t node) {
  prnt_msgprt_t cursor;
#ifndef _PRNT_INT_NO_CALCULATE_SPACES
  u_int16_t len, mxlen;

  len = 0;
  mxlen = _prnt_int_termcols();
#endif

  for (cursor = node; cursor != 0; cursor = cursor->next) {
#ifndef _PRNT_INT_NO_CALCULATE_SPACES
    len += strlen(cursor->message_text);
#endif
    printf("\033[0;%d;%dm%s\033[0m", cursor->primary_color,
        cursor->secondary_color == 0 ? cursor->primary_color : 
        cursor->secondary_color, cursor->message_text);
  }

#ifdef _PRNT_INT_NO_CALCULATE_SPACES
  printf("                     \r"); /* but its works! */
#else
  for (; len < mxlen; len++) {
    putc(' ', stdout);
  }
  putc('\r', stdout);
#endif

  fflush(stdout);

  _PRNT_INT_SPIN_NEXT
}

void prnt_msg_end(prnt_msgprt_t node) {
  prnt_msgprt_t cursor;
#ifndef _PRNT_INT_NO_CALCULATE_SPACES
  u_int16_t len, mxlen;

  len = 0;
  mxlen = _prnt_int_termcols();
#endif

  for (cursor = node; cursor != 0; ) {
    prnt_msgprt_t old;

#ifndef _PRNT_INT_NO_CALCULATE_SPACES
    len += strlen(cursor->message_text);
#endif
    printf("\033[0;%d;%dm%s\033[0m", cursor->primary_color,
        cursor->secondary_color == 0 ? cursor->primary_color : 
        cursor->secondary_color, cursor->message_text);
    
    old = cursor;
    cursor = cursor->next;
    free(old);
  }

#ifdef _PRNT_INT_NO_CALCULATE_SPACES
  printf("                     \n");
#else
  for (; len < mxlen; len++) {
    putc(' ', stdout);
  }
  putc('\n', stdout);
#endif

  fflush(stdout);

  _PRNT_INT_SPIN_NEXT
}
