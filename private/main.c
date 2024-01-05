#include "main.h"
#include "model.h"
#include "control.h"
#include "view.h"
#include <stdio.h>
#include <assert.h>

void default_view(struct Game* game) {

	print_raw(game);
}

void pretty_view(struct Game* game) {

	print_pretty(game);
}

void default_control(struct Game* game, const time_t passed) {

	key_control(game, passed);
}

void tock(struct Game* game, struct timeb* then, struct timeb* now) {

	memcpy(then, now, sizeof(struct timeb));

#if OS_WINDOWS
	Sleep(MPF);
	system("cls");
#else
	sleep(SPF);
	system("clear");
#endif
}

void default_model(struct Game* game, const time_t falls) {

	exist(game, falls);
}

bool tick(struct Game* game, const time_t passed, CONTROL_FUNCTOR(control), MODEL_FUNCTOR(model), VIEW_FUNCTOR(view)) {

	const time_t falls = COUNT_INTERVALS(game->fell, game->time, ease(game));;

	game->time += passed * !game->paused;
	control(game, passed);
	if (!game->paused) model(game, falls);
	if (GAME_OVER(game->state)) return true;
	view(game);
	if (falls > 0) game->fell = game->time;

	return false;
}

void play(struct Game* game, struct timeb* then, CONTROL_FUNCTOR(control), MODEL_FUNCTOR(model), VIEW_FUNCTOR(view)) {

	struct timeb now;
	ftime(&now);
	const time_t passed = MILLISECONDS_DIFFERENCE((*then), (now));

	if (tick(game, passed, control, model, view)) return;
	tock(game, then, &now);
	play(game, then, control, model, view);
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

void run_pretty_text_game(size_t settings) {

	struct Game* game = malloc_init_game(settings);

#if OS_WINDOWS
#else
	initscr();
	nodelay(stdscr, TRUE);
#endif
	play_pretty(game);
	free_game(game);
#if OS_WINDOWS
#else
	endwin();
#endif
}

int main(int argc, char** argv) {

	run_pretty_text_game(DEFAULT_SETTINGS);
	return 0;
}
