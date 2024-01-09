#include "control.h"
#include "model.h"
#include "platform.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define DROP_KEY ' '
#define QUIT_KEY 'q'
#define PAUSE_KEY 'p'
#define LEFT_KEY 'A'
#define RIGHT_KEY 'D'
#define DOWN_KEY 'S'

enum Input {
	NO_INPUT = -1,
	DROP_INPUT,
	DOWN_INPUT,
	LEFT_INPUT,
	RIGHT_INPUT,
};

#if OS_WINDOWS
#include <windows.h>
#include <WinUser.h>
#include <conio.h>
#define KBHIT _kbhit
#define GETCH _getch
#define GETKEYSTATE GetKeyState
#define UNGETCH(K) ; 
void QUIT() {
	printf("\a");
	exit(0);
}
#else
#include <unistd.h>
#include <ncurses.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#define GETCH getch
#define UNGETCH ungetch
void QUIT() {
	printf("\a");
	endwin();
	exit(0);
}
int KBHIT(void)
{
    int ch = GETCH();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}
int GETKEYSTATE(const int C) {

	const int c = GETCH();
	const int ret = toupper(c) == C ? -1 : 1;
	ungetch(c);
	return ret;
}
#endif

void control_move(struct Game* game, long int* moved, const char key, const time_t passed) {

	if (GETKEYSTATE(key) < 0) {

		const bool was_up = *moved < 0;
		*moved = max(*moved, 0);
		*moved += (long int)passed;
	}
	else {

		*moved = -1;
	}
}

void control_drop(struct Game* game) {

	if (KBHIT()) {

		const char key = GETCH();
		switch (key) {
		case DROP_KEY: if (!game->paused) game->dropped = true; break;
		case QUIT_KEY: QUIT(); break;
		case PAUSE_KEY: game->paused = !game->paused; break;
		default: UNGETCH(key); break;
		}
	}
}

void key_control(struct Game* game, const time_t passed) {

	control_drop(game);
	if(game->paused) return;
	control_move(game, &(game->moved_right), RIGHT_KEY, passed);
	control_move(game, &(game->moved_left), LEFT_KEY, passed);
	control_move(game, &(game->moved_down), DOWN_KEY, passed);
}


