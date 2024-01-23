#include "api.h"
#include "../../rules.h"
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

void input_toggle_pause(struct Game* game) {

	assert(game);
	game->pause = !game->pause;
}

const char* get_rules() {

	return RULES;
}

size_t get_rules_length() {

	return RULES_LENGTH;
}

size_t get_ease(struct Game* game) {

	assert(game);
	return ease(game);
}

size_t get_ranks() {

	return RANKS;
}

size_t get_files() {

	return FILES;
}

size_t get_board_length() {

	return BOARD_LENGTH;
}

size_t get_symbol_count() {

	return SYMBOL_COUNT;
}

Piece get_next_piece(struct Game* game) {

	assert(game);
	return next_piece(game);
}

size_t forecast_captures(struct Game* game) {

	assert(game);
	return attack(game, false, true, false);
}

size_t attack_pattern(struct Game* game) {

	assert(game);
	return attack(game, false, false, true);
}

size_t get_forecast_rank(struct Game* game) {

	assert(game);
	return forecast_rank(game);
}

char get_forecast_piece(struct Game* game) {

	assert(game);
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
	return ret;
}

void begin_game(struct Game* game) {

	assert(game);
	begin(game);
}

void pump_game(struct Game* game, time_t passed) {

	assert(game);
	pump(game, passed);
}

bool is_game_over(struct Game* game) {

	assert(game);
	return game_over(game);
}

bool is_on_brink(struct Game* game) {

	assert(game);
	return on_brink(game);
}

bool is_paused(struct Game* game) {

	assert(game);
	return game->pause;
}

void delete_game(struct Game* game) {

	assert(game);
	free_game(game);
}