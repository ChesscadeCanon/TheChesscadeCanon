#include "../../rules.h"
#include "config.h"
#include "game.h"
#include "histotrie.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __unix__ 
#define max(A, B) (A > B ? A : B)
#define min(A, B) (A < B ? A : B)
#endif

#define DEFAULT_EASE (3000)

Time _default_ease(const struct Game* game) {

	return DEFAULT_EASE;
}

struct Game {

	Bool pause;
	Bool dropped;
	Bool moved_left;
	Bool moved_right;
	Bool moved_down;
	Fraction dragged_left;
	Fraction dragged_right;
	Fraction dragged_down;
	char last_move;
	Count score;
	Count scored;
	Time time;
	Time end_time;
	Time last_moved;
	Time last_fell;
	Time last_spawned;
	Piece player;
	Index player_rank;
	Index player_file;
	Trenary cursor;
	Index cursor_grade;
	Index cursor_increment;
	Piece board[BOARD_LENGTH];
	Piece captures[CAPTURE_LENGTH];
	Bool repeat;
	const Settings settings;
	Events events;
	Count white_pieces;
	Count black_pieces;
	Count total_pieces;
	Count total_value;
	Count fall_count;
	struct Histotrie* histotrie;
	const Time (*ease_functor)(const struct Game* self);
	struct Game* source;
	Count copies;
	Step path[MAX_STEPS];
	Index step;
	Set filters;
};

#define PLAYER_SQUARE(__game__) SQUARE_INDEX(__game__->player_rank, __game__->player_file)
#define SET(__settings__, __setting__) (__settings__ |= __setting__)
#define IS_SET(__settings__, __setting__) (__settings__ & __setting__)
#define BASE_MOVE_RATE (64)
#define MOVE_RATE(__game__) (BASE_MOVE_RATE)
#define SINCE_MOVED(__game__, __passed__) ((__game__->time + __passed__) - (__game__->last_moved))
#define SINCE_FELL(__game__, __passed__) ((__game__->time + __passed__) - __game__->last_fell)
#define SINCE_SPAWNED(__game__, __passed__) ((__game__->time + __passed__) - (__game__->last_spawned))
#define TIME_TAKEN(__game__) (max(__game__->last_spawned, (max(__game__->last_moved, __game__->last_fell))))
#define DRAG(__drag__, __steps__) __drag__ = max(0, __drag__ - (Fraction)__steps__);
#define PLACE_PLAYER(__game__) SET_SQUARE(__game__->board, PLAYER_SQUARE(__game__), __game__->player)
#define REVERSE_CURSOR(__game__) (__game__->cursor *= -1)
#define DOUBLE_BISHOP(__game__) (PIECE_MAP[__game__->player] == BISHOP && IS_SET(__game__->settings, DOUBLE_BISHOPS))
#define BISHOP_SPEED(__game__, __moved__) ((__moved__) / (1 + DOUBLE_BISHOP(__game__)))
#define BISHOP_RATE(__game__, __moved__) ((__moved__) * (1 + DOUBLE_BISHOP(__game__)))
#define PLAYER_DOWN(__game__) SQUARE_INDEX(__game__->player_rank + (DOUBLE_BISHOP(__game__) + 1), __game__->player_file)
#define PLAYER_RIGHT(__game__) SQUARE_INDEX(__game__->player_file, __game__->player_file + (DOUBLE_BISHOP(__game__) + 1))
#define PLAYER_LEFT(__game__) SQUARE_INDEX(__game__->player_file, __game__->player_file - (DOUBLE_BISHOP(__game__) + 1))
#define DROP_RATE(__game__) 0 
#define QUEEN_ME(__game__, __rank__) (\
	IS_SET(__game__->settings, PAWNS_PROMOTE) ?\
		__game__->player == WHITE_PAWN && (__rank__) == 0 ? WHITE_QUEEN \
		: ((__game__->player == BLACK_PAWN && (__rank__) == LAST_RANK) ? BLACK_QUEEN \
		: __game__->player) \
	: __game__->player \
)
#define DRAW_NEXT(__game__) deck(__game__, __game__->cursor_grade, __game__->cursor_increment)
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
	is_piece(GET_SQUARE(__game__->board, __index__)) &&\
	IS_WHITE(GET_SQUARE(__game__->board, __index__)) != IS_WHITE(__game__->player) \
)
#define TALLY_COLORS(__game__, __piece__, __count__) (\
	(__game__->white_pieces += IS_WHITE(__piece__) * __count__) &\
	(__game__->black_pieces += !IS_WHITE(__piece__) * __count__) \
)
#define NEXT_PLAYER(__game__) (__game__->player = NEXT_PIECE(__game__)) 
#define SPAWN(__game__) (\
	TALLY_COLORS(__game__, __game__->player, 1) &\
	NEXT_PLAYER(__game__) &\
	(__game__->player_rank = SPAWN_RANK(__game__)) &\
	(__game__->player_file = __game__->cursor_increment) &\
	(++__game__->total_pieces) &\
	(__game__->total_value += PIECE_VALUE(__game__->player)) \
)
#define PACMAN(__game__, __index__) (abs(SQUARE_FILE(__index__) - (__game__->player_file)) > 2)
#define CAN_STRIKE(__game__, __index__) (ON_BOARD(__game__, __index__ + RAISE_FLOOR(__game__)) && EMPTY_SQUARE(__game__->board, __index__))
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

