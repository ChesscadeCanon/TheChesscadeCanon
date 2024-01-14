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

#define MOVE_RATE(G) 64
#define FPS 60
#define PLACE_PLAYER(G) SET_SQUARE(G->board, PLAYER_SQUARE(G), G->player)
#define REVERSE_CURSOR(G) (G->cursor *= -1)
#define PLAYER_SQUARE(G) SQUARE_INDEX(G->player_rank, G->player_file)
#define DOUBLE_BISHOP(G) (PIECE_MAP[G->player] == BISHOP && IS_SET(G->settings, DOUBLE_BISHOPS))
#define BISHOP_SPEED(G, M) ((M) / (1 + DOUBLE_BISHOP(G)))
#define PLAYER_DOWN(G) SQUARE_INDEX(G->player_rank + (DOUBLE_BISHOP(G) + 1), G->player_file)
#define EASE(G) max(1, 1024 >> G->combo) * (1 + DOUBLE_BISHOP(G))
#define QUEEN_ME(G, R) (\
	IS_SET(G->settings, PAWNS_PROMOTE) ?\
		G->player == WHITE_PAWN && (R) == 0 ? WHITE_QUEEN \
		: ((G->player == BLACK_PAWN && (R) == LAST_RANK) ? BLACK_QUEEN \
		: G->player) \
	: G->player \
)
#define DRAW_NEXT(G) DECKS[G->cursor_grade][G->cursor_increment]
#define SPAWN_RANK(G) (IS_SET(G->settings, BLACK_PAWN_SPAWN_LOW) && DRAW_NEXT(G) == BLACK_PAWN ? 1 : 0)
#define NEXT_PIECE(G) (\
	EMPTY_SQUARE(G->board, SQUARE_INDEX(SPAWN_RANK(G), G->cursor_increment)) ?\
		DRAW_NEXT(G) \
	:\
		DEAD_PLAYER \
)
#define IN_BOUNDS(V, L, H) (V >= L && V < H)
#define RAISE_FLOOR(G) (IS_SET(G->settings, WHITE_PAWN_LAND_HIGH) && (G->player == WHITE_PAWN) ? LINE_LENGTH : 0)
#define ON_BOARD(G, I) (IN_BOUNDS(I, 0, BOARD_LENGTH) && G->board[I] != '\n' && G->board[I])
#define CAN_CAPTURE(G, I) (\
	ON_BOARD(G, I) &&\
	IS_PIECE(GET_SQUARE(G->board, I)) &&\
	IS_WHITE(GET_SQUARE(G->board, I)) != IS_WHITE(G->player) &&\
	(!G->repeat) \
)
#define NEXT_PLAYER(G) (G->player = NEXT_PIECE(G)) 
#define SPAWN(G) (\
	NEXT_PLAYER(G) &\
	(G->player_rank = SPAWN_RANK(G)) &\
	(G->player_file = G->cursor_increment) \
)
#define PACMAN(G, I) (abs(SQUARE_FILE(I) - (G->player_file)) > 2)
#define CAN_STRIKE(G, I) (ON_BOARD(G, I + RAISE_FLOOR(G)) && EMPTY_SQUARE(G->board, I))
#define CAN_MOVE(G, I) (CAN_STRIKE(G, I) && !PACMAN(G, I))
#define CURSOR_WRAPPED(G) (G->cursor_grade > 1)
#define CURSOR_GRADE(G, K) (!HAS_CAPTURED(G->captures) + (K) * 2)
#define CURSOR_INCREMENT(G) (\
	(G->cursor > 0 && G->cursor_increment < LAST_FILE) ||\
	(G->cursor < 0 && G->cursor_increment > 0) ?\
	G->cursor : -G->cursor * LAST_FILE \
)
#define LAND(G) (\
	PLACE_PLAYER(G) &\
	SPAWN(G) \
)
#define GAME_OVER(G) (G->player == DEAD_PLAYER)

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
	const short reverse = piece_type == PAWN && IS_WHITE(game->player) && IS_SET(game->settings, WHITE_PAWN_HIT_UP) ? -1 : 1;
	const size_t to_rank = rank + move_set.moves[move][0] * reverse;
	const size_t to_file = file + move_set.moves[move][1];
	const size_t square = SQUARE_INDEX(to_rank, to_file);
	const bool open = CAN_STRIKE(game, square);
	size_t ret = open && pattern ? SQUARE_BIT(square) : 0;

	if (move_set.repeat && open) {

		return ret | _hit(game, piece_type, to_rank, to_file, move, execute, pattern);
	}
	else if (CAN_CAPTURE(game, square)) {

		return ret | _capture(game, square, move, execute);
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
	printf("%s\n", game->board);
	_chronicle(game);
	if (game->repeat) {
		printf("Repeat!\n");
	}
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

time_t _buy_move(struct Game* game, long int* moved, const unsigned short multiplier) {

	if (*moved < 0) return 0;
	const long int rate = MOVE_RATE(game) * multiplier;
	const long int steps = *moved / rate;
	assert(steps >= 0);
	*moved -= rate * steps;
	return (time_t)steps;
}

void _drop(struct Game* game) {

	const size_t from = PLAYER_SQUARE(game);
	const size_t to = _drop_to(game, from);
	_move_player(game, to);
	game->events = EVENT_DROPPED;
}

void _move(struct Game* game, time_t steps, const short by_rank, const short by_file) {

	if (steps == 0) return;
	const bool orthogonal = !by_rank != !by_file;
	const size_t mult = (DOUBLE_BISHOP(game) + orthogonal);
	const size_t to_rank = game->player_rank + (by_rank * mult);
	const size_t to_file = game->player_file + (by_file * mult);
	const size_t to = SQUARE_INDEX(to_rank, to_file);
	if (_move_player(game, to)) {

		if (IS_SET(game->settings, FLYING_PIECES)) {

			game->fell += (game->time - game->fell) >> 1;
		}
		_move(game, steps - 1, by_rank, by_file);
	}
}

void _fall(struct Game* game, time_t falls) {

	if (falls <= 0) return;
	size_t to = SQUARE_INDEX(game->player_rank + (DOUBLE_BISHOP(game) + 1), game->player_file);
	
	if (_move_player(game, to)) {

		game->events = EVENT_FELL;
		_update_cursor(game);
		_fall(game, falls - 1);
	}
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

	game->events |= (EVENT_LEFT * (left > 0)) | (EVENT_RIGHT * (right > 0)) | (EVENT_DOWN * (down > 0));

	if (diagonals && left > 0 && down > 0 && right == 0) {

		_move(game, min(left - right, down), 1, -1);
	}
	else if (diagonals && left == 0 && down > 0 && right > 0) {

		_move(game, min(right - left, down), 1, 1);
	}
	else if (left > 0 || right > 0 || down > 0) {

		_move(game, max(0, right - left), 0, 1);
		_move(game, max(0, left - right), 0, -1);
		_move(game, max(0, down), 1, 0);
	}
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
	game->cursor_grade = 1;
	game->cursor_increment = LAST_FILE;
	game->dropped = false;
	game->moved_left = -1;
	game->moved_right = -1;
	game->moved_down = -1;
	game->settings = 0;
	game->events = 0;
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

void exist(struct Game* game, const time_t falls) {

	game->events = 0;
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
	game->histotrie = malloc_init_histotrie();
	game->settings = settings;
	return game;
}