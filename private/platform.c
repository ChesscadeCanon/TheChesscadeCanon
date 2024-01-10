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

int KBHIT() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (!initialized) {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

int GETKEYSTATE(const int C) {

	const int c = GETCH();
	const int ret = toupper(c) == C ? -1 : 1;
	if(ret != ERR) ungetch(c);
	return ret;
}
#endif