#include "view.h"
#include "game.h"
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

void _print_info(const struct Game* game) {
	printf("score %llu\n", current_score(game));
	printf("combo %llu\n", current_combo(game));
	printf("repeat %d\n", repeated(game));
	printf("time %llu\n", milliseconds(game));
}

void _print_cursor(const struct Game* game) {

	const Bool wrapped = cursor_wrapped(game);
	const Index grade = current_cursor_grade(game);
	const Index increment = current_cursor_increment(game);
	char next = deck(game, grade, increment);
	next = wrapped ? IS_WHITE(next) ? 'K' : 'k' : next;
	char cursor[FOUR_LINES + 1] = {
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'*', '*', '*', '*', '*', '*', '*', '*', '\n',
		'\0'
	};
	cursor[grade * (LINE_LENGTH) + increment] = next;
	printf("%s", cursor);
}

void print_raw(const struct Game* game) {

	printf("score %llu\n", current_score(game));
	printf("time %llu\n", milliseconds(game));
	printf("%s", board_state(game));
}

void print_pretty(const struct Game* game) {

	Index index = SQUARE_INDEX(player_piece_rank(game), player_piece_file(game));
	char board[BOARD_LENGTH] = {[BOARD_LENGTH - 1] = '\0'};
	memcpy(board, board_state(game), BOARD_LENGTH * sizeof(char));
	SET_SQUARE(board, index, player_piece(game));
	_print_instructions();
	_print_info(game);
	_print_cursor(game);
	printf("%s", board);
	printf("%s\n", current_path(game));
}