Bool _can_move(const struct Game* game, const Index rank, const Index file) {

	const Index square = SQUARE_INDEX(rank, file);

	if (!CAN_STRIKE(game, square)) {

		return False;
	}

	if (PACMAN(game, square)) {

		return False;
	}

	if (DOUBLE_BISHOP(game)) {

		if (IS_WHITE(game->player) != SQUARE_IS_WHITE(square)) {

			return False;
		}
	}

	return True;
}

Index _update_cursor(struct Game* game) {

	const short inc = CURSOR_INCREMENT(game);
	const Bool wrapped = abs(inc) > 1;
	game->cursor_increment += inc;
	game->cursor_grade = CURSOR_GRADE(game, wrapped || CURSOR_WRAPPED(game));
	game->events |= EVENT_WRAPPED * wrapped;
	return game->cursor;
}

Index _bit_index(Set bit) {

	Index i = 0;
	Set b = 1;

	while (i < 64) {

		if (bit & b) break;
		++i;
		b <<= 1;
	}

	return i >= 64 ? 0 : i;
}

void _kill(struct Game* game, const Index square, const Index move) {

	Piece piece = GET_SQUARE(game->board, square);
	TALLY_COLORS(game, piece, -1);
	SET_CAPTURE(game->captures, move, piece);
	SET_SQUARE(game->board, square, EMPTY);
}

Set _capture(struct Game* game, const Index square, const Index move, const Bool execute) {

	if (execute) {

		_kill(game, square, move);
	}

	return SQUARE_BIT(square);
}

Set _hit(struct Game* game, const enum Square piece_type, const Index rank, const Index file, const Index move, const Bool execute, const Bool pattern) {

	struct MoveSet move_set = MOVES[piece_type];
	const short invert = piece_type == PAWN && IS_WHITE(game->player) && IS_SET(game->settings, WHITE_PAWN_HIT_UP) ? -1 : 1;
	Index to_rank = rank;
	Index to_file = file;
	Bool open = True;
	Set ret = 0;

	while (open) {

		to_rank += move_set.moves[move][0] * invert;
		to_file += move_set.moves[move][1];
		const Index square = SQUARE_INDEX(to_rank, to_file);
		open = CAN_STRIKE(game, square);
		ret |= open && pattern ? SQUARE_BIT(square) : 0;
		if (CAN_CAPTURE(game, square)) {

			ret |= _capture(game, square, move, execute);
		}
		if (!move_set.repeat) break;
	}

	return ret;
}

