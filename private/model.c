#include "platform.h"
#include "model.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>

#define MOVE_RATE(G) 64
#define FPS 60
#define EMPTY '_'
#define GET_SQUARE(S, I) S[I]
#define GET_CAPTURE(S, I) (S[CAPTURE_INDEX + I])
#define PLACE_PLAYER(G) SET_SQUARE(G->state, PLAYER_SQUARE(G), G->player)
#define EMPTY_CAPTURES "********"
#define EMPTY_SQUARE(S, I) (GET_SQUARE(S, I) == EMPTY)
#define HAS_CAPTURED(S) (strncmp(EMPTY_CAPTURES, S + CAPTURE_INDEX, CAPTURE_LENGTH) != 0)
#define SQUARE_RANK(I) ((unsigned short)(I / LINE_LENGTH))
#define SQUARE_FILE(I) ((unsigned short)(I % LINE_LENGTH))
#define SQUARE_BIT(I) (1ull << (SQUARE_RANK(I) * FILES + SQUARE_FILE(I)))
#define SET_CAPTURE(S, C, V) (S[CAPTURE_INDEX + C] = V)
#define REVERSE_CURSOR(G) (G->cursor *= -1)
#define SQUARE_DOWN(I) (I + LINE_LENGTH)
#define PLAYER_SQUARE(G) SQUARE_INDEX(G->player_rank, G->player_file)
#define DOUBLE_BISHOP(G) (PIECE_MAP[G->player] == BISHOP && IS_SET(G->settings, DOUBLE_BISHOPS))
#define BISHOP_SPEED(G, M) ((M) / (1 + DOUBLE_BISHOP(G)))
#define PLAYER_DOWN(G) SQUARE_INDEX(G->player_rank + (DOUBLE_BISHOP(G) + 1), G->player_file)
#define PLAYER_LEFT(G) SQUARE_INDEX(G->player_rank, G->player_file - (1 + DOUBLE_BISHOP(G)))
#define PLAYER_RIGHT(G) SQUARE_INDEX(G->player_rank, G->player_file + (1 + DOUBLE_BISHOP(G)))
#define PLAYER_DOWN_LEFT(G) SQUARE_INDEX(G->player_rank + 1, G->player_file - 1)
#define PLAYER_DOWN_RIGHT(G) SQUARE_INDEX(G->player_rank + 1, G->player_file + 1)
#define EASE(G) (1024 - min(G->combo * 64, 512)) * (1 + DOUBLE_BISHOP(G))
#define QUEEN_ME(G, R) (\
	IS_SET(G->settings, PAWNS_PROMOTE) ?\
		G->player == WHITE_PAWN && (R) == 0 ? WHITE_QUEEN \
		: ((G->player == BLACK_PAWN && (R) == LAST_RANK) ? BLACK_QUEEN \
		: G->player) \
	: G->player \
)
#define KING_ME(S, P) (IS_WHITE(P) ? WHITE_KING : BLACK_KING)
#define DRAW_NEXT(G) DECKS[G->cursor_rank][G->cursor_file]
#define SPAWN_RANK(G) (IS_SET(G->settings, WHITE_PAWN_SPAWN_HIGH) && DRAW_NEXT(G) == WHITE_PAWN ? 0 : (G->cursor_rank))
#define NEXT_PIECE(G) (\
	EMPTY_SQUARE(G->state, SQUARE_INDEX(SPAWN_RANK(G), G->cursor_file)) ?\
		DRAW_NEXT(G) \
	:\
		DEAD_PLAYER \
)
#define IN_BOUNDS(V, L, H) (V >= L && V < H)
#define RAISE_FLOOR(G) (IS_SET(G->settings, WHITE_PAWN_LAND_HIGH) && (G->player == WHITE_PAWN) ? LINE_LENGTH : 0)
#define ON_BOARD(G, I) (IN_BOUNDS(I, 0, BOARD_LENGTH) && G->state[I] != '\n')
#define CAN_CAPTURE(G, I) (\
	ON_BOARD(G, I) &&\
	IS_PIECE(GET_SQUARE(G->state, I)) &&\
	IS_WHITE(GET_SQUARE(G->state, I)) != IS_WHITE(G->player) &&\
	(!G->repeat) \
)
#define NEXT_PLAYER(G) (G->player = NEXT_PIECE(G)) 
#define SPAWN(G) (\
	NEXT_PLAYER(G) &\
	(G->player_rank = SPAWN_RANK(G)) &\
	(G->player_file = G->cursor_file) \
)
#define PACMAN(G, I) (abs(SQUARE_FILE(I) - (G->player_file)) > 2)
#define CAN_STRIKE(G, I) (ON_BOARD(G, I + RAISE_FLOOR(G)) && EMPTY_SQUARE(G->state, I))
#define CAN_MOVE(G, I) (CAN_STRIKE(G, I) && !PACMAN(G, I))
#define CURSOR_WRAPPED(G) (G->cursor_rank > 1)
#define CURSOR_GRADE(G, W) (!HAS_CAPTURED(G->state) + (W) * 2)
#define CURSOR_INCREMENT(G) (\
	(G->cursor > 0 && G->cursor_file < LAST_FILE) ||\
	(G->cursor < 0 && G->cursor_file > 0) ?\
	G->cursor : -G->cursor * LAST_FILE \
)
#define LAND(G) (\
	PLACE_PLAYER(G) &\
	SPAWN(G) \
)
#define MOVE_DOWN(G) (_move_player(G, PLAYER_DOWN(G)))
#define MOVE_RIGHT(G) (_move_player(G, PLAYER_RIGHT(G)))
#define MOVE_LEFT(G) (_move_player(G, PLAYER_LEFT(G)))
#define MOVE_DOWN_RIGHT(G) (_move_player(G, PLAYER_DOWN_RIGHT(G)))
#define MOVE_DOWN_LEFT(G) (_move_player(G, PLAYER_DOWN_LEFT(G)))
#define FALL(G) (MOVE_DOWN(G) && _update_cursor(G))
#define INIT(S) (TERMINATE(S))
#define GAME_OVER(G) (G->player == DEAD_PLAYER)
#define LAST_FILE (FILES - 1ull)
#define LAST_RANK (RANKS - 1ull)

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

