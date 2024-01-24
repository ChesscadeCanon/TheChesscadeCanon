#include "api.h"
#include "../../rules.h"
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

#define ASSERT_GAME(__game__) assert(dynamic_cast<struct Game*>(__game__))

void input_toggle_pause(struct Game* game) {

	ASSERT_GAME(game);
	game->pause = !game->pause;
}

void input_drop(struct Game* game) {

	ASSERT_GAME(game);
	game->dropped = true;
}

void input_digital_move(struct Game* game, bool left, bool right, bool down) {

	ASSERT_GAME(game);
	game->moved_left = left;
	game->moved_right = right;
	game->moved_down = down;
}

void input_analog_move(struct Game* game, long double x, long double y) {

	ASSERT_GAME(game);

	if (x > 0) {

		game->dragged_left = 0;
		game->dragged_right += x;
	}
	else if (x < 0) {

		game->dragged_left -= x;
		game->dragged_right = 0;
	}
	else {

		game->dragged_left = 0;
		game->dragged_right = 0;
	}

	if (y > 0) {

		game->dragged_down += y;
	}
	else {

		game->dragged_down = 0;
	}
}

Events get_events(struct Game* game) {

	ASSERT_GAME(game);
	return game->events;
}

const char* get_rules() {

	return RULES;
}

size_t get_rules_length() {

	return RULES_LENGTH;
}

size_t get_ease(struct Game* game) {

	ASSERT_GAME(game);
	return ease(game);
}

size_t get_ranks() {

	return RANKS;
}

size_t get_files() {

	return FILES;
}

char* get_board(struct Game* game) {

	ASSERT_GAME(game);
	assert(game->board[BOARD_LENGTH - 1] == '\0');
	return game->board;
}

size_t get_board_length() {

	return BOARD_LENGTH;
}

size_t get_symbol_count() {

	return SYMBOL_COUNT;
}

Piece get_next_piece(struct Game* game) {

	ASSERT_GAME(game);
	return next_piece(game);
}

Piece get_player(struct Game* game) {

	ASSERT_GAME(game);
	return game->player;
}

Index get_player_rank(struct Game* game) {

	ASSERT_GAME(game);
	return game->player_rank;
}

Index get_player_file(struct Game* game) {

	ASSERT_GAME(game);
	return game->player_file;
}

Index get_cursor_grade(struct Game* game) {

	ASSERT_GAME(game);
	return game->cursor_grade;
}

Index get_cursor_increment(struct Game* game) {

	ASSERT_GAME(game);
	return game->cursor_increment;
}

size_t get_score(struct Game* game) {

	ASSERT_GAME(game);
	return game->score;
}

size_t get_scored(struct Game* game) {

	ASSERT_GAME(game);
	return game->scored;
}

size_t get_combo(struct Game* game) {

	ASSERT_GAME(game);
	return game->combo;
}

size_t forecast_captures(struct Game* game) {

	ASSERT_GAME(game);
	return attack(game, false, true, false);
}

size_t attack_pattern(struct Game* game) {

	ASSERT_GAME(game);
	return attack(game, false, false, true);
}

size_t get_forecast_rank(struct Game* game) {

	ASSERT_GAME(game);
	return forecast_rank(game);
}

char get_forecast_piece(struct Game* game) {

	ASSERT_GAME(game);
	return forecast_piece(game);
}

size_t get_square_bit(size_t rank, size_t file) {

	return square_bit(rank, file);
}

const char* get_deck(size_t d) {

	return deck(d);
}

struct Game* malloc_init_default_game() {

	struct Game* ret = malloc_init_game(DEFAULT_SETTINGS);
	ASSERT_GAME(ret);
	return ret;
}

struct Game* malloc_init_custom_game(Settings settings) {

	struct Game* ret = malloc_init_game(settings);
	ASSERT_GAME(ret);
	return ret;
}

void begin_game(struct Game* game) {

	ASSERT_GAME(game);
	begin(game);
}

void pump_game(struct Game* game, time_t passed) {

	ASSERT_GAME(game);
	pump(game, passed);
}

bool is_game_over(struct Game* game) {

	ASSERT_GAME(game);
	return game_over(game);
}

bool is_on_brink(struct Game* game) {

	ASSERT_GAME(game);
	return on_brink(game);
}

bool is_paused(struct Game* game) {

	ASSERT_GAME(game);
	return game->pause;
}

bool is_repeat(struct Game* game) {

	ASSERT_GAME(game);
	return game->repeat;
}

void delete_game(struct Game* game) {

	ASSERT_GAME(game);
	free_game(game);
}