Set _strike(struct Game* game, const enum Square piece_type, const Index rank, const Index file, const Bool execute, const Bool pattern) {

	const struct MoveSet move_set = MOVES[piece_type];
	Set ret = 0;
	for (Index move = 0; move < move_set.count; ++move) {

		ret |= _hit(game, piece_type, rank, file, move, execute, pattern);
	}
	return ret;
}

Index _drop_to(const struct Game* game, const Index from) {

	Index to = SQUARE_DOWN(from);

	if (DOUBLE_BISHOP(game)) {

		to = SQUARE_DOWN(to);
	}

	if (_can_move(game, SQUARE_RANK(to), SQUARE_FILE(to))) {

		return _drop_to(game, to);
	}

	return from;
}

void _checkmate(struct Game* game) {

	if (!IS_SET(game->settings, CHECKMATE)) return;

	for (Index i = 0; i < FILES; ++i) {

		Piece piece = GET_CAPTURE(game->captures, i);

		if (PIECE_MAP[piece] == KING) {

			game->player = DEAD_PLAYER;
		}
	}
}

void _judge(struct Game* game) {

	const Bool white = IS_WHITE(game->player);
	const Count gain = white ? 1 + game->player_rank : RANKS - game->player_rank;
	game->scored = white ? game->white_pieces : game->black_pieces;
	Count count = 0;

	for (Index i = 0; i < FILES; ++i) {

		Piece piece = GET_CAPTURE(game->captures, i);

		if (piece != *EMPTY_CAPTURES) {

			game->scored += PIECE_VALUE(piece);
			++count;
		}
	}

	game->scored *= gain + count;
	game->score += game->scored;
}

Count _chronicle(struct Game* game) {

	if (!game->histotrie) return False;
	if (!IS_SET(game->settings, NO_CAPTURE_ON_REPEAT | KING_ON_REPEAT)) return False;

	const Count ret = record_state(game->histotrie, game->board, 0);
	MEMLOGF("created %llu histotrie nodes\n", ret);
	game->repeat = !ret;
	return ret;
}

void _assert_on_time(const struct Game* game, const Time time) {

	if (TIME_TAKEN(game) > time) {

		//assert(False);
	}
}

void _set_last_moved(struct Game* game, const Time time, const Time to) {

	game->last_moved = to;
	_assert_on_time(game, time);
}

void _set_last_spawned(struct Game* game, const Time passed, const Time to) {

	game->last_spawned = to;
	_assert_on_time(game, game->time + passed);
}

void _set_last_fell(struct Game* game, const Time passed, const Time to) {

	game->last_fell = to;
	_assert_on_time(game, game->time + passed);
}

void _resolve(struct Game* game, const Time passed, const Time time_spent) {

	const Index length = path_length(game);

	if (length) {

		if (path_length(game) <= game->step) {

			assert(False);
		}

		if (game->path[game->step + 1] && game->path[game->step + 1] != DOWN) {

			assert(False);
		}
	}

	if (_can_move(game, SQUARE_RANK(PLAYER_DOWN(game)), game->player_file)) {

		assert(False);
	}

	const Time time = TIME_TAKEN(game) + time_spent;
	game->events |= EVENT_LANDED;
	init_path(game->path);
	game->step = 0;
	game->last_move = LANDED;
	const Index from_rank = game->player_rank, from_file = game->player_file;
	game->player = QUEEN_ME(game, from_rank);
	const Set captures = attack(game, True, False, False);
	_judge(game);
	LAND(game);
	_checkmate(game);
	_chronicle(game);
	game->cursor_grade = CURSOR_GRADE(game, game->repeat && IS_SET(game->settings, KING_ON_REPEAT));

	if (captures) {
		
		game->events |= EVENT_CAPTURE;
		REVERSE_CURSOR(game);
	}

	_set_last_spawned(game, passed, time);
	_set_last_fell(game, passed, time);

	if (game->end_time < 0 && game_over(game)) {

		game->end_time = time;
	}
}

