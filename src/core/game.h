#pragma once
#include "histotrie.h"
#include <sys/timeb.h>

typedef unsigned int Settings;
typedef unsigned int Events;

#define FPS 60
#define SPF (1.0 / ((long double) FPS))
#define MPF ((time_t) (1000.0 * SPF))
#define PLAYER_SQUARE(__game__) SQUARE_INDEX(__game__->player_rank, __game__->player_file)
#define SET(__settings__, __setting__) (__settings__ |= __setting__)
#define IS_SET(__settings__, __setting__) (__settings__ & __setting__)

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

/**
* All game attributes should be considered private by any programs using the Chesscade
* library. Use the API functions instead. There's nothing to stop you from breaking
* this rule except that it would be cheating and you might not know what you're doing.
*/
struct Game {

	bool pause;
	bool dropped;
	bool moved_left;
	bool moved_right;
	bool moved_down;
	long double dragged_left;
	long double dragged_right;
	long double dragged_down;
	size_t score;
	size_t combo;
	size_t scored;
	time_t time;
	time_t last_moved;
	time_t last_fell;
	char player;
	Index player_rank;
	Index player_file;
	short cursor;
	Index cursor_grade;
	Index cursor_increment;
	char board[BOARD_LENGTH];
	char captures[CAPTURE_LENGTH];
	bool repeat;
	Settings settings;
	Events events;
	size_t total_pieces;
	size_t total_value;
	struct Histotrie* histotrie;
};

void print_rules();
time_t ease(struct Game*);
const char* deck(size_t);
size_t square_bit(size_t, size_t);
struct Game* malloc_init_game(Settings);
void free_game(struct Game*);
void begin(struct Game*);
void pump(struct Game* game, const time_t passed);
Piece next_piece(struct Game*);
size_t forecast_rank(struct Game*);
char forecast_piece(struct Game*);
size_t attack(struct Game*, const bool, const bool, const bool);
bool cursor_wrapped(struct Game*);
bool game_over(struct Game*);
bool on_brink(struct Game*);