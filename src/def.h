#pragma once

// The master game object, exposed in src/core/game.h.
struct Game;

/**
* Internally, ranks range 0 - 7 from the top of the board to the bottom,
* while files range 0 - 7 from left to right. Interfaces should follow
* traditional chess conventions, numbering ranks 1 - 8 bottom to top,
* and files A - H from left to right.
*/
#define RANKS 8
#define FILES 8

/**
* The position of piece in the "deck" from which pieces are spawned is
* described as its "grade" (x-axis) and "increment" (y-axis).
*/
#define GRADES 4
#define INCREMENTS FILES

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
#define FPS 60
#define SPF (1.0 / ((long double) FPS))
#define MPF ((Time) (1000.0 * SPF))

typedef char Piece;
typedef short Trenary;
typedef unsigned short Index;
typedef unsigned long long Count;
typedef unsigned long long Set;
typedef int Settings;
typedef int Events;
typedef long long Time;
typedef unsigned short Bool;
typedef long double Fraction;
typedef long long Rating;
#define True 1u
#define False 0u

enum Setting {

	// white pawns attack upward instead of downward
	WHITE_PAWN_HIT_UP = 1 << 0,
	// black pawns spawn on rank 7 instead of 8
	BLACK_PAWN_SPAWN_LOW = 1 << 1,
	// white pawns land on rank 2 instead of 1
	WHITE_PAWN_LAND_HIGH = 1 << 2,
	// white pawns become queens if they land on rank 8, black pawns become queens if they land on rank 1
	PAWNS_PROMOTE = 1 << 3,
	// pieces can't be captured on a board that's a repeat of a previous board state in the current game
	NO_CAPTURE_ON_REPEAT = 1 << 4,
	// bishops move two spaces at a time when moving or falling orthogonally, so they stay on the color where they spawned
	DOUBLE_BISHOPS = 1 << 5,
	// the game ends when a king is captured
	CHECKMATE = 1 << 6,
	// the falling piece can be moved diagonally
	DIAGONALS = 1 << 7,
	// a king spawns when the game reaches a state that is a repeat of a previous state
	KING_ON_REPEAT = 1 << 8,
	// a piece doesn't automatically fall in rank as long as the player continues moving it
	FLYING_PIECES = 1 << 9,
	STANDARD_SETTINGS = 
		WHITE_PAWN_HIT_UP
		| BLACK_PAWN_SPAWN_LOW 
		| WHITE_PAWN_LAND_HIGH 
		| PAWNS_PROMOTE
		| KING_ON_REPEAT
		| DOUBLE_BISHOPS
		| CHECKMATE
		| DIAGONALS
};

enum Event {

	EVENT_FELL = 1 << 0,
	EVENT_LEFT = 1 << 2,
	EVENT_RIGHT = 1 << 3,
	EVENT_DOWN = 1 << 4,
	EVENT_DROPPED = 1 << 5,
	EVENT_WRAPPED = 1 << 6,
	EVENT_LANDED = 1 << 7,
	EVENT_CAPTURE = 1 << 8
};

enum Filter {

	FILTER_RANDOM = 0,
	FILTER_LOSSES = 1
};