Bool _move_player(struct Game* game, const Index to, const Time passed, const Time time_spent) {

	const Index to_rank = SQUARE_RANK(to), to_file = SQUARE_FILE(to);

	if (_can_move(game, to_rank, to_file)) {

		game->player_rank = to_rank;
		game->player_file = to_file;
		return True;
	}
	else if (to_file != game->player_file) {

		return False;
	}
	else if (to_rank > game->player_rank && to_rank <= RANKS + 1) {

		_resolve(game, passed, time_spent);
	}

	return False;
}

Time _buy_move(struct Game* game, const Bool moved, const Time passed) {

	if (!moved) return 0;
	const Time rate = MOVE_RATE(game);
	const Time since = SINCE_MOVED(game, passed);
	const Time steps = game->path[0] || !game->histotrie ? 1 : since / rate;
	assert(steps >= 0);
	return steps;
}

Time _move(struct Game* game, const Time passed, Time steps, const short by_rank, const short by_file) {

	if (steps == 0) return 0;
	const Bool orthogonal = !by_rank != !by_file;
	const short multiplier = max(1, (DOUBLE_BISHOP(game) + orthogonal));
	Time ret = 0;

	for (Time s = 0; s < steps; ++s) {

		const short by_ranks = by_rank * multiplier;
		if (abs(by_ranks) > game->player_rank && by_ranks < 0) break;
		const short by_files = by_file * multiplier;
		if (abs(by_files) > game->player_file && by_files < 0) break;
		const Index to_rank = game->player_rank + by_ranks;
		const Index to_file = game->player_file + by_files;
		const Index to = SQUARE_INDEX(to_rank, to_file);
		const Time time_spent = (ret + 1) * MOVE_RATE(game);
		if (!_move_player(game, to, passed, time_spent)) break;
		++ret;
	};

	return ret;
}

Bool _drop(struct Game* game, const Time passed) {

	const Index from = PLAYER_SQUARE(game);
	const Index to = _drop_to(game, from);
	const Bool ret = to != from;
	_move_player(game, to, passed, 0);
	_move(game, passed, 1, 1, 0);
	game->events |= EVENT_DROPPED * ret;
	return ret;
}

Time _fall(struct Game* game, const Time passed) {

	assert(game);
	const Time falls = SINCE_FELL(game, passed) / ease(game);
	Time ret = 0;
	
	for (Time f = 0; f < falls; ++f) {

		const Index rank = game->player_rank + (DOUBLE_BISHOP(game) + 1);
		const Index file = game->player_file;
		const Index to = SQUARE_INDEX(rank, file);
		ret += ease(game);
		++game->fall_count;

		if (_move_player(game, to, passed, ret)) {

			game->events |= EVENT_FELL;
			game->last_move = FALL;
			_update_cursor(game);
		}
		else break;
	}

	return ret;
}

Time _do_move(struct Game* game, const Time passed, const Time left, const Time right, Time down) {

	Time ret = 0;
	const Bool diagonals = IS_SET(game->settings, DIAGONALS);

	game->events |= (EVENT_LEFT * (left > 0)) | (EVENT_RIGHT * (right > 0)) | (EVENT_DOWN * (down > 0));

	if (diagonals && left > 0 && down > 0 && right == 0) {

		const Time steps = min(left, down);
		game->last_move = DOWN_LEFT;
		ret = _move(game, passed, steps, 1, -1);
		DRAG(game->dragged_left, ret);
		DRAG(game->dragged_down, ret);
	}
	else if (diagonals && left == 0 && down > 0 && right > 0) {

		const Time steps = min(right, down);
		game->last_move = DOWN_RIGHT;
		ret = _move(game, passed, steps, 1, 1);
		DRAG(game->dragged_right, ret);
		DRAG(game->dragged_down, ret);
	}
	else if (left == 0 && right > 0 && down == 0) {
		
		game->last_move = RIGHT;
		ret = _move(game, passed, right, 0, 1);
		DRAG(game->dragged_right, ret);
	}
	else if (left > 0 && right == 0 && down == 0) {

		game->last_move = LEFT;
		ret = _move(game, passed, left, 0, -1);
		DRAG(game->dragged_left, ret);
	}
	else if (left == 0 && right == 0 && down > 0) {
		
		game->last_move = DOWN;
		ret = _move(game, passed, max(0, down), 1, 0);
		if (ret) game->moved_down = False;
		DRAG(game->dragged_down, ret);
	}

	return ret;
}

