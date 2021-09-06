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

## printing

Ну... это вроде как либа для красивых сообщений...

Макросы при компиляции:

* `INT_CALCULATE_SPACES` высчитывание пробелов в терминале, если не стоит, в конце
	каждой строки будет добавляться куча пробелов (?)
	* `INT_CALCULATE_SPACES_GETENV` высчитывание по переменной `$COLUMNS`. Часто не
		работает и не рекомендуется к использованию
	* `INT_CALCULATE_SPACES_HALF` обрезание количества пробелов на 2, что бы при
		изменении размера терминала ничего не слетало. Спойлер: идея плохая
	* `INT_CALCULATE_SPACES_THREE` 2/3 пробелов
* `INT_REPORT_RUNTIME` сообщает предупреждения в рантайме в stdout. Только для отладки
* `INT_FAIL_ZEROID` не пытается найти элемент с id == 0

Использование:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "printing.h"

/* создание простого сообщения */
prnt_msgprt_t msg = prnt_new_info("Hello, ",
	prnt_new(PRNT_CODE, 1, "world", 
		prnt_new_info("! ", 
			prnt_new_spin(2, 0 /* aka NULL */))));

/* вывод сообщения */
prnt_msg_start(msg);

/* сообщения с типом spin при повторном вызове будет меняться: */
for (int i = 0; i < 25; i++) {
	prnt_msg_start(msg); /* start так же используется для обновления */
	usleep(15000);
}

/* редактирование */
prnt_edt_text(msg, 1, "Mike"); /* функции prnt_edt* возвращают найденный
																	объект, или NULL (0) если ничего не нашли */
/* prnt_edt_remv возвращает удалённый объект */
prnt_edt_remv(msg, 2);

/* после prnt_msg_end цепь вычещается из памяти */
prnt_msg_end(msg);
```

