# snus

Бесполезная коллекция кода на C

## matestr

Попытка написать какие-то строки... Использование:

```c
#include <stdio.h>
#include <stdlib.h>

#include "matestr.h"

/* Создаёт новую строку. Копирует память в новую строку */
matestr str = matestr_new("Hello world!");
/* Использование её в функциях: */
printf("String: %s, Length: %u, Allocated: %u",
	str.cstr, str.length, str.__allocated);
/* Данная функция ставит нулевой символ (окончание строки)
   в cstr[length] */
matestr_normalize(str);
/* Данная функция перевыделяет память строке, если ей
	 было выделено больше её длинны + 1 */
matestr_optimize(str);
/* Данная функция добавляет (копирует) строку в строку */
matestr_append(str, " How are you?");

/* Пока, для вычестки из памяти строки нужно освободить cstr: */
free(str.cstr);
```

