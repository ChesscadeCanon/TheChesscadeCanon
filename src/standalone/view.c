#include "view.h"
#include "game.h"
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
	printf("score %llu\n", game->score);
	printf("combo %llu\n", game->combo);
	printf("repeat %d\n", game->repeat);
	printf("time %llu\n", game->time);
}

void _print_cursor(struct Game* game) {

	const bool wrapped = cursor_wrapped(game);
	const Index rank = game->cursor_grade;
	const Index file = game->cursor_increment;
	const char* d = deck(rank);
	char next = d[file];
	next = wrapped ? IS_WHITE(next) ? 'K' : 'k' : next;
	char cursor[FOUR_LINES + 1] = {
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'\0'
	};
	cursor[rank * (LINE_LENGTH) + file] = next;
	printf("%s", cursor);
}

void print_raw(struct Game* game) {

	printf("score %llu\n", game->score);
	printf("time %llu\n", game->time);
	printf("last moved %llu\n", game->last_moved);
	printf("left %d\n", game->moved_left);
	printf("right %d\n", game->moved_right);
	printf("down %d\n", game->moved_down);
	printf("%s", game->board);
}

void print_pretty(struct Game* game) {
#if OS_WINDOWS
	char board[BOARD_LENGTH] = {[BOARD_LENGTH - 1] = '\0'};
	memcpy(board, game->board, BOARD_LENGTH * sizeof(char));
	SET_SQUARE(board, PLAYER_SQUARE(game), game->player);
#else
	char board[CURSES_STATE_LENGTH];
	size_t to_index = 0, from_index = 0;
	memset(board, '\n', CURSES_STATE_LENGTH * sizeof(char));
	for(size_t i = 1; i < CURSES_STATE_LENGTH; i += 2) board[i] = '\r';
	board[CURSES_STATE_LENGTH - 1] = '\0';
	while(game->board[from_index]) {
		
		if(game->board[from_index] == '\n') {
			
			board[to_index++] = '\r';
		}

		board[to_index++] = game->board[from_index++];
	}
	const size_t r = game->player_rank, f = game->player_file;
	board[(r * (FILES + 2)) + f] = game->player;
#endif
	_print_instructions();
	_print_info(game);
	_print_cursor(game);
	printf("%s", board);
}
