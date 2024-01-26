#pragma once
#include <sys/timeb.h>

typedef char Piece;
typedef unsigned short Index;
typedef unsigned int Settings;
typedef unsigned int Events;

enum Setting {

	WHITE_PAWN_HIT_UP = 1 << 0,
	BLACK_PAWN_SPAWN_LOW = 1 << 1,
	WHITE_PAWN_LAND_HIGH = 1 << 2,
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