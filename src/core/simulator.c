#include "simulator.h"
#include "game.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

const char WAYS[WAY_COUNT + 1] = { DOWN, FALL, RIGHT, LEFT, DOWN_LEFT, DOWN_RIGHT, LANDED };
#define MAX_FLAGS 16
#define FILTER_FUNCTOR(VAR) Rating (*VAR)(struct Game*)

#define IS_LEAF(__game__) (current_events(__game__) & EVENT_LANDED)
#define STEP_GAME(__game__) (\
	SQUARE_STEP(\
		player_piece_rank(__game__),\
		player_piece_file(__game__),\
		current_cursor_increment(__game__)\
	)\
)

Rating _filter_kings(const struct Game* game) {

	return PIECE_MAP[player_piece(game)] != KING;
}

Rating _filter_grade(const struct Game* game) {

	return current_cursor_grade(game) < 2;
}

Rating _filter_losses(const struct Game* game) {

	return (Rating) !game_over(game);
}

Rating _filter_score(const struct Game* game) {

	return current_score(game);
}

Rating _filter_direction(const struct Game* game) {

	const Index increment = current_cursor_increment(game);
	const Index direction = cursor_direction(game);
	return direction > 0 && increment < 4 || direction < 0 && increment > 3;
}

Rating _filter_cursor(const struct Game* game) {

	const struct Game* source = get_source(game);
	if (!source) {

		assert(False);
	}
	
	const Bool white = IS_WHITE(player_piece(source));
	const Rating increment = (Rating)current_cursor_increment(game);
	const grade = current_cursor_grade(source);
	const Rating target = grade % 2 == 0 ? white ? 3 : 4 : white ? 4 : 3;
	return -abs(target - increment);
}

Rating _filter_rank(const struct Game* game) {

	const struct Game* source = get_source(game);
	if (!source) {

		assert(False);
	}

	return player_piece_rank(game);
}

void _init_path(Step path[MAX_STEPS]) {

	memset(path, LANDED, MAX_STEPS * sizeof(Step));
}

void _init_field(struct Game* field[STEPS]) {

	memset(field, 0, STEPS * sizeof(struct Game*));
}

void _free_field(struct Game* field[STEPS]) {

	for (Count i = 0; i < STEPS; ++i) {

		if(field[i]) free_game_shallow(field[i]);
	}
}

void _search(struct Game* game, struct Game* leaves[STEPS]) {

	struct Game* discovered[STEPS];
	struct Game* visited[STEPS];
	Count discoveries = 1;
	Count open = 0;
	_init_field(discovered);
	_init_field(visited);
	_init_field(leaves);
	visited[STEP_GAME(game)] = discovered[open++] = malloc_init_game_shallow_copy(game);

	for (Count start = 0, limit = 1; discoveries > 0;) {

for (Count d = start; d < limit; ++d) {

	struct Game* previous = discovered[d];
	assert(previous);

	const Count had = STEP_GAME(previous);

	for (Count w = 0; w < WAY_COUNT; ++w) {

		Step way = WAYS[w];
		struct Game* next = malloc_init_game_moved_copy_or_null(previous, way);

		if (!next) continue;

		const Count has = STEP_GAME(next);
		const struct Game* was = visited[has];

		if (IS_LEAF(next) && !leaves[had]) {

			leaves[had] = next;
		}
		else if (was) {

			free_game_shallow(next);
			continue;
		}
		else {

			visited[has] = next;
			discovered[open++] = next;
			++discoveries;
		}
	}

	discovered[d] = NULL;
	--discoveries;
}

start = limit;
limit = open;
	}

	_free_field(visited);
}

Bool _pick_leaf(struct Game* game, struct Game* leaves[STEPS], const Count pick) {

	const struct Game* leaf = leaves[pick];

	if (leaf) {

		const Step* path = current_path(leaf);
		take_path(game, path);
		return True;
	}

	return False;
}

void _select_random(struct Game* game, struct Game* leaves[STEPS]) {

	struct Game* buffer[STEPS];
	Count to = 0;
	for (Count from = 0; from < STEPS; ++from) {

		if (leaves[from]) {

			buffer[to++] = leaves[from];
		}
	}

	if (to) {

		_pick_leaf(game, buffer, rand() % to);
	}
}

void _select_first(struct Game* game, struct Game* leaves[STEPS]) {

	for (Index p = 0; p < STEPS; ++p) {

		if (_pick_leaf(game, leaves, p)) return;
	}
}

void _apply_filter(struct Game* leaves[STEPS], FILTER_FUNCTOR(filter_functor)) {

	Rating best = INT_MIN;
	for (Count l = 0; l < STEPS; ++l) {

		struct Game* leaf = leaves[l];
		if (leaf) {

			const Rating result = filter_functor(leaf);
			best = max(best, result);
		}
	}

	for (Count l = 0; l < STEPS; ++l) {

		struct Game* leaf = leaves[l];
		if (leaf) {

			const Rating result = filter_functor(leaf);

			if (result < best) {

				free_game_shallow(leaf);
				leaves[l] = NULL;
			}
		}
	}
}

Bool _apply_filter_flag(struct Game* leaves[STEPS], const char flag) {

	switch (flag) {

	case '0': return False;
	case '1': _apply_filter(leaves, _filter_losses); return True;
	case '2': _apply_filter(leaves, _filter_kings); return True;
	case '3': _apply_filter(leaves, _filter_score); return True;
	case '4': _apply_filter(leaves, _filter_direction); return True;
	case '5': _apply_filter(leaves, _filter_cursor); return True;
	case '6': _apply_filter(leaves, _filter_rank); return True;
	case '7': _apply_filter(leaves, _filter_grade); return True;
	default: return False;
	}
}

void _filter(struct Game* leaves[STEPS], const Set filters) {

	char buffer[MAX_FLAGS + 1];
	_ultoa_s((unsigned long)filters, buffer, MAX_FLAGS, 16);

	for (Index index = 0; index < MAX_FLAGS && buffer[index]; ++index) {

		if (!_apply_filter_flag(leaves, buffer[index])) {

			break;
		}
	}
}

void _find_best(struct Game* game) {

	struct Game* leaves[STEPS];
	_search(game, leaves);
	_filter(leaves, get_filters(game));
	_select_random(game, leaves);
	_free_field(leaves);
}

void init_simulator(const unsigned int seed)
{
	srand(seed);
}

void automate(struct Game* game) {

	if (path_length(game) == 0) {

		_find_best(game);
	}
	else {

		follow_path(game);
	}
}