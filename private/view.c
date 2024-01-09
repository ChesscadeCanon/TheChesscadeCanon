#include "view.h"
#include "model.h"
#include "platform.h"
#include <stdio.h>

void print_info(struct Game* game) {
#if OS_WINDOWS
	printf("score %llu\n", game->score);
	printf("time %llu\n", game->time);
#else
	printf("score %llu\n\r", game->score);
	printf("time %llu\n\r", game->time);
#endif
}

void print_raw(struct Game* game) {
#if OS_WINDOWS
	printf("score %llu\n", game->score);
	printf("time %llu\n", game->time);
	printf("fell %llu\n", game->fell);
	printf("left %d\n", game->moved_left);
	printf("right %d\n", game->moved_right);
	printf("down %d\n", game->moved_down);
#else
	printf("score %llu\n\r", game->score);
	printf("time %llu\n\r", game->time);
	printf("fell %llu\n\r", game->fell);
	printf("left %d\n\r", game->moved_left);
	printf("right %d\n\r", game->moved_right);
	printf("down %d\n\r", game->moved_down);
#endif
	printf(game->state);
}

void print_cursor(struct Game* game) {

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
	printf(cursor);
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
	const size_t r = GET_PLAYER_RANK(game->state), f = GET_PLAYER_FILE(game->state);
	board[(r * (FILES + 2)) + f] = GET_PLAYER(game->state);
#endif
	print_info(game);
	print_cursor(game);
	printf((const char*)board);
}
