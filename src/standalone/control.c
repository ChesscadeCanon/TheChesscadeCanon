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

	*moved = GETKEYSTATE(key) < 0;
}

#if OS_WINDOWS
void _control_tap(struct Game* game) {

	if (KBHIT()) {

		const char key = GETCH();
		switch (key) {
		case DROP_KEY: if (!game->paused) game->dropped = true; break;
		case QUIT_KEY: QUIT(); break;
		case PAUSE_KEY: {

			if (!game->paused) print_rules();
			game->paused = !game->paused;
			break;
		}
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
	case ERR: game->moved_right = game->moved_left = game->moved_down = -1;
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


