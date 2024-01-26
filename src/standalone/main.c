#include "platform.h"
#include "game.h"
#include "control.h"
#include "view.h"
#include <stdio.h>
#include <assert.h>
#include <sys/timeb.h>

#define MILLISECONDS_DIFFERENCE(__from_time__, __to_time__) ((Time)(1000.0 * ((__to_time__).time - (__from_time__).time) + ((__to_time__).millitm - (__from_time__).millitm)))
#define VIEW_FUNCTOR(VAR) void (*VAR)(struct Game*)
#define MODEL_FUNCTOR(VAR) void (*VAR)(struct Game*, const Time)
#define CONTROL_FUNCTOR(VAR) void (*VAR)(struct Game*, const Time)

void default_view(struct Game* game) {

	print_raw(game);
}

void pretty_view(struct Game* game) {

	print_pretty(game);
}

void default_control(struct Game* game, const Time passed) {

	key_control(game, passed);
}

void tock(struct Game* game, struct timeb* then, struct timeb* now) {

	memcpy(then, now, sizeof(struct timeb));
	Sleep(MPF);
	if(!paused(game)) system("cls");
}

void default_model(struct Game* game, const Time passed) {

	pump(game, passed);
}

Bool tick(struct Game* game, const Time passed, CONTROL_FUNCTOR(control), MODEL_FUNCTOR(model), VIEW_FUNCTOR(view)) {

	control(game, passed);

	if (!paused(game)) {
		model(game, passed);
		if (game_over(game)) return True;
		view(game);
	}

	return False;
}

void play(struct Game* game, struct timeb* then, CONTROL_FUNCTOR(control), MODEL_FUNCTOR(model), VIEW_FUNCTOR(view)) {

	while (True) {

		struct timeb now;
		ftime(&now);
		const Time passed = MILLISECONDS_DIFFERENCE(*then, now);
		if (tick(game, passed, control, model, view)) return;
		tock(game, then, &now);
	}
}

void play_default(struct Game* game) {

	struct timeb start;
	ftime(&start);
	begin(game);
	play(game, &start, default_control, default_model, default_view);
}

void play_pretty(struct Game* game) {

	struct timeb start;
	ftime(&start);
	begin(game);
	play(game, &start, default_control, default_model, pretty_view);
}

void run_pretty_text_game(Settings settings) {

	struct Game* game = malloc_init_game(settings);
	play_pretty(game);
	free_game(game);
}

int main(int argc, char** argv) {

	run_pretty_text_game(DEFAULT_SETTINGS);
	return 0;
}