void _take_move(struct Game* game, Time passed) {

	const Bool any_move = game->moved_down || game->moved_left || game->moved_right;
	if (any_move) {

		if (game->last_moved < 0) {

			_set_last_moved(game, game->time + passed, game->time);
		}

		const Time down = _buy_move(game, game->moved_down, passed);
		const Time left = _buy_move(game, game->moved_left, passed);
		const Time right = _buy_move(game, game->moved_right, passed);
		const Time count = _do_move(game, passed, left, right, down);
		const Time move_time = count * MOVE_RATE(game);

		_set_last_moved(game, game->time + passed, game->last_moved + move_time);

		if (IS_SET(game->settings, FLYING_PIECES) && game->last_fell < game->last_moved) {

			_set_last_fell(game, game->time + passed, game->last_moved);
		}
	} else {

		game->last_moved = -1;
	}

	const Time fall_time = _fall(game, passed);
	_set_last_fell(game, game->time + passed, game->last_fell + fall_time);
}

void init_path(Step path[MAX_STEPS]) {

	memset(path, LANDED, sizeof(Step) * MAX_STEPS);
}

void take_path(struct Game* game, const Step path[MAX_STEPS])
{
	memcpy(game->path, path, MAX_STEPS * sizeof(Step));
}

void _take_step(struct Game* game, const Step step)
{
	const Count open = strnlen(game->path, MAX_STEPS);
	assert(open < MAX_STEPS);
	assert(open >= 0);
	game->path[open] = step;
	game->step = (Index) open;
}

void _init_game(struct Game* game) {

	game->score = 0;
	game->scored = 0;
	game->time = 0;
	game->last_moved = -1;
	game->last_fell = 0;
	game->last_move = LANDED;
	game->last_spawned = 0;
	game->end_time = -1;
	game->repeat = False;
	game->pause = False;
	game->player = 'W';
	game->player_rank = 1;
	game->player_file = 7;
	game->cursor = -1;
	game->cursor_grade = 1;
	game->cursor_increment = LAST_FILE;
	game->dropped = False;
	game->moved_left = 0;
	game->moved_right = 0;
	game->moved_down = 0;
	game->dragged_left = 0;
	game->dragged_right = 0;
	game->dragged_down = 0;
	game->events = 0;
	game->white_pieces = 0;
	game->black_pieces = 0;
	game->total_pieces = 0;
	game->total_value = 0;
	game->fall_count = 0;
	game->source = NULL;
	game->copies = 1;
	game->step = 0;
	game->filters = 0;
	init_board(game->board);
	init_captures(game->captures);
	init_path(game->path);
}

Bool game_over(const struct Game* game) {

	return GAME_OVER(game);
}

Bool _will_checkmate(struct Game* game) {

	const Set hits = attack(game, False, True, False);
	Set bit = 1;
	Index index = 0;

	if (hits) while (bit) {

		if (bit & hits) {

			const Index rank = index / FILES;
			const Index file = index % FILES;

			if (PIECE_MAP[GET_SQUARE(game->board, SQUARE_INDEX(rank, file))] == KING) {

				return True;
			}
		}

		bit <<= 1;
		++index;
	}

	return False;
}

Bool _will_overflow(const struct Game* game) {

	const Index rank = SPAWN_RANK(game);
	const Index file = game->cursor_increment;
	const Index index = SQUARE_INDEX(rank, file);
	const Index square = GET_SQUARE(game->board, index);
	return square != EMPTY;
}

