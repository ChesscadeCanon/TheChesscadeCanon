#include "application.h"
#include "platform.h"
#include "game.h"
#include "control.h"
#include "view.h"
#include "main.h"
#include "simulator.h"
#include <stdio.h>
#include <assert.h>
#include <time.h>

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

void auto_control(struct Game* game, const Time passed) {

	automate(game);
}

void tock(struct Game* game, struct timespec* then, struct timespec* now) {

	memcpy(then, now, sizeof(struct timespec));
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

void play(struct Game* game, struct timespec* then, CONTROL_FUNCTOR(control), MODEL_FUNCTOR(model), VIEW_FUNCTOR(view)) {

	while (True) {

		struct timespec now;
		assert(timespec_get(&now, TIME_UTC) == TIME_UTC);
		const Time passed = MILLISECONDS_DIFFERENCE(*then, now);
		if (tick(game, passed, control, model, view)) return;
		tock(game, then, &now);
	}
}

void play_default(struct Game* game) {

	struct timespec start;
	assert(timespec_get(&start, TIME_UTC) == TIME_UTC);
	begin(game);
	play(game, &start, default_control, default_model, default_view);
}

void play_pretty(struct Game* game) {

	struct timespec start;
	assert(timespec_get(&start, TIME_UTC) == TIME_UTC);
	begin(game);
	play(game, &start, default_control, default_model, pretty_view);
}

void play_pretty_auto(struct Game* game) {

	struct timespec start;
	assert(timespec_get(&start, TIME_UTC) == TIME_UTC);
	begin(game);
	play(game, &start, auto_control, default_model, pretty_view);
}

void run_pretty_text_game(Settings settings) {

	struct Game* game = malloc_init_game(settings);
	play_pretty(game);
	free_game(game);
}

void run_pretty_auto_text_game(Settings settings) {

	struct Game* game = malloc_init_game(settings);
	play_pretty_auto(game);
	free_game(game);
}

