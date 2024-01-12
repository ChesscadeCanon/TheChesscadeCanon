#pragma once
#include "histotrie.h"
#include <sys/timeb.h>
#include <stdbool.h>
#if defined(_MSC_VER)
//  Microsoft 
#elif defined(__GNUC__)
//  GCC
#include <stddef.h>
#endif
typedef char* Board;
typedef size_t Settings;
typedef char Piece;
typedef char* State;

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

#define FPS 60
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
#define SET_SQUARE(S, I, V) (S[I] = V)
#define PLAYER_SQUARE(G) SQUARE_INDEX(G->player_rank, G->player_file)
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
#define TERMINATE(S) (S[TERMINATOR_INDEX] = '\0')
#define CAPTURE_INDEX BOARD_LENGTH
#define SET(B, V) (B |= V)
#define IS_SET(B, V) (B & V)
#define COUNT_INTERVALS(A, Z, N) (((Z) - (A)) / (N))
#define SPF (1.0 / ((long double) FPS))
#define MPF ((time_t) (1000.0 * SPF))
#define MILLISECONDS_DIFFERENCE(A, Z) ((time_t)(1000.0 * ((Z).time - (A).time) + ((Z).millitm - (A).millitm)));

#define PIECE_BIT(P) (((size_t)1u) << ((size_t) (P - 'B')))
extern const size_t PIECE_SET;
#define IS_PIECE(P) (PIECE_SET & PIECE_BIT(P))

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

extern const enum Square PIECE_MAP[128];

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
	char state[STATE_LENGTH];
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