void _go_way(struct Game* game, const Step way) {

	switch (way) {

	case DOWN:
		do_digital_move(game, 0, 0, 1);
		break;
	case LEFT:
		do_digital_move(game, 1, 0, 0);
		break;
	case RIGHT:
		do_digital_move(game, 0, 1, 0);
		break;
	case DOWN_LEFT:
		do_digital_move(game, 1, 0, 1);
		break;
	case DOWN_RIGHT:
		do_digital_move(game, 0, 1, 1);
		break;
	case FALL:
		break;
	default:
		break;
	}
}

Bool on_brink(struct Game* game) {

	return _will_overflow(game) || _will_checkmate(game);
}

void toggle_pause(struct Game* game) {

	game->pause = !game->pause;
}

Bool paused(const struct Game* game) {

	return game->pause;
}

Bool repeated(const struct Game* game) {

	return game->repeat;
}

void do_drop(struct Game* game) {

	game->dropped = True;
}

void do_digital_move(struct Game* game, Bool left, Bool right, Bool down) {

	if (left && down && !(game->moved_left && game->moved_down && !game->moved_right)) {

		_set_last_moved(game, game->time, game->time);
	}
	else if (right && down && !(game->moved_right && game->moved_down && !game->moved_left)) {

		_set_last_moved(game, game->time, game->time);
	}
	else if (right && !down && (game->moved_left || game->moved_down)) {

		_set_last_moved(game, game->time, game->time);
	}
	else if (left && !down && (game->moved_right || game->moved_down)) {

		_set_last_moved(game, game->time, game->time);
	}
	else if (down && !left && !right && (game->moved_right || game->moved_left)) {

		_set_last_moved(game, game->time, game->time);
	}

	game->moved_left = left;
	game->moved_right = right;
	game->moved_down = down;
}

void do_analog_move(struct Game* game, Fraction x, Fraction y) {

	if (x > 0) {

		game->dragged_left = 0;
		game->dragged_right += x;
	}
	else if (x < 0) {

		game->dragged_left -= x;
		game->dragged_right = 0;
	}
	else {

		game->dragged_left = 0;
		game->dragged_right = 0;
	}

	if (y > 0) {

		game->dragged_down += y;
	}
	else {

		game->dragged_down = 0;
	}
}

Events current_events(const struct Game* game) {

	return game->events;
}

Piece player_piece(const struct Game* game) {

	return game->player;
}

Index player_piece_rank(const struct Game* game) {

	return game->player_rank;
}

Index player_piece_file(const struct Game* game) {

	return game->player_file;
}

Trenary cursor_direction(const struct Game* game) {

	return game->cursor;
}

Index current_cursor_grade(const struct Game* game) {

	return game->cursor_grade;
}

Index current_cursor_increment(const struct Game* game) {

	return game->cursor_increment;
}

Count current_score(const struct Game* game) {

	return game->score;
}

Count last_scored(const struct Game* game) {

	return game->scored;
}

Time milliseconds(const struct Game* game) {

	return game->time;
}

Time move_rate(const struct Game* game)
{
	if (!game) return BASE_MOVE_RATE;
	return MOVE_RATE(game);
}

Index spawn_rank(const struct Game* game) {

	return SPAWN_RANK(game);
}

Count falls(const struct Game* game)
{
	return game->fall_count;
}

Time time_taken(const struct Game* game)
{
	return TIME_TAKEN(game);
}

const struct Game* get_source(const struct Game* game)
{
	return game->source;
}

char last_move(const struct Game* game)
{
	return game->last_move;
}

const char* current_path(const struct Game* game)
{
	return game->path;
}

Index path_length(const struct Game* game)
{
	return (Index)strnlen(game->path, MAX_STEPS);
}

