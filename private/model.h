#pragma once
////////////////////////////////////////////////////////////////////////////////   
#define RULES "\
Chesscade is a falling block puzzle game with chess pieces. Pieces fall from the\n\
top of the board. When a piece reaches the bottom, it captures all pieces it\n\
could capture in chess from its position. When a king is captured or the next\n\
piece can't spawn, the game ends. As a piece falls, a cursor moves across the\n\
top of the board. When it reaches one side, it wraps around to the other. When\n\
a capture is made, the cursor changes directions. When a piece lands, the next\n\
piece is determined by where the cursor is, whether the cursor wrapped around,\n\
and whether the last piece made any captures. If the cursor wrapped, a king\n\
spawns. Otherwise, if the last piece made any captures, a rook, knight, bishop,\n\
or queen spawns, depending on the position of the cursor. Otherwise, a pawn\n\
spawns. Each piece can only reach squares it could reach in chess. Thus, black\n\
pawns spawn on the rank 7, white pawns land on rank 2, and bishops move 2\n\
squares at a time so they stay on their colors. White pawns capture upward and\n\
black pawns capture downward. If a white pawn lands on rank 8 or a black pawn\n\
lands on rank 1, it promotes to a queen before capturing. The current \"combo\"\n\
is the number of consecutive turns on which pices have been captured. The higher\n\
the combo, the faster pieces fall. The points scored by a given piece are\n\
determined by:\n\
[total point value of pieces captured] x [number of pieces captured] x [combo]\n\
Point values are as follows: pawn=1 knight=3 bishop=3 rook=5 queen=9 king=0\n\
Finally, no points can be scored if the board state is identical to any\n\
previous board state in the current game.\n\
"

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

#define FPS 60
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
	WHITE_PAWN_SPAWN_HIGH = 1 << 1,
	WHITE_PAWN_LAND_HIGH = 1 << 2,
	PAWNS_PROMOTE = 1 << 3,
	NO_CAPTURE_ON_REPEAT = 1 << 4,
	DOUBLE_BISHOPS = 1 << 5,
	CHECKMATE = 1 << 6,
	DIAGONALS = 1 << 7,
	DEFAULT_SETTINGS = WHITE_PAWN_HIT_UP | WHITE_PAWN_SPAWN_HIGH | WHITE_PAWN_LAND_HIGH | PAWNS_PROMOTE | NO_CAPTURE_ON_REPEAT | DOUBLE_BISHOPS | CHECKMATE | DIAGONALS
};

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
	char player;
	unsigned short player_rank;
	unsigned short player_file;
	short cursor;
	unsigned short cursor_rank;
	unsigned short cursor_file;
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