#include "control.h"
#include "game.h"
#include "platform.h"
#include "config.h"

enum Input {
	NO_INPUT = -1,
	DROP_INPUT,
	DOWN_INPUT,
	LEFT_INPUT,
	RIGHT_INPUT,
};

void _control_tap(struct Game* game) {

	if (_kbhit()) {

		const char key = _getch();
		switch (key) {
		case DROP_KEY: if (!paused(game)) do_drop(game); break;
		case QUIT_KEY: exit(0); break;
		case PAUSE_KEY: {

			if (!paused(game)) print_rules();
			toggle_pause(game);
			break;
		}
		default: break;
		}
	}
}

void key_control(struct Game* game, const Time passed) {

	const Bool left = GetKeyState(LEFT_KEY) < 0;
	const Bool right = GetKeyState(RIGHT_KEY) < 0;
	const Bool down = GetKeyState(DOWN_KEY) < 0;
	_control_tap(game);
	if(paused(game)) return;
	do_digital_move(game, left, right, down);
}


