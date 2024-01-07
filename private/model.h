#pragma once
#include "config.h"
#include <sys/timeb.h>
#include <stdbool.h>

#ifdef __unix__                    /* __unix__ is usually defined by compilers targeting Unix systems */
#define max(A, B) (A > B ? A : B)
#define min(A, B) (A < B ? A : B)
#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */
#define OS_WINDOWS 1
#endif

typedef char* Board;
typedef size_t Settings;

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

struct Histotrie;
struct Game;

typedef char Piece;
typedef char* State;

#define FPS 60
#define EMPTY '_'
#define DEAD_PLAYER '!'
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
#define LAST_FILE (FILES - 1ull)
#define LAST_RANK (RANKS - 1ull)
#define MAX_CAPTURES 8ull
#define LINE_LENGTH (FILES + 1ull)
#define FOUR_LINES (LINE_LENGTH * 4ull)
#define BOARD_LENGTH (LINE_LENGTH * RANKS)
#define CAPTURE_LENGTH 8ull
#define DATA_LENGTH 15ull
#define STATE_LENGTH (BOARD_LENGTH + CAPTURE_LENGTH + DATA_LENGTH)
#define NEWLINE_COUNT 17ull
#define CURSES_STATE_LENGTH (STATE_LENGTH + NEWLINE_COUNT)
#define CAPTURE_INDEX BOARD_LENGTH
#define PLAYER_INDEX (STATE_LENGTH - 14)
#define PLAYER_RANK_INDEX (STATE_LENGTH - 11)
#define PLAYER_FILE_INDEX (STATE_LENGTH - 12)
#define CURSOR_INDEX (STATE_LENGTH - 9)
#define CURSOR_RANK_INDEX (STATE_LENGTH - 7)
#define CURSOR_FILE_INDEX (STATE_LENGTH - 6)
#define WRAPPED_INDEX (STATE_LENGTH - 4)
#define TERMINATOR_INDEX (STATE_LENGTH - 1)
#define TERMINATE(S) (S[TERMINATOR_INDEX] = '\0')
#define SET_SQUARE(S, I, V) (S[I] = V)
#define GET_SQUARE(S, I) S[I]
#define GET_PLAYER(S) (S[PLAYER_INDEX])
#define GET_PLAYER_RANK(S) (S[PLAYER_RANK_INDEX] - '0')
#define GET_PLAYER_FILE(S) (S[PLAYER_FILE_INDEX] - '0')
#define GET_CURSOR(S) (S[CURSOR_INDEX] - '0' ? 1 : -1)
#define GET_CURSOR_RANK(S) (S[CURSOR_RANK_INDEX] - '0')
#define GET_CURSOR_FILE(S) (S[CURSOR_FILE_INDEX] - '0')
#define GET_WRAPPED(S) (S[WRAPPED_INDEX] - '0')
#define GET_CAPTURE(S, I) (S[CAPTURE_INDEX + I])
#define SQUARE_INDEX(R, F) ((R) * LINE_LENGTH + (F))
#define PLAYER_SQUARE(S) SQUARE_INDEX(GET_PLAYER_RANK(S), GET_PLAYER_FILE(S))
#define PLACE_PLAYER(S) SET_SQUARE(S, PLAYER_SQUARE(S), GET_PLAYER(S))
#define GAME_OVER(S) (GET_PLAYER(S) == DEAD_PLAYER)
#define IS_WHITE(P) (P <= 'Z')
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
	WHITE_PAWN_SPAWN_HIGH = 1 << 1,
	WHITE_PAWN_LAND_HIGH = 1 << 2,
	PAWNS_PROMOTE = 1 << 3,
	NO_CAPTURE_ON_REPEAT = 1 << 4,
	DOUBLE_BISHOPS = 1 << 5,
	CHECKMATE = 1 << 6,
	DIAGONALS = 1 << 7,
	DEFAULT_SETTINGS = WHITE_PAWN_HIT_UP | WHITE_PAWN_SPAWN_HIGH | WHITE_PAWN_LAND_HIGH | PAWNS_PROMOTE | NO_CAPTURE_ON_REPEAT | DOUBLE_BISHOPS | CHECKMATE | DIAGONALS
};

extern const char DECKS[4][9];
extern const char MAP[128];

extern const enum Square PIECE_MAP[128];

#define TRIE_CHILDREN 13

struct Histotrie {

	struct Histotrie* children[TRIE_CHILDREN];
};

struct Game {

	size_t score;
	size_t combo;
	size_t scored;
	time_t time;
	time_t fell;
	bool paused;
	bool dropped;
	long int moved_left;
	long int moved_right;
	long int moved_down;
	char state[STATE_LENGTH];
	struct Histotrie* histotrie;
	bool repeat;
	Settings settings;
};

time_t ease(struct Game*);
const char* deck(struct Game*, size_t);
size_t square_bit(size_t, size_t);
void init_board(Board);
void init_game(struct Game*);
struct Game* malloc_init_game(Settings);
void free_game(struct Game*);
void init_state(State);
void begin(struct Game*);
void drop(struct Game*);
void fall(struct Game*, time_t);
void exist(struct Game* game, const time_t falls);
void take_input(struct Game*);
Piece next_piece(struct Game*);
size_t forecast_rank(struct Game*);
char forecast_piece(struct Game*);
size_t attack(struct Game*, const bool, const bool, const bool);
size_t chronicle(struct Game*);
struct Histotrie* malloc_histotrie();
void free_histotrie(struct Game*);

