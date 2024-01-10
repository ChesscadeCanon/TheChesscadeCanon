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

void _add_move(long int* moved, const time_t passed) {

	const bool was_up = *moved < 0;
	*moved = max(*moved, 0);
	*moved += (long int)passed;
}

void _control_move(long int* moved, const char key, const time_t passed) {

	if (GETKEYSTATE(key) < 0) {

		_add_move(moved, passed);
	}
	else {

		*moved = -1;
	}
}

#if OS_WINDOWS
void _control_tap(struct Game* game) {

	if (KBHIT()) {

		const char key = GETCH();
		switch (key) {
		case DROP_KEY: if (!game->paused) game->dropped = true; break;
		case QUIT_KEY: QUIT(); break;
		case PAUSE_KEY: game->paused = !game->paused; break;
		default: break;
		}
	}
}
#endif

void key_control(struct Game* game, const time_t passed) {

#if OS_WINDOWS
	_control_tap(game);
	if(game->paused) return;
	_control_move(&(game->moved_right), RIGHT_KEY, passed);
	_control_move(&(game->moved_left), LEFT_KEY, passed);
	_control_move(&(game->moved_down), DOWN_KEY, passed);
#else
	const char key = GETCH();
	switch (key) {
	case DROP_KEY: if (!game->paused) game->dropped = true; break;
	case QUIT_KEY: QUIT(); break;
	case PAUSE_KEY: game->paused = !game->paused; break;
	case RIGHT_KEY: _add_move(&(game->moved_right), passed); break;
	case LEFT_KEY: _add_move(&(game->moved_left), passed); break;
	case DOWN_KEY: _add_move(&(game->moved_down), passed); break;
	//case ERR: game->moved_right = game->moved_left = game->moved_down = -1;
	default: break;
	}

	switch (key) {
	case RIGHT_KEY:
	case LEFT_KEY:
	case DOWN_KEY:
		UNGETCH(key);
	default: break;
	}
#endif
}


