#pragma once
#include "histotrie.h"
#include <sys/timeb.h>

typedef size_t Settings;

#define FPS 60
#define PLAYER_SQUARE(G) SQUARE_INDEX(G->player_rank, G->player_file)
#define SET(B, V) (B |= V)
#define IS_SET(B, V) (B & V)
#define COUNT_INTERVALS(A, Z, N) (((Z) - (A)) / (N))
#define SPF (1.0 / ((long double) FPS))
#define MPF ((time_t) (1000.0 * SPF))
#define MILLISECONDS_DIFFERENCE(A, Z) ((time_t)(1000.0 * ((Z).time - (A).time) + ((Z).millitm - (A).millitm)));

enum Setting {

	WHITE_PAWN_HIT_UP = 1 << 0,
	BLACK_PAWN_SPAWN_LOW = 1 << 1,
	WHITE_PAWN_LAND_HIGH = 1 << 2,
	PAWNS_PROMOTE = 1 << 3,
	NO_CAPTURE_ON_REPEAT = 1 << 4,
	DOUBLE_BISHOPS = 1 << 5,
	CHECKMATE = 1 << 6,
	DIAGONALS = 1 << 7,
	DEFAULT_SETTINGS = WHITE_PAWN_HIT_UP | BLACK_PAWN_SPAWN_LOW | WHITE_PAWN_LAND_HIGH | PAWNS_PROMOTE | NO_CAPTURE_ON_REPEAT | DOUBLE_BISHOPS | CHECKMATE | DIAGONALS
};

struct Game {

	size_t score;
	size_t combo;
	size_t scored;
	time_t time;
	time_t fell;
	bool paused;
	char player;
	unsigned short player_rank;
	unsigned short player_file;
	short cursor;
	unsigned short cursor_grade;
	unsigned short cursor_increment;
	bool dropped;
	long int moved_left;
	long int moved_right;
	long int moved_down;
	char board[STATE_LENGTH];
	struct Histotrie* histotrie;
	bool repeat;
	Settings settings;
};

void print_rules();
time_t ease(struct Game*);
const char* deck(size_t);
size_t square_bit(size_t, size_t);
struct Game* malloc_init_game(Settings);
void free_game(struct Game*);
void begin(struct Game*);
void exist(struct Game* game, const time_t falls);
Piece next_piece(struct Game*);
size_t forecast_rank(struct Game*);
char forecast_piece(struct Game*);
size_t attack(struct Game*, const bool, const bool, const bool);
bool cursor_wrapped(struct Game*);
bool game_over(struct Game*);
bool on_brink(struct Game*);