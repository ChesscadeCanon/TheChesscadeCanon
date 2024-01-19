#include "game.h"
#include "config.h"
#include "../../rules.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>

#ifdef __unix__ 
#define max(A, B) (A > B ? A : B)
#define min(A, B) (A < B ? A : B)
#endif

#define MOVE_RATE(__game__) (64)
#define SINCE_MOVED(__game__) ((__game__->time) - __game__->last_moved)
#define SINCE_FELL(__game__) ((__game__->time) - __game__->last_fell)
#define FPS 60
#define PLACE_PLAYER(__game__) SET_SQUARE(__game__->board, PLAYER_SQUARE(__game__), __game__->player)
#define REVERSE_CURSOR(__game__) (__game__->cursor *= -1)
#define PLAYER_SQUARE(__game__) SQUARE_INDEX(__game__->player_rank, __game__->player_file)
#define DOUBLE_BISHOP(__game__) (PIECE_MAP[__game__->player] == BISHOP && IS_SET(__game__->settings, DOUBLE_BISHOPS))
#define BISHOP_SPEED(__game__, __moved__) ((__moved__) / (1 + DOUBLE_BISHOP(G)))
#define PLAYER_DOWN(__game__) SQUARE_INDEX(__game__->player_rank + (DOUBLE_BISHOP(__game__) + 1), __game__->player_file)
#define EASE(__game__) max(1, 512) * (1 + DOUBLE_BISHOP(__game__))
#define QUEEN_ME(__game__, __rank__) (\
	IS_SET(__game__->settings, PAWNS_PROMOTE) ?\
		__game__->player == WHITE_PAWN && (__rank__) == 0 ? WHITE_QUEEN \
		: ((__game__->player == BLACK_PAWN && (__rank__) == LAST_RANK) ? BLACK_QUEEN \
		: __game__->player) \
	: __game__->player \
)
#define DRAW_NEXT(__game__) DECKS[__game__->cursor_grade][__game__->cursor_increment]
#define SPAWN_RANK(__game__) (IS_SET(__game__->settings, BLACK_PAWN_SPAWN_LOW) && DRAW_NEXT(__game__) == BLACK_PAWN ? 1 : 0)
#define NEXT_PIECE(__game__) (\
	EMPTY_SQUARE(__game__->board, SQUARE_INDEX(SPAWN_RANK(__game__), __game__->cursor_increment)) ?\
		DRAW_NEXT(__game__) \
	:\
		DEAD_PLAYER \
)
#define IN_BOUNDS(__value__, __low__, __high__) (__value__ >= __low__ && __value__ < __high__)
#define RAISE_FLOOR(__game__) (IS_SET(__game__->settings, WHITE_PAWN_LAND_HIGH) && (__game__->player == WHITE_PAWN) ? LINE_LENGTH : 0)
#define ON_BOARD(__game__, __index__) (IN_BOUNDS(__index__, 0, BOARD_LENGTH) && __game__->board[__index__] != '\n' && __game__->board[__index__])
#define CAN_CAPTURE(__game__, __index__) (\
	ON_BOARD(__game__, __index__) &&\
	IS_PIECE(GET_SQUARE(__game__->board, __index__)) &&\
	IS_WHITE(GET_SQUARE(__game__->board, __index__)) != IS_WHITE(__game__->player) \
)
#define NEXT_PLAYER(__game__) (__game__->player = NEXT_PIECE(__game__)) 
#define SPAWN(__game__) (\
	NEXT_PLAYER(__game__) &\
	(__game__->player_rank = SPAWN_RANK(__game__)) &\
	(__game__->player_file = __game__->cursor_increment) &\
	(++__game__->total_pieces) &\
	(__game__->total_value += PIECE_VALUE(__game__->player)) \
)
#define PACMAN(__game__, __index__) (abs(SQUARE_FILE(__index__) - (__game__->player_file)) > 2)
#define CAN_STRIKE(__game__, __index__) (ON_BOARD(__game__, __index__ + RAISE_FLOOR(__game__)) && EMPTY_SQUARE(__game__->board, __index__))
#define CAN_MOVE(__game__, __index__) (CAN_STRIKE(__game__, __index__) && !PACMAN(__game__, __index__))
#define CURSOR_WRAPPED(__game__) (__game__->cursor_grade > 1)
#define CURSOR_GRADE(__game__, __king__) (!HAS_CAPTURED(__game__->captures) + (__king__) * 2)
#define CURSOR_INCREMENT(__game__) (\
	(__game__->cursor > 0 && __game__->cursor_increment < LAST_FILE) ||\
	(__game__->cursor < 0 && __game__->cursor_increment > 0) ?\
	__game__->cursor : -__game__->cursor * LAST_FILE \
)
#define LAND(__game__) (\
	PLACE_PLAYER(__game__) &\
	SPAWN(__game__) \
)
#define GAME_OVER(__game__) (__game__->player == DEAD_PLAYER)

