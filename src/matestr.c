#include <stdlib.h>
#include <string.h>

#include "./include/matestr.h"

/*
 * Creates new instance of matestr.
 * Calculates length of cstr, if cstr == 0,
 * passes empty allocated string
 */
matestr matestr_new(char* cstr) {
  matestr _new;

  if (cstr == 0) {
    _new.cstr = malloc(1);
    _new.length = 0;
    _new.__allocated = 1;
    _new.cstr[0] = '\0';
  } else {
    _new.length = strlen(cstr);
    _new.__allocated = _new.length + 1;
    _new.cstr = malloc(_new.__allocated);
    memcpy(_new.cstr, cstr, _new.__allocated);
  }

  return(_new);
}
/*
 * Normalizes cstr in matestr
 */
void    matestr_normalize(matestr self) {
  self.cstr[self.length] = '\0';

  return;
}
/*
 * Optimizes memory in matestr
 */
void    matestr_optimize(matestr self) {
  if (self.length + 1 == self.__allocated) {
    return;
  }

  self.__allocated = self.length + 1;
  self.cstr = realloc(self.cstr, self.__allocated);

  return;
}
/*
 * Copy string to matestr
 */
void    matestr_append(matestr self, char* cstr) {
  u_int32_t cstr_len;

  cstr_len = strlen(cstr);

  self.__allocated += cstr_len;
  self.cstr = realloc(self.cstr, self.__allocated);
  memcpy(self.cstr + self.length, cstr, cstr_len);
  self.length += cstr_len;
  self.cstr[self.length] = '\0'; /* normalize */

  return;
}
