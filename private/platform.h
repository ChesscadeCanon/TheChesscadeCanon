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
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#define GETCH getch
#define UNGETCH ungetch
void QUIT();
int KBHIT(void);
int GETKEYSTATE(const int C);
#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */
#define OS_WINDOWS 1
#include <windows.h>
#include <WinUser.h>
#include <conio.h>
#define KBHIT _kbhit
#define GETCH _getch
#define GETKEYSTATE GetKeyState
#define UNGETCH(K) ; 
void QUIT();
#endif