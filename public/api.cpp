#include "api.h"
#include <stdio.h>
#include <ctype.h>

#define BE_game struct Game* game = (struct Game*)g

struct Game* test(struct Game* game) {

	return game;
}

size_t get_ranks() {

	return RANKS;
}

size_t get_files() {

	return FILES;
}

size_t get_state_length() {

	return STATE_LENGTH;
}

size_t get_trie_children() {

	return TRIE_CHILDREN;
}

Piece get_player(struct Game* game) {

	return GET_PLAYER(game->state);
}

size_t get_player_rank(struct Game* game) {

	return GET_PLAYER_RANK(game->state);
}

size_t get_player_file(struct Game* game) {

	return GET_PLAYER_FILE(game->state);
}

Piece get_cursor(struct Game* game) {

	return GET_CURSOR(game->state);
}

size_t get_cursor_rank(struct Game* game) {

	return GET_CURSOR_RANK(game->state);
}

size_t get_cursor_file(struct Game* game) {

	return GET_CURSOR_FILE(game->state);
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

const char* get_deck(struct Game* game, size_t d) {

	return deck(game, d);
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
	if (GAME_OVER(game->state)) return;
	game->time += passed;
	exist(game, falls);
	if (falls > 0) game->fell = game->time;
}

bool game_over(struct Game* game) {

	return GAME_OVER(game->state);
}

void delete_game(struct Game* game) {

	free_game(game);
}