void follow_path(struct Game* game) {

	const enum Event event_moved = EVENT_LEFT | EVENT_RIGHT | EVENT_DOWN | EVENT_FELL;
	const enum Event events = current_events(game);
	const enum Event event_down_left = EVENT_LEFT | EVENT_DOWN;
	const enum Event event_down_right = EVENT_RIGHT | EVENT_DOWN;
	Index length = path_length(game);
	
	if (event_moved & events) {

		Bool done = False;
		Bool wrong = False;
		const Step step = game->path[game->step];
		switch (step) {

		case LEFT:
			done = EVENT_LEFT & events;
			break;
		case RIGHT:
			done = EVENT_RIGHT & events;
			break;
		case DOWN:
			done = EVENT_DOWN & events;
			if (game->step + 1 == length) done |= EVENT_FELL & events;
			break;
		case FALL:
			done = EVENT_FELL & events;
			break;
		case DOWN_LEFT:
			done = (event_down_left & events) == event_down_left;
			break;
		case DOWN_RIGHT:
			done = (event_down_right & events) == event_down_right;
			break;
		default: break;
		}

		wrong = !done && (events & event_moved);

		if (wrong) {

			assert(False);
		}
		else if (done) {

			if (game->step < length - 1) {

				++game->step;
				assert(game->step < MAX_STEPS);
			}
		}
	}

	Step way = game->path[game->step];
	_go_way(game, way);
}

Step get_step(const struct Game* game, const Index step)
{
	if (step >= path_length(game)) {

		assert(False);
	}

	return game->path[step];
}

void set_filters(struct Game* game, const Set filters)
{
	game->filters = filters;
}

Set get_filters(const struct Game* game)
{
	return game->filters;
}

Bool cursor_wrapped(const struct Game* game) {

	return CURSOR_WRAPPED(game);
}

const Board board_state(const struct Game* game) {

	return (const Board)game->board;
}

Index board_length() {

	return BOARD_LENGTH;
}

Piece square_contents(const struct Game* game, const Index rank, const Index file) {

	return GET_SQUARE(game->board, SQUARE_INDEX(rank, file));
}

Set square_bit(Index rank, Index file) {

	return SQUARE_BIT(SQUARE_INDEX(rank, file));
}

Time ease(const struct Game* game) {

	if (!game || !game->ease_functor) return DEFAULT_EASE;
	return game->ease_functor(game);
}

Time ended(const struct Game* game) {

	return game->end_time;
}

const Piece deck(const struct Game* game, const Index grade, const Index increment) {

	const char ret = DECKS[grade][increment];
	return ret;
}

const Set figure(const Piece piece, const enum Layer layer, const Index index)
{
	if (is_piece(piece) && layer >= 0 && layer < LAYERS_DEEP && index < FIGURE_HEIGHT) {

		return FIGURES[PIECE_MAP[piece]][layer][index];
	}

	return 0;
}

Piece next_piece(const struct Game* game) {

	return NEXT_PIECE(game);
}

Bool free_game(struct Game* game) {

	if (game) {

		if (--game->copies > 0) return False;
		if(free_game(game->source)) game->source = NULL;
		free_histotrie(game->histotrie);
		MEMLOG("freed game\n");
		free(game);
	}

	return True;
}

Bool free_game_shallow(struct Game* game) {

	if (game) {

		if (--game->copies > 0) return False;
		if (free_game_shallow(game->source)) {
			game->source = NULL;
		}
		MEMLOG("freed game\n");
		free(game);
	}

	return True;
}

