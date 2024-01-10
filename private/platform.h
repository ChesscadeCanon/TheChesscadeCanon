#pragma once
#include <string.h>
#include <stdio.h>

#ifdef __unix__                    /* __unix__ is usually defined by compilers targeting Unix systems */

#define OS_WINDOWS 0
#define max(A, B) (A > B ? A : B)
#define min(A, B) (A < B ? A : B)
#include <unistd.h>
#include <ncurses.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#define GETCH getch
#define UNGETCH ungetch
inline void QUIT() {
	printf("\a");
	endwin();
	exit(0);
}
inline int KBHIT(void)
{
	int ch = GETCH();

	if (ch != ERR) {
		ungetch(ch);
		return 1;
	}
	else {
		return 0;
	}
}
inline int GETKEYSTATE(const int C) {

	const int c = GETCH();
	const int ret = toupper(c) == C ? -1 : 1;
	ungetch(c);
	return ret;
}
#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */
#define OS_WINDOWS 1
#include <windows.h>
#include <WinUser.h>
#include <conio.h>
#define KBHIT _kbhit
#define GETCH _getch
#define GETKEYSTATE GetKeyState
#define UNGETCH(K) ; 
inline void QUIT() {
	printf("\a");
	exit(0);
}
#endif