const struct MoveSet MOVES[SQUARE_COUNT] = {
	[PAWN] = {
		.repeat = false,
		.count = 2,
		.moves = {
			{1, 1},
			{1, -1}
		}
	},
	[BISHOP] = {
		.repeat = true,
		.count = 4,
		.moves = {
			{1, 1},
			{-1, 1},
			{-1, -1},
			{1, -1}
		}
	},
	[ROOK] = {
		.repeat = true,
		.count = 4,
		.moves = {
			{0, 1},
			{-1, 0},
			{0, -1},
			{1, 0}
		}
	},
	[KNIGHT] = {
		.repeat = true,
		.count = 8,
		.moves = {
			{1, 2},
			{-1, 2},
			{-2, 1},
			{-2, -1},
			{-1, -2},
			{1, -2},
			{2, -1},
			{2, 1}
		}
	},
	[QUEEN] = {
		.repeat = true,
		.count = 8,
		.moves = {
			{1, 1},
			{-1, 1},
			{-1, -1},
			{1, -1},
			{0, 1},
			{-1, 0},
			{0, -1},
			{1, 0}
		}
	},
	[KING] = {
		.repeat = false,
		.count = 8,
		.moves = {
			{1, 1},
			{-1, 1},
			{-1, -1},
			{1, -1},
			{0, 1},
			{-1, 0},
			{0, -1},
			{1, 0}
		}
	}
};

const char DECKS[4][9] = {
	"RnBqQbNr",
	"pPpPpPpP",
	"kkkkkkkk",
	"KKKKKKKK"
};

const size_t PIECE_SET =
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

const enum Square PIECE_MAP[128] = {
	[WHITE_PAWN] = PAWN,
	[BLACK_PAWN] = PAWN,
	[WHITE_BISHOP] = BISHOP,
	[BLACK_BISHOP] = BISHOP,
	[WHITE_ROOK] = ROOK,
	[BLACK_ROOK] = ROOK,
	[WHITE_KNIGHT] = KNIGHT,
	[BLACK_KNIGHT] = KNIGHT,
	[WHITE_QUEEN] = QUEEN,
	[BLACK_QUEEN] = QUEEN,
	[WHITE_KING] = KING,
	[BLACK_KING] = KING,
	[EMPTY] = NO_PIECE
};

