extern "C" {
#include "api.h"
#include "game.h"
#include "../../rules.h"
#include <assert.h>
#include <string.h>

#define ASSERT_GAME(__game__) assert(static_cast<struct Game*>(__game__))

void input_toggle_pause(struct Game* game) {

	ASSERT_GAME(game);
	toggle_pause(game);
}

void input_drop(struct Game* game) {

	ASSERT_GAME(game);
	do_drop(game);
}

void input_digital_move(struct Game* game, bool left, bool right, bool down) {

	ASSERT_GAME(game);
	do_digital_move(game, left, right, down);
}

void input_analog_move(struct Game* game, long double x, long double y) {

	ASSERT_GAME(game);
	do_analog_move(game, x, y);
}

Events get_events(struct Game* game) {

	ASSERT_GAME(game);
	return current_events(game);
}

const char* get_rules() {

	return RULES;
}

Count get_rules_length() {

	return RULES_LENGTH;
}

Time get_ease(struct Game* game) {

	ASSERT_GAME(game);
	return ease(game);
}

Index get_ranks() {

	return RANKS;
}

Index get_files() {

	return FILES;
}

Piece get_square(struct Game* game, Index rank, Index file) {

	ASSERT_GAME(game);
	return square_contents(game, rank, file);
}

Piece get_next_piece(struct Game* game) {

	ASSERT_GAME(game);
	return next_piece(game);
}

Piece get_player(struct Game* game) {

	ASSERT_GAME(game);
	return player_piece(game);
}

Index get_player_rank(struct Game* game) {

	ASSERT_GAME(game);
	return player_piece_rank(game);
}

Index get_player_file(struct Game* game) {

	ASSERT_GAME(game);
	return player_piece_file(game);
}

Index get_cursor_grade(struct Game* game) {

	ASSERT_GAME(game);
	return current_cursor_grade(game);
}

Index get_cursor_increment(struct Game* game) {

	ASSERT_GAME(game);
	return current_cursor_increment(game);
}

Count get_score(struct Game* game) {

	ASSERT_GAME(game);
	return current_score(game);
}

Count get_scored(struct Game* game) {

	ASSERT_GAME(game);
	return last_scored(game);
}

Count get_combo(struct Game* game) {

	ASSERT_GAME(game);
	return current_combo(game);
}

Trenary get_cursor_direction(struct Game* game) {

	ASSERT_GAME(game);
	return cursor_direction(game);
}

Set forecast_captures(struct Game* game) {

	ASSERT_GAME(game);
	return attack(game, false, True, false);
}

Set attack_pattern(struct Game* game) {

	ASSERT_GAME(game);
	return attack(game, false, false, True);
}

Index get_forecast_rank(struct Game* game) {

	ASSERT_GAME(game);
	return forecast_rank(game);
}

char get_forecast_piece(struct Game* game) {

	ASSERT_GAME(game);
	return forecast_piece(game);
}

Set get_square_bit(Index rank, Index file) {

	return square_bit(rank, file);
}

Piece get_deck_piece(Index grade, Index increment) {

	const char* d = deck(grade);
	assert(d[FILES] == '\0');
	assert(strlen(d) == FILES);
	return d[increment];
}

struct Game* malloc_init_default_game() {

	struct Game* ret = malloc_init_game(DEFAULT_SETTINGS);
	ASSERT_GAME(ret);
	return ret;
}

void print_board(Game* game) {

	ASSERT_GAME(game);
	print_board_state(game);
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

void pump_game(struct Game* game, Time passed) {

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
	return paused(game);
}

bool is_repeat(struct Game* game) {

	ASSERT_GAME(game);
	return repeated(game);
}

void delete_game(struct Game* game) {

	ASSERT_GAME(game);
	free_game(game);
}
} // extern "C"