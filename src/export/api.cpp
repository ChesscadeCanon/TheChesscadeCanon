#include "api.h"
#include "../../rules.h"
#include <stdio.h>
#include <ctype.h>

const char* get_rules() {

	return RULES;
}

size_t get_rules_length() {

	return RULES_LENGTH;
}

size_t get_ease(struct Game* game) {

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

	return next_piece(game);
}

size_t forecast_captures(struct Game* game) {

	return attack(game, false, true, false);
}

size_t attack_pattern(struct Game* game) {

	return attack(game, false, false, true);
}

size_t get_forecast_rank(struct Game* game) {

	return forecast_rank(game);
}

char get_forecast_piece(struct Game* game) {

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

	begin(game);
}

void increment_game(struct Game* game, time_t passed) {

	const time_t falls = COUNT_INTERVALS(game->fell, game->time, ease(game));
	if (game_over(game) || game->paused) return;
	game->time += passed;
	exist(game, falls);
	if (falls > 0) game->fell = game->time;
}

bool is_game_over(struct Game* game) {

	return game_over(game);
}

bool is_on_brink(struct Game* game) {

	return on_brink(game);
}

void delete_game(struct Game* game) {

	free_game(game);
}