const size_t PIECE_VALUES[SQUARE_COUNT] = {
	[PAWN] = 1,
	[BISHOP] = 3,
	[KNIGHT] = 3,
	[ROOK] = 5,
	[QUEEN] = 9,
	[KING] = 0,
	[NO_PIECE] = 0
};

#define PIECE_VALUE(P) (PIECE_VALUES[PIECE_MAP[P]])

unsigned short _update_cursor(struct Game* game) {

	const short inc = CURSOR_INCREMENT(game);
	game->cursor_file += inc;
	game->cursor_rank = CURSOR_GRADE(game, abs(inc) > 1 || CURSOR_WRAPPED(game));
	return game->cursor;
}

size_t _bit_index(size_t bit) {

	size_t i = 0, b = 1;

	while (i < 64) {

		if (bit & b) break;
		++i;
		b <<= 1;
	}

	return i >= 64 ? 0 : i;
}

size_t _bit_rank(size_t bit) {

	return _bit_index(bit) / FILES;
}

size_t _bit_file(size_t bit) {

	return _bit_index(bit) % FILES;
}

size_t _free_children(struct Histotrie* root) {

	if (!root) return 0;
	size_t ret = 1;

	for (size_t c = 0; c < TRIE_CHILDREN; ++c) {

		if (root->children[c]) {

			ret += _free_children(root->children[c]);
		}
	}

	free(root);
	MEMLOG("freed histotrie node\n");
	return ret;
}

void _free_histotrie(struct Game* game) {

	size_t count = _free_children(game->histotrie);
	MEMLOGF("freed %llu histotrie nodes\n", count);
}

void _init_histotrie(struct Histotrie* histotrie) {

	memset(histotrie->children, 0, TRIE_CHILDREN * sizeof(struct Histotrie*));
}

struct Histotrie* _malloc_histotrie() {

	struct Histotrie* ret = malloc(sizeof(struct Histotrie));
	MEMLOG("malloc histotrie\n");
	_init_histotrie(ret);
	return ret;
}

size_t _record_state(struct Histotrie* root, const Board board, const size_t index) {

	if (index >= BOARD_LENGTH) return 0;
	if (board[index] == '\n') return _record_state(root, board, index + 1);

	Piece piece = board[index];
	enum Square square = PIECE_MAP[piece];
	size_t child = square == NO_PIECE ? TRIE_CHILDREN - 1 : square + NO_PIECE * IS_WHITE(piece);

	if (root->children[child]) {

		return _record_state(root->children[child], board, index + 1);
	}

	root->children[child] = malloc(sizeof(struct Histotrie));
	MEMLOG("malloc histotrie node\n");
	_init_histotrie(root->children[child]);
	return 1 + _record_state(root->children[child], board, index + 1);
}

size_t _chronicle(struct Game* game) {

	if (!game->histotrie) return false;
	if (!IS_SET(game->settings, NO_CAPTURE_ON_REPEAT)) return false;

	const size_t ret = _record_state(game->histotrie, game->state, 0);
	MEMLOGF("created %llu histotrie nodes\n", ret);
	game->repeat = !ret;
	return ret;
}

void _init_captures(State state) {

	memcpy(state + BOARD_LENGTH, EMPTY_CAPTURES, CAPTURE_LENGTH * sizeof(char));
}

void _init_board(Board board) {

	for (size_t i = 0, r = 0; r < RANKS; ++r) {

		for (size_t f = 0; f < FILES; ++f) {

			board[i] = EMPTY;
			++i;
		}

		++i;
	}
}

void _kill(State state, const size_t square, const size_t move) {

	Piece piece = GET_SQUARE(state, square);
	SET_CAPTURE(state, move, piece);
	SET_SQUARE(state, square, EMPTY);
}

size_t _capture(State state, const size_t square, const size_t move, const bool execute) {

	if (execute) {

		_kill(state, square, move);
	}

	return SQUARE_BIT(square);
}

