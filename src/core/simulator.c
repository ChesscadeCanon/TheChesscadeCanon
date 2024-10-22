#include "simulator.h"
#include "game.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

const char WAYS[WAY_COUNT + 1] = { DOWN, DOWN_LEFT, DOWN_RIGHT, RIGHT, LEFT, FALL, LANDED };

#define IS_LEAF(__game__) (current_events(__game__) & EVENT_LANDED)
#define STEP_GAME(__game__) (\
	SQUARE_STEP(\
		player_piece_rank(__game__),\
		player_piece_file(__game__),\
		current_cursor_increment(__game__)\
	)\
)

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

			if (path_length(previous) >= MAX_STEPS) {
				discoveries--;
				discovered[d] = NULL;
				continue;
			}

			const Count had = STEP_GAME(previous);
			
			for (Count w = 0; w < WAY_COUNT; ++w) {

				Step way = WAYS[w];
				struct Game* next = malloc_init_game_moved_copy_or_null(previous, way);

				if (!next) continue;

				const Count has = STEP_GAME(next);
				const struct Game* was = visited[has];

				if (IS_LEAF(next) && !leaves[had]) {

					leaves[had] = next;
					take_path(next, current_path(previous));
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

				take_step(next, way);
			}

			discovered[d] = NULL;
			--discoveries;
		}

		start = limit;
		limit = open;
	}

	_free_field(visited);
}

void _select_random(struct Game* game, struct Game* leaves[STEPS]) {


}

void _select_first(struct Game* game, struct Game* leaves[STEPS]) {

	for (Count p = 0; p < STEPS; ++p) {

		const struct Game* leaf = leaves[p];

		if (leaf) {

			const Step* path = current_path(leaf);

			take_path(game, path);
			break;
		}
	}
}

void _find_best(struct Game* game) {

	struct Game* leaves[STEPS];
	_search(game, leaves);
	_select_first(game, leaves);
	_free_field(leaves);
}

void init_simulator()
{
	srand((unsigned int)time(NULL));
}

void automate(struct Game* game) {

	if (path_length(game) == 0) {

		_find_best(game);
	}

	follow_path(game);
}