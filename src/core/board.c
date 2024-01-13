#include "board.h"
#include <string.h>

const size_t PIECE_VALUES[SQUARE_COUNT] = {
	[PAWN] = 1,
	[BISHOP] = 3,
	[KNIGHT] = 3,
	[ROOK] = 5,
	[QUEEN] = 9,
	[KING] = 0,
	[NO_PIECE] = 0
};

const enum Square PIECE_MAP[128] = {
	[WHITE_PAWN] = PAWN,
	[BLACK_PAWN] = PAWN,
	[WHITE_BISHOP] = BISHOP,
	[BLACK_BISHOP] = BISHOP,
	[WHITE_ROOK] = ROOK,
	[BLACK_ROOK] = ROOK,
	[WHITE_KNIGHT] = KNIGHT,
	[BLACK_KNIGHT] = KNIGHT,
	[WHITE_QUEEN] = QUEEN,
	[BLACK_QUEEN] = QUEEN,
	[WHITE_KING] = KING,
	[BLACK_KING] = KING,
	[EMPTY] = NO_PIECE
};

const struct MoveSet MOVES[SQUARE_COUNT] = {
	[PAWN] = {
		.repeat = false,
		.count = 2,
		.moves = {
			{1, 1},
			{1, -1}
		}
	},
	[BISHOP] = {
		.repeat = true,
		.count = 4,
		.moves = {
			{1, 1},
			{-1, 1},
			{-1, -1},
			{1, -1}
		}
	},
	[ROOK] = {
		.repeat = true,
		.count = 4,
		.moves = {
			{0, 1},
			{-1, 0},
			{0, -1},
			{1, 0}
		}
	},
	[KNIGHT] = {
		.repeat = true,
		.count = 8,
		.moves = {
			{1, 2},
			{-1, 2},
			{-2, 1},
			{-2, -1},
			{-1, -2},
			{1, -2},
			{2, -1},
			{2, 1}
		}
	},
	[QUEEN] = {
		.repeat = true,
		.count = 8,
		.moves = {
			{1, 1},
			{-1, 1},
			{-1, -1},
			{1, -1},
			{0, 1},
			{-1, 0},
			{0, -1},
			{1, 0}
		}
	},
	[KING] = {
		.repeat = false,
		.count = 8,
		.moves = {
			{1, 1},
			{-1, 1},
			{-1, -1},
			{1, -1},
			{0, 1},
			{-1, 0},
			{0, -1},
			{1, 0}
		}
	}
};

void _init_squares(Board board) {

	for (size_t i = 0, r = 0; r < RANKS; ++r) {

		for (size_t f = 0; f < FILES; ++f) {

			board[i] = EMPTY;
			++i;
		}

		++i;
	}
}

void init_captures(Board board) {

	memcpy(board + BOARD_LENGTH, EMPTY_CAPTURES, CAPTURE_LENGTH * sizeof(char));
}

void init_board(Board board) {

	memset(board, '\n', STATE_LENGTH * sizeof(char));
	init_captures(board);
	_init_squares(board);
	TERMINATE(board);
}