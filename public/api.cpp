#include "api.h"
#include <stdio.h>
#include <ctype.h>

#define BE_game struct Game* game = (struct Game*)g

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

Piece get_player(NOTAGAME g) {

	BE_game;
	return GET_PLAYER(game->state);
}

size_t get_player_rank(NOTAGAME g) {

	BE_game;
	return GET_PLAYER_RANK(game->state);
}

size_t get_player_file(NOTAGAME g) {

	BE_game;
	return GET_PLAYER_FILE(game->state);
}

Piece get_cursor(NOTAGAME g) {

	BE_game;
	return GET_CURSOR(game->state);
}

size_t get_cursor_rank(NOTAGAME g) {

	BE_game;
	return GET_CURSOR_RANK(game->state);
}

size_t get_cursor_file(NOTAGAME g) {

	BE_game;
	return GET_CURSOR_FILE(game->state);
}

Piece get_next_piece(NOTAGAME g) {

	BE_game;
	return next_piece(game);
}

struct Game* malloc_init_default_game() {

	struct Game* ret = malloc_init_game(DEFAULT_SETTINGS);
	return ret;
}

void begin_game(NOTAGAME g) {

	BE_game;
	begin(game);
}

void increment_game(NOTAGAME g, time_t passed) {

	BE_game;
	const time_t falls = COUNT_INTERVALS(game->fell, game->time, get_ease(game));
	if (GAME_OVER(game->state)) return;
	game->time += passed;
	exist(game, falls);
	if (falls > 0) game->fell = game->time;
}

bool game_over(NOTAGAME g) {

	BE_game;
	return GAME_OVER(game->state);
}

void delete_game(NOTAGAME g) {

	BE_game;
	free_game(game);
}

void test() {

	struct Game* game = malloc_init_game(DEFAULT_SETTINGS);
	free_game(game);
}