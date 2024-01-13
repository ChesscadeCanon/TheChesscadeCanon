#pragma once
#include <stdbool.h>
#include <stdlib.h>

#if defined(_MSC_VER)
//  Microsoft 
#elif defined(__GNUC__)
//  GCC
#include <stddef.h>
#endif
#define DEAD_PLAYER '!'
#define EMPTY '_'
#define WHITE_PAWN 'P'
#define BLACK_PAWN 'p'
#define WHITE_KING 'K'
#define BLACK_KING 'k'
#define WHITE_QUEEN 'Q'
#define BLACK_QUEEN 'q'
#define WHITE_ROOK 'R'
#define BLACK_ROOK 'r'
#define WHITE_KNIGHT 'N'
#define BLACK_KNIGHT 'n'
#define WHITE_BISHOP 'B'
#define BLACK_BISHOP 'b'
#define FILES 8ull
#define RANKS 8ull
#define EMPTY_CAPTURES "********"
#define EMPTY_SQUARE(B, I) (GET_SQUARE(B, I) == EMPTY)
#define HAS_CAPTURED(B) (strncmp(EMPTY_CAPTURES, B + CAPTURE_INDEX, CAPTURE_LENGTH) != 0)
#define SQUARE_RANK(I) ((unsigned short)(I / LINE_LENGTH))
#define SQUARE_FILE(I) ((unsigned short)(I % LINE_LENGTH))
#define SQUARE_BIT(I) (1ull << (SQUARE_RANK(I) * FILES + SQUARE_FILE(I)))
#define SET_CAPTURE(B, C, V) (B[CAPTURE_INDEX + C] = V)
#define SET_SQUARE(B, I, V) (B[I] = V)
#define IS_WHITE(P) (P <= 'Z')
#define SQUARE_INDEX(R, F) ((R) * LINE_LENGTH + (F))
#define LINE_LENGTH (FILES + 1ull)
#define FOUR_LINES (LINE_LENGTH * 4ull)
#define BOARD_LENGTH (LINE_LENGTH * RANKS)
#define CAPTURE_LENGTH 8ull
#define END_LENGTH 2ull
#define STATE_LENGTH (BOARD_LENGTH + CAPTURE_LENGTH + END_LENGTH)
#define CURSES_STATE_LENGTH (STATE_LENGTH + RANKS + 2)
#define TERMINATOR_INDEX (STATE_LENGTH - 1)
#define TERMINATE(B) (B[TERMINATOR_INDEX] = '\0')
#define CAPTURE_INDEX BOARD_LENGTH
#define GET_SQUARE(B, I) B[I]
#define GET_CAPTURE(B, I) (B[CAPTURE_INDEX + I])
#define LAST_FILE (FILES - 1ull)
#define LAST_RANK (RANKS - 1ull)
#define SQUARE_DOWN(I) (I + LINE_LENGTH)

typedef char* Board;
typedef char Piece;

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

struct MoveSet {

	const bool repeat;
	const unsigned short count;
	const short moves[8][2];
};

extern const enum Square PIECE_MAP[128];
extern const struct MoveSet MOVES[SQUARE_COUNT];

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
static const char DECKS[4][9] = STANDARD_DECKS;

extern const size_t PIECE_VALUES[SQUARE_COUNT];
#define PIECE_VALUE(P) (PIECE_VALUES[PIECE_MAP[P]])

#define PIECE_BIT(P) (((size_t)1u) << ((size_t) (P - 'B')))

static const size_t PIECE_SET =
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
void init_captures(Board);