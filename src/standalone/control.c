#include "control.h"
#include "game.h"
#include "platform.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

enum Input {
	NO_INPUT = -1,
	DROP_INPUT,
	DOWN_INPUT,
	LEFT_INPUT,
	RIGHT_INPUT,
};

void _control_move(bool* moved, const char key, const time_t passed) {

	*moved = GetKeyState(key) < 0;
}

void _control_tap(struct Game* game) {

	if (_kbhit()) {

		const char key = _getch();
		switch (key) {
		case DROP_KEY: if (!game->pause) game->dropped = true; break;
		case QUIT_KEY: exit(0); break;
		case PAUSE_KEY: {

			if (!game->pause) print_rules();
			game->pause = !game->pause;
			break;
		}
		default: break;
		}
	}
}

void key_control(struct Game* game, const time_t passed) {

	_control_tap(game);
	if(game->pause) return;
	_control_move(&(game->moved_right), RIGHT_KEY, passed);
	_control_move(&(game->moved_left), LEFT_KEY, passed);
	_control_move(&(game->moved_down), DOWN_KEY, passed);
}


