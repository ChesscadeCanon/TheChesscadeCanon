#include "view.h"
#include "model.h"
#include "config.h"
#include "platform.h"
#include <stdio.h>

void _print_instructions() {
#if OS_WINDOWS
	printf("pause: %c\n", PAUSE_KEY);
	printf("quit: %c\n", QUIT_KEY);
	printf("drop: %c\n", DROP_KEY);
	printf("left, down, right: %c, %c, %c\n", LEFT_KEY, DOWN_KEY, RIGHT_KEY);
#else
	printf("pause: %c\n\r", PAUSE_KEY);
	printf("quit: %c\n\r", QUIT_KEY);
	printf("drop: %c\n\r", DROP_KEY);
	printf("left, down, right: %c, %c, %c\n\r", LEFT_KEY, DOWN_KEY, RIGHT_KEY);
#endif
}

void _print_info(struct Game* game) {
#if OS_WINDOWS
	printf("score %llu\n", game->score);
	printf("combo %llu\n", game->combo);
	printf("repeat %d\n", game->repeat);
	printf("time %llu\n", game->time);
#else
	printf("score %lu\n\r", game->score);
	printf("combo %lu\n\r", game->combo);
	printf("repeat %d\n\r", game->repeat);
	printf("time %lu\n\r", game->time);
#endif
}

void _print_cursor(struct Game* game) {

	const bool wrapped = cursor_wrapped(game);
	const size_t rank = game->cursor_rank;
	const size_t file = game->cursor_file;
	const char* d = deck(rank);
	char next = d[file];
	next = wrapped ? IS_WHITE(next) ? 'K' : 'k' : next;
#if OS_WINDOWS
	char cursor[FOUR_LINES + 1] = {
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'\0'
	};
#else
	char cursor[FOUR_LINES + 5] = {
		'*', '*', '*', '*', '*', '*', '*', '*', '\n', '\r',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n', '\r',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n', '\r',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n', '\r',
		'\0'
	};
#endif
	cursor[rank * (LINE_LENGTH + !OS_WINDOWS) + file] = next;
	printf("%s", cursor);
}

void print_raw(struct Game* game) {
#if OS_WINDOWS
	printf("score %llu\n", game->score);
	printf("time %llu\n", game->time);
	printf("fell %llu\n", game->fell);
	printf("left %ld\n", game->moved_left);
	printf("right %ld\n", game->moved_right);
	printf("down %ld\n", game->moved_down);
#else
	printf("score %lu\n\r", game->score);
	printf("time %lu\n\r", game->time);
	printf("fell %lu\n\r", game->fell);
	printf("left %ld\n\r", game->moved_left);
	printf("right %ld\n\r", game->moved_right);
	printf("down %ld\n\r", game->moved_down);
#endif
	printf("%s", game->state);
}

void print_pretty(struct Game* game) {
#if OS_WINDOWS
	char board[STATE_LENGTH] = {[STATE_LENGTH - 1] = '\0'};
	memcpy(board, game->state, STATE_LENGTH * sizeof(char));
	SET_SQUARE(board, PLAYER_SQUARE(game), game->player);
#else
	char board[CURSES_STATE_LENGTH];
	size_t to_index = 0, from_index = 0;
	memset(board, '\n', CURSES_STATE_LENGTH * sizeof(char));
	for(size_t i = 1; i < CURSES_STATE_LENGTH; i += 2) board[i] = '\r';
	board[CURSES_STATE_LENGTH - 1] = '\0';
	while(game->state[from_index]) {
		
		if(game->state[from_index] == '\n') {
			
			board[to_index++] = '\r';
		}

		board[to_index++] = game->state[from_index++];
	}
	const size_t r = game->player_rank, f = game->player_file;
	board[(r * (FILES + 2)) + f] = game->player;
#endif
	_print_instructions();
	_print_info(game);
	_print_cursor(game);
	printf("%s", board);
}