Set attack(struct Game* game, const Bool execute, const Bool forecast, const Bool pattern) {

	if (game->repeat && IS_SET(game->settings, NO_CAPTURE_ON_REPEAT)) return 0;

	Piece piece = game->player;
	Index rank = game->player_rank;
	const Index file = game->player_file;

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

Index forecast_rank(const struct Game* game) {

	const Index rank = game->player_rank;
	const Index file = game->player_file;
	return SQUARE_RANK(_drop_to(game, SQUARE_INDEX(rank, file)));
}

char forecast_piece(const struct Game* game) {

	const Index rank = forecast_rank(game);
	return QUEEN_ME(game, rank);
}

void _take_drag(struct Game* game) {

	if (game->dragged_right >= 1.0) {

		game->moved_right = True;
	}
	else if (game->dragged_left >= 1.0) {

		game->moved_left = True;
	}

	if (game->dragged_down >= 1.0) {

		game->moved_down = True;
	}
}

void _take_drop(struct Game* game, Time passed) {

	if (game->dropped && SINCE_SPAWNED(game, passed) >= DROP_RATE(game)) {

		if (_drop(game, passed)) {

			game->last_moved = game->last_fell = game->time;
		}
	}

	game->dropped = False;
}

void _pump_recursive(struct Game* game, Time passed) {

	if (game_over(game)) {
		return;
	}

	_take_drop(game, passed);
	_take_drag(game);
	_take_move(game, passed);

	if (game_over(game)) {
		return;
	}

	const Bool moved = game->moved_left || game->moved_right || game->moved_down;
	const Bool time_to_move = SINCE_MOVED(game, passed) >= move_rate(game) && moved;
	const Bool time_to_fall = SINCE_FELL(game, passed) >= ease(game);

	game->moved_left = game->moved_right = game->moved_down = False;
}

void pump(struct Game* game, const Time passed) {

	if (game_over(game) || game->pause) return;

	game->events = 0;
	_pump_recursive(game, passed);
	game->time += passed;

	_assert_on_time(game, game->time);
}

void begin(struct Game* game) {

	SPAWN(game);
}

void print_board_state(const struct Game* game) {

	printf("%s\n", game->board);
}

struct Game* malloc_init_game(const Settings settings) {

	const struct Game game = {.settings = settings, .ease_functor = _default_ease};
	struct Game* ret = malloc(sizeof(struct Game));
	assert(ret);
	memcpy(ret, &game, sizeof(struct Game));
	MEMLOG("malloc game\n");
	_init_game(ret);
	ret->histotrie = malloc_init_histotrie();
	return ret;
}

struct Game* malloc_init_standard_game_with_ease_functor(EASE_FUNCTOR(ease_func)) {

	const struct Game game = { .settings = STANDARD_SETTINGS, .ease_functor = ease_func };
	struct Game* ret = malloc(sizeof(struct Game));
	assert(ret);
	memcpy(ret, &game, sizeof(struct Game));
	MEMLOG("malloc game\n");
	_init_game(ret);
	ret->histotrie = malloc_init_histotrie();
	return ret;
}

struct Game* malloc_init_game_shallow_copy(struct Game* game)
{
	struct Game* ret = malloc(sizeof(struct Game));
	assert(ret);
	memcpy(ret, game, sizeof(struct Game));
	MEMLOG("malloc game\n");
	ret->histotrie = NULL;
	ret->source = game;
	ret->copies = 1;
	game->copies++;
	return ret;
}

struct Game* malloc_init_game_moved_copy(struct Game* previous, const Step way)
{
	struct Game* next = malloc_init_game_shallow_copy(previous);

	_take_step(next, way);
	_go_way(next, way);

	if (way == FALL) {

		pump(next, ease(next));
	}
	else {

		pump(next, move_rate(next));
	}

	if (!path_length(next)) {

		take_path(next, previous->path);
		_take_step(next, DOWN);
	}
	
	return next;
}

struct Game* malloc_init_game_moved_copy_or_null(struct Game* previous, const Step way)
{
	struct Game* next = malloc_init_game_moved_copy(previous, way);
	const Index
		r = player_piece_rank(previous),
		f = player_piece_file(previous),
		R = player_piece_rank(next),
		F = player_piece_file(next);
	const Bool L = last_move(next) == LANDED;

	Bool good = True;
	switch (way) {

	case LEFT:
		good = r == R && f > F; break;
	case RIGHT:
		good = r == R && f < F; break;
	case DOWN_LEFT:
		good = r < R && f > F; break;
	case DOWN_RIGHT:
		good = r < R && f < F; break;
	case FALL:
		good = r < R && f == F; break;
	case DOWN:
		good = r < R && f == F || L; break;
	default: break;
	}

	if (good) {

		return next;
	}

	free_game_shallow(next);
	return NULL;
}
