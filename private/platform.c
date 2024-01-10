#include "platform.h"
#if OS_WINDOWS
void QUIT() {
	printf("\a");
	exit(0);
}
#else
void QUIT() {
	printf("\a");
	endwin();
	exit(0);
}

int GETKEYSTATE(const int C) {

	const int c = GETCH();
	const int ret = toupper(c) == C ? -1 : 1;
	if(ret != ERR) ungetch(c);
	return ret;
}
#endif