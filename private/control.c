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

void _control_move(struct Game* game, long int* moved, const char key, const time_t passed) {

	if (GETKEYSTATE(key) < 0) {

		const bool was_up = *moved < 0;
		*moved = max(*moved, 0);
		*moved += (long int)passed;
	}
	else {

		*moved = -1;
	}
}

void _control_drop(struct Game* game) {

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

	_control_drop(game);
	if(game->paused) return;
	_control_move(game, &(game->moved_right), RIGHT_KEY, passed);
	_control_move(game, &(game->moved_left), LEFT_KEY, passed);
	_control_move(game, &(game->moved_down), DOWN_KEY, passed);
}