size_t _hit(struct Game* game, enum Square piece_type, const size_t rank, const size_t file, const size_t move, const bool execute, const bool pattern) {

	struct MoveSet move_set = MOVES[piece_type];
	const short reverse = piece_type == PAWN && IS_WHITE(game->player) && IS_SET(game->settings, WHITE_PAWN_HIT_UP) ? -1 : 1;
	const size_t to_rank = rank + move_set.moves[move][0] * reverse;
	const size_t to_file = file + move_set.moves[move][1];
	const size_t square = SQUARE_INDEX(to_rank, to_file);
	const bool open = CAN_STRIKE(game, square);
	size_t ret = open && pattern ? SQUARE_BIT(square) : 0;

	if (move_set.repeat && open) {

		ret |= _hit(game, piece_type, to_rank, to_file, move, execute, pattern);
	}
	else if (CAN_CAPTURE(game, square)) {

		ret |= _capture(game->state, square, move, execute);
	}

	return ret;
}

size_t _strike(struct Game* game, const enum Square piece_type, const size_t rank, const size_t file, const size_t move, const bool execute, const bool pattern) {

	const struct MoveSet move_set = MOVES[piece_type];
	if (move >= move_set.count) return 0;
	const size_t ret = _hit(game, piece_type, rank, file, move, execute, pattern);
	return ret | _strike(game, piece_type, rank, file, move + 1, execute, pattern);
}

size_t _drop_to(struct Game* game, const size_t from) {

	size_t to = SQUARE_DOWN(from);

	if (DOUBLE_BISHOP(game)) {

		to = SQUARE_DOWN(to);
	}

	if (CAN_MOVE(game, to)) {

		return _drop_to(game, to);
	}

	return from;
}

size_t _judge(struct Game* game) {

	game->scored = 0;
	size_t count = 0;

	for (size_t i = 0; i < FILES; ++i) {

		Piece piece = GET_CAPTURE(game->state, i);

		if (IS_SET(game->settings, CHECKMATE) && PIECE_MAP[piece] == KING) {

			game->player = DEAD_PLAYER;
		}

		if (piece != *EMPTY_CAPTURES) {

			game->scored += PIECE_VALUE(piece);
			++count;
		}
	}

	count ? ++game->combo : (game->combo = 0);
	game->scored *= game->combo * count;
	game->score += game->scored;
	return game->score;
}

void _resolve(struct Game* game) {

	const unsigned short from_rank = game->player_rank, from_file = game->player_file;
	game->player = QUEEN_ME(game, from_rank);
	const size_t captures = attack(game, true, false, false);
	LAND(game);
	game->cursor_rank = max(0, game->cursor_rank - (captures ? 1 : 0) - (game->cursor_rank > 1) * 2);
	_judge(game);

	if (captures) {

		REVERSE_CURSOR(game);
	}

	_chronicle(game);
}

bool _move_player(struct Game* game, size_t to) {

	const unsigned short to_rank = SQUARE_RANK(to), to_file = SQUARE_FILE(to);

	if (CAN_MOVE(game, to)) {

		game->player_rank = to_rank;
		game->player_file = to_file;
		return true;
	}
	else if (to_rank > game->player_rank) {

		_resolve(game);
	}

	return false;
}

time_t _buy_move(struct Game* game, long int* moved, const unsigned short multiplier) {

	if (*moved < 0) return 0;
	const long int rate = MOVE_RATE(game) * multiplier;
	const long int steps = *moved / rate;
	*moved -= rate * steps;
	assert(steps >= 0);
	return (time_t)steps;
}

void _drop(struct Game* game) {

	const size_t from = PLAYER_SQUARE(game);
	const size_t to = _drop_to(game, from);
	_move_player(game, to);
}

time_t _move_right(struct Game* game, time_t steps) {

	if (steps <= 0) return steps;
	MOVE_RIGHT(game);
	_move_right(game, steps - 1);
	return steps;
}

time_t _move_left(struct Game* game, time_t steps) {

	if (steps <= 0) return steps;
	MOVE_LEFT(game);
	_move_left(game, steps - 1);
	return steps;
}

time_t _move_down(struct Game* game, time_t steps) {

	if (steps <= 0) return steps;
	MOVE_DOWN(game);
	_move_down(game, steps - 1);
	return steps;
}

time_t _move_down_left(struct Game* game, time_t steps) {

	if (steps <= 0) return steps;
	MOVE_DOWN_LEFT(game);
	_move_down_left(game, steps - 1);
	return steps;
}

time_t _move_down_right(struct Game* game, time_t steps) {

	if (steps <= 0) return steps;
	MOVE_DOWN_RIGHT(game);
	_move_down_right(game, steps - 1);
	return steps;
}

void _fall(struct Game* game, time_t falls) {

	if (falls <= 0) return;
	else FALL(game);
	_fall(game, falls - 1);
}

void _take_input(struct Game* game) {

	if (game->dropped) {

		_drop(game);
		game->dropped = false;
	}

	const bool moving_down = game->moved_down >= MOVE_RATE(game);
	const bool moving_left = game->moved_left >= MOVE_RATE(game);
	const bool moving_right = game->moved_right >= MOVE_RATE(game);
	const bool diagonals = IS_SET(game->settings, DIAGONALS);
	const bool diagonal = diagonals && moving_down && moving_left != moving_right;
	const unsigned short multiplier = (!diagonal && DOUBLE_BISHOP(game)) + 1;
	const time_t down = _buy_move(game, &game->moved_down, multiplier);
	const time_t left = _buy_move(game, &game->moved_left, multiplier);
	const time_t right = _buy_move(game, &game->moved_right, multiplier);

	if (diagonals && left > 0 && down > 0 && right == 0) {
		_move_down_left(game, min(left - right, down));
	}
	else if (diagonals && left == 0 && down > 0 && right > 0) {
		_move_down_right(game, min(right - left, down));
	}
	else if (left > 0 || right > 0 || down > 0) {
		_move_right(game, max(0, right - left));
		_move_left(game, max(0, left - right));
		_move_down(game, max(0, down));
	}
}

void _init_state(State state) {

	memset(state, '\n', STATE_LENGTH * sizeof(char));
	_init_captures(state);
	_init_board(state);
	INIT(state);
}

void _init_game(struct Game* game) {

	game->score = 0;
	game->combo = 0;
	game->scored = 0;
	game->time = 0;
	game->fell = 0;
	game->repeat = false;
	game->paused = false;
	game->player = 'W';
	game->player_rank = 1;
	game->player_file = 7;
	game->cursor = -1;
	game->cursor_rank = 1;
	game->cursor_file = LAST_FILE;
	game->dropped = false;
	game->moved_left = -1;
	game->moved_right = -1;
	game->moved_down = -1;
	game->settings = 0;
	_init_state(game->state);
}

bool game_over(struct Game* game) {

	return GAME_OVER(game);
}

bool cursor_wrapped(struct Game* game) {

	return CURSOR_WRAPPED(game);
}

size_t square_bit(size_t rank, size_t file) {

	return SQUARE_BIT(SQUARE_INDEX(rank, file));
}

time_t ease(struct Game* game) {

	return EASE(game);
}

const char* deck(size_t d) {

	return DECKS[d];
}

Piece next_piece(struct Game* game) {

	return NEXT_PIECE(game);
}

void free_game(struct Game* game) {

	if (game) {

		_free_histotrie(game);
		MEMLOG("freed game\n");
		free(game);
	}
}

size_t attack(struct Game* game, const bool execute, const bool forecast, const bool pattern) {

	Piece piece = game->player;
	size_t rank = game->player_rank;
	const size_t file = game->player_file;

	if (forecast) {

		rank = SQUARE_RANK(_drop_to(game, SQUARE_INDEX(rank, file)));
		piece = QUEEN_ME(game, rank);
	}

	if (execute) {

		_init_captures(game->state);
	}

	enum Square piece_type = PIECE_MAP[piece];
	return _strike(game, piece_type, rank, file, 0, execute, pattern);
}

size_t forecast_rank(struct Game* game) {

	const size_t rank = game->player_rank;
	const size_t file = game->player_file;
	return SQUARE_RANK(_drop_to(game, SQUARE_INDEX(rank, file)));
}

char forecast_piece(struct Game* game) {

	const size_t rank = forecast_rank(game);
	return QUEEN_ME(game, rank);
}

void exist(struct Game* game, const time_t falls) {

	_take_input(game);
	_fall(game, falls);
}

void begin(struct Game* game) {

	SPAWN(game);
}

struct Game* malloc_init_game(Settings settings) {

	struct Game* game = malloc(sizeof(struct Game));
	MEMLOG("malloc game\n");
	assert(game);
	_init_game(game);
	game->histotrie = _malloc_histotrie();
	game->settings = settings;
	return game;
}