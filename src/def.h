#pragma once
/**
* Internally, ranks range 0 - 7 from the top of the board to the bottom,
* while files range 0 - 7 from left to right. Interfaces should follow
* traditional chess conventions, numbering ranks 1 - 8 bottom to top,
* and files A - H from left to right.
*/
#define FILES 8
#define RANKS 8

// when the game ends, the player will be set to this
#define DEAD_PLAYER '!'
// an empty square on the board
#define EMPTY '_'
// chess piece notation
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

struct Game;

typedef char Piece;
typedef unsigned short Index;
typedef unsigned long long Count;
typedef unsigned long long Set;
typedef int Settings;
typedef int Events;
typedef long long Time;

enum Setting {

	// white pawns attack upward instead of downward
	WHITE_PAWN_HIT_UP = 1 << 0,
	// black pawns spawn on rank 7 instead of 8
	BLACK_PAWN_SPAWN_LOW = 1 << 1,
	// white pawns land on rank 2 instead of 1
	WHITE_PAWN_LAND_HIGH = 1 << 2,
	// white pawns become queens if they land on rank 8, black pawns become queens if they land on rank 1
	PAWNS_PROMOTE = 1 << 3,
	NO_CAPTURE_ON_REPEAT = 1 << 4,
	DOUBLE_BISHOPS = 1 << 5,
	CHECKMATE = 1 << 6,
	DIAGONALS = 1 << 7,
	KING_ON_REPEAT = 1 << 8,
	FLYING_PIECES = 1 << 9,
	DEFAULT_SETTINGS = WHITE_PAWN_HIT_UP | BLACK_PAWN_SPAWN_LOW | WHITE_PAWN_LAND_HIGH | PAWNS_PROMOTE | KING_ON_REPEAT | DOUBLE_BISHOPS | CHECKMATE | DIAGONALS | FLYING_PIECES
};

enum Event {

	EVENT_FELL = 1 << 0,
	EVENT_LEFT = 1 << 2,
	EVENT_RIGHT = 1 << 3,
	EVENT_DOWN = 1 << 4,
	EVENT_DROPPED = 1 << 5,
	EVENT_WRAPPED = 1 << 6
};