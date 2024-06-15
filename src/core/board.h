#pragma once
#include "def.h"

#define CAPTURE_LENGTH 9
#define EMPTY_CAPTURES "********"
#define HAS_CAPTURED(__captures__) (strncmp(EMPTY_CAPTURES, __captures__, CAPTURE_LENGTH) != 0)
#define LINE_LENGTH (FILES + 1)
#define SQUARE_RANK(__index__) ((Index)(__index__ / LINE_LENGTH))
#define SQUARE_FILE(__index__) ((Index)(__index__ % LINE_LENGTH))
#define SQUARE_BIT(__index__) (1ull << (Set)(SQUARE_RANK(__index__) * FILES + SQUARE_FILE(__index__)))
#define SET_CAPTURE(__captures__, __piece_index__, __value__) (__captures__[__piece_index__] = __value__)
#define SET_SQUARE(__board__, __index__, __value__) (__board__[__index__] = __value__)
#define IS_WHITE(__piece__) (__piece__ <= 'Z')
#define INVERT_PIECE(__piece__) (__piece__ + (IS_WHITE(__piece__) ? 1 : -1) * ('a' - 'A'))
#define SQUARE_INDEX(__rank__, __file__) ((__rank__) * LINE_LENGTH + (__file__))
#define FOUR_LINES (LINE_LENGTH * 4)
#define BOARD_LENGTH (LINE_LENGTH * RANKS + 1)
#define CURSES_STATE_LENGTH (BOARD_LENGTH + RANKS)
#define TERMINATOR_INDEX (BOARD_LENGTH - 1)
#define TERMINATE(__board__) (__board__[TERMINATOR_INDEX] = '\0')
#define GET_SQUARE(__board__, __index__) __board__[__index__]
#define GET_CAPTURE(__captures__, __index__) (__captures__[__index__])
#define EMPTY_SQUARE(__board__, __index__) (GET_SQUARE(__board__, __index__) == EMPTY)
#define LAST_FILE (FILES - 1)
#define LAST_RANK (RANKS - 1)
#define SQUARE_DOWN(__index__) (__index__ + LINE_LENGTH)
#define FIGURE_HEIGHT 4

typedef char* Board;
typedef char* Captures;

enum Square {
	PAWN,
	BISHOP,
	ROOK,
	KNIGHT,
	QUEEN,
	KING,
	NO_PIECE,
	SQUARE_COUNT
};

enum Layer {
	PIXEL,
	MASK,
	LAYERS_DEEP
};

struct MoveSet {

	const Bool repeat;
	const Index count;
	const short moves[8][2];
};

extern const enum Square PIECE_MAP[128];
extern const struct MoveSet MOVES[SQUARE_COUNT];
extern const Set FIGURES[SQUARE_COUNT][LAYERS_DEEP][FIGURE_HEIGHT];
#define SYMBOL_COUNT 13
extern const char SYMBOLS[SYMBOL_COUNT];
#define SYMBOL_INDEX(P) (PIECE_MAP[P] == NO_PIECE ? SYMBOL_COUNT - 1 : PIECE_MAP[P] + NO_PIECE * IS_WHITE(P))

#define STANDARD_DECKS {\
	"RnBqQbNr", \
	"pPpPpPpP", \
	"kkkkkkkk", \
	"KKKKKKKK" \
}
#define KNIGHT_DECKS {\
	"NnNnNnNn", \
	"nNnNnNnN", \
	"nnnnnnnn", \
	"NNNNNNNN" \
}
#define BISHOP_DECKS {\
	"BbBbBbBb", \
	"bBbBbBbB", \
	"bbbbbbbb", \
	"BBBBBBBB" \
}
static const char DECKS[GRADES][INCREMENTS + 1] = STANDARD_DECKS;

extern const Count PIECE_VALUES[SQUARE_COUNT];
#define PIECE_VALUE(P) (PIECE_VALUES[PIECE_MAP[P]])

#define PIECE_BIT(P) (((Set)1u) << ((Set) (P - 'B')))

static const Set PIECE_SET =
PIECE_BIT(WHITE_PAWN) |
PIECE_BIT(BLACK_PAWN) |
PIECE_BIT(WHITE_BISHOP) |
PIECE_BIT(BLACK_BISHOP) |
PIECE_BIT(WHITE_ROOK) |
PIECE_BIT(BLACK_ROOK) |
PIECE_BIT(WHITE_KNIGHT) |
PIECE_BIT(BLACK_KNIGHT) |
PIECE_BIT(WHITE_QUEEN) |
PIECE_BIT(BLACK_QUEEN) |
PIECE_BIT(WHITE_KING) |
PIECE_BIT(BLACK_KING);

#define IS_PIECE(P) (PIECE_SET & PIECE_BIT(P))

void init_board(Board);
void init_captures(Captures);