void print_rules() {

	printf("%s", RULES);
}

unsigned short _update_cursor(struct Game* game) {

	const short inc = CURSOR_INCREMENT(game);
	const bool wrapped = abs(inc) > 1;
	game->cursor_increment += inc;
	game->cursor_grade = CURSOR_GRADE(game, wrapped || CURSOR_WRAPPED(game));
	game->events |= EVENT_WRAPPED * wrapped;
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

void _kill(struct Game* game, const size_t square, const size_t move) {

	Piece piece = GET_SQUARE(game->board, square);
	SET_CAPTURE(game->captures, move, piece);
	SET_SQUARE(game->board, square, EMPTY);
}

size_t _capture(struct Game* game, const size_t square, const size_t move, const bool execute) {

	if (execute) {

		_kill(game, square, move);
	}

	return SQUARE_BIT(square);
}

size_t _hit(struct Game* game, enum Square piece_type, const size_t rank, const size_t file, const size_t move, const bool execute, const bool pattern) {

	struct MoveSet move_set = MOVES[piece_type];
	const short invert = piece_type == PAWN && IS_WHITE(game->player) && IS_SET(game->settings, WHITE_PAWN_HIT_UP) ? -1 : 1;
	size_t to_rank = rank;
	size_t to_file = file;
	bool open = true;
	size_t ret = 0;

	while (open) {

		to_rank += move_set.moves[move][0] * invert;
		to_file += move_set.moves[move][1];
		const size_t square = SQUARE_INDEX(to_rank, to_file);
		open = CAN_STRIKE(game, square);
		ret |= open && pattern ? SQUARE_BIT(square) : 0;
		if (CAN_CAPTURE(game, square)) {

			ret |= _capture(game, square, move, execute);
		}
		if (!move_set.repeat) break;
	}

	return ret;
}

size_t _strike(struct Game* game, const enum Square piece_type, const size_t rank, const size_t file, const bool execute, const bool pattern) {

	const struct MoveSet move_set = MOVES[piece_type];
	size_t ret = 0;
	for (size_t move = 0; move < move_set.count; ++move) {

		ret |= _hit(game, piece_type, rank, file, move, execute, pattern);
	}
	return ret;
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

		Piece piece = GET_CAPTURE(game->captures, i);

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

size_t _chronicle(struct Game* game) {

	if (!game->histotrie) return false;
	if (!IS_SET(game->settings, NO_CAPTURE_ON_REPEAT | KING_ON_REPEAT)) return false;

	const size_t ret = record_state(game->histotrie, game->board, 0);
	MEMLOGF("created %llu histotrie nodes\n", ret);
	game->repeat = !ret;
	return ret;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               const char* LICENSE = "Chesscade is a falling block puzzle game with chess pieces.\nCopyright(C) 2024  George Cesana ne Guy\n\nThis program is free software : you can redistribute it and /or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the\nGNU General Public License for more details.\n\nYou should have received a copy of the GNU General Public License\nalong with this program.If not, see < https://www.gnu.org/licenses/>.";
void _resolve(struct Game* game) {

	const unsigned short from_rank = game->player_rank, from_file = game->player_file;
	game->player = QUEEN_ME(game, from_rank);
	const size_t captures = attack(game, true, false, false);
	LAND(game);
	_judge(game);
	_chronicle(game);
	game->cursor_grade = CURSOR_GRADE(game, 0);// game->repeat&& IS_SET(game->settings, KING_ON_REPEAT));

	if (captures) {

		REVERSE_CURSOR(game);
	}
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

time_t _buy_move(struct Game* game, bool* moved) {

	if (!*moved) return 0;
	const time_t rate = MOVE_RATE(game);
	const time_t since = SINCE_MOVED(game);
	const time_t steps = since / rate;
	assert(steps >= 0);
	printf("steps %lld\n", steps);
	//*moved = false;
	return steps;
}

void _drop(struct Game* game) {

	const size_t from = PLAYER_SQUARE(game);
	const size_t to = _drop_to(game, from);
	_move_player(game, to);
	game->events = EVENT_DROPPED;
}

time_t _move(struct Game* game, time_t steps, const short by_rank, const short by_file) {

	if (steps == 0) return 0;
	printf("move %lld steps\n", steps);
	const bool orthogonal = !by_rank != !by_file;
	const size_t multiplier = (DOUBLE_BISHOP(game) + orthogonal);
	time_t ret = 0;

	for (time_t s = 0; s < steps; ++s) {

		const size_t to_rank = game->player_rank + (by_rank * multiplier);
		const size_t to_file = game->player_file + (by_file * multiplier);
		const size_t to = SQUARE_INDEX(to_rank, to_file);
		if (!_move_player(game, to)) break;
		++ret;
	};

	return ret;
}

time_t _fall(struct Game* game) {

	const time_t falls = SINCE_FELL(game) / EASE(game);
	time_t ret = 0;
	
	for (time_t f = 0; f < falls; ++f) {

		const size_t rank = game->player_rank + (DOUBLE_BISHOP(game) + 1);
		const size_t file = game->player_file;
		const size_t to = SQUARE_INDEX(rank, file);

		if (_move_player(game, to)) {

			game->events = EVENT_FELL;
			_update_cursor(game);
			ret += EASE(game);
		}
	}

	return ret;
}

time_t _take_input(struct Game* game) {

	if (game->dropped) {

		_drop(game);
		game->dropped = false;
	}

	const bool moving_down = game->moved_down;
	const bool moving_left = game->moved_left;
	const bool moving_right = game->moved_right;
	const bool diagonals = IS_SET(game->settings, DIAGONALS);
	const bool diagonal = diagonals && moving_down && moving_left != moving_right;
	const time_t down = _buy_move(game, &game->moved_down);
	const time_t left = _buy_move(game, &game->moved_left);
	const time_t right = _buy_move(game, &game->moved_right);
	time_t count = 0;

	game->events |= (EVENT_LEFT * (left > 0)) | (EVENT_RIGHT * (right > 0)) | (EVENT_DOWN * (down > 0));

	if (diagonals && left > 0 && down > 0 && right == 0) {

		count = _move(game, min(left, down), 1, -1);
	}
	else if (diagonals && left == 0 && down > 0 && right > 0) {

		count = _move(game, min(right, down), 1, 1);
	}
	else if (left == 0 && right > 0 && down == 0) {

		count = _move(game, right, 0, 1);
	}
	else if (left > 0 && right == 0 && down == 0) {

		count = _move(game, left, 0, -1);
	}
	else if(left == 0 && right == 0 && down > 0) {

		count = _move(game, max(0, down), 1, 0);
	}

	return count * MOVE_RATE(game);
}

void _init_game(struct Game* game) {

	game->score = 0;
	game->combo = 0;
	game->scored = 0;
	game->time = 0;
	game->last_moved = 0;
	game->last_fell = 0;
	game->repeat = false;
	game->paused = false;
	game->player = 'W';
	game->player_rank = 1;
	game->player_file = 7;
	game->cursor = -1;
	game->cursor_grade = 1;
	game->cursor_increment = LAST_FILE;
	game->dropped = false;
	game->moved_left = -1;
	game->moved_right = -1;
	game->moved_down = -1;
	game->settings = 0;
	game->events = 0;
	game->total_pieces = 0;
	game->total_value = 0;
	init_board(game->board);
	init_captures(game->captures);
}

bool game_over(struct Game* game) {

	return GAME_OVER(game);
}

bool on_brink(struct Game* game) {

	size_t hits = attack(game, false, true, false);
	size_t b = 1;

	if(hits) while (b) {

		if (b & hits) {

			size_t i = _bit_index(b);
			size_t r = i / FILES;
			size_t f = i % FILES;

			if (PIECE_MAP[GET_SQUARE(game->board, SQUARE_INDEX(r, f))] == KING) {

				return true;
			}
		}

		b <<= 1;
	}

	size_t spawn_rank = SPAWN_RANK(game);
	size_t spawn_file = game->cursor_increment;

	if (GET_SQUARE(game->board, SQUARE_INDEX(spawn_rank, spawn_file)) != EMPTY) {

		return true;
	}

	return false;
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

		free_histotrie(game->histotrie);
		MEMLOG("freed game\n");
		free(game);
	}
}

size_t attack(struct Game* game, const bool execute, const bool forecast, const bool pattern) {

	if (game->repeat && IS_SET(game->settings, NO_CAPTURE_ON_REPEAT)) return 0;

	Piece piece = game->player;
	size_t rank = game->player_rank;
	const size_t file = game->player_file;

	if (forecast) {

		rank = SQUARE_RANK(_drop_to(game, SQUARE_INDEX(rank, file)));
		piece = QUEEN_ME(game, rank);
	}

	if (execute) {

		init_captures(game->captures);
	}

	enum Square piece_type = PIECE_MAP[piece];
	return _strike(game, piece_type, rank, file, execute, pattern);
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

void pump(struct Game* game, const time_t passed) {

	if (game_over(game) || game->paused) return;
	printf("PUMP\n");
	game->events = 0;
	game->time += passed;
	game->last_moved += _take_input(game);

	if (!game->moved_left && !game->moved_right && !game->moved_down) {

		game->last_moved = game->time;
	}
	else if (IS_SET(game->settings, FLYING_PIECES) && game->last_fell < game->last_moved) {

		game->last_fell = game->last_moved;
	}

	game->last_fell += _fall(game);
}

void begin(struct Game* game) {

	SPAWN(game);
}

struct Game* malloc_init_game(Settings settings) {

	struct Game* game = malloc(sizeof(struct Game));
	MEMLOG("malloc game\n");
	assert(game);
	_init_game(game);
	game->histotrie = malloc_init_histotrie();
	game->settings = settings;
	return game;
}