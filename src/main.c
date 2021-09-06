#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/printing.h"

char* getusername(void) {
	char* name = getenv("USER");
	if (!name)
		name = "Kate";
	return(name);
}

int main(void) {
	prnt_msgprt_t chain, el;
	int i;

	puts("=== printing demo ===");

	chain = prnt_new(PRNT_INFO, 0, "Hello, ",
			prnt_new(PRNT_CODE, 1, "world", 
				prnt_new(PRNT_INFO, 0, "! ",
					prnt_new_spin(2, 
						prnt_new(PRNT_NONE, 3, " loading", 0)))));

	prnt_msg_start(chain);

	for (i = 0; i < 25; i++) {
		usleep(50000);
		prnt_msg_start(chain);
	}

	el = prnt_edt(chain, 1);
	el->message_text = getusername();

	prnt_edt_remv(chain, 2);

	el = prnt_edt_type(chain, 3, PRNT_DEBG);
	el->message_text = "completed";

	prnt_msg_end(chain);

	return(0);
}
