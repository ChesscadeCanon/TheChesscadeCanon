#include "simulator.h"
#include "game.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

typedef char Step;
typedef char* Path;
typedef Path* Guide;
typedef struct Game** Field;

#define IS_LEAF(__game__) (current_events(__game__) & EVENT_LANDED)
#define STEP_FALLS(__step__) (__step__ / BOARD_LENGTH)
#define STEP_INDEX(__step__) (__step__ % BOARD_LENGTH)
#define INDEX_STEP(__index__, __falls__) (__index__ * __falls__)
#define SQUARE_STEP(__rank__, __file__, __falls__) (INDEX_STEP(SQUARE_INDEX(__rank__, __file__), __falls__))
#define STEP_GAME(__game__) (\
	SQUARE_STEP(\
		player_piece_rank(__game__),\
		player_piece_file(__game__),\
		current_cursor_increment(__game__)\
	)\
)
#define MAX_PATH ((RANKS >> 1) * FILES + (RANKS >> 1))
#define STEPS (BOARD_LENGTH * RANKS)
#define GUIDE_LENGTH (MAX_PATH * STEPS)
#define GUIDE_PATH(__guide__, __index__) (__guide__ + (__index__ * MAX_PATH))
#define WAY_COUNT 6
const char WAYS[WAY_COUNT + 1] = "dlrDLR";

void _init_path(Step path[MAX_PATH]) {

	memset(path, LANDED, MAX_PATH * sizeof(Step));
}

void _init_guide(Step guide[GUIDE_LENGTH]) {

	memset(guide, LANDED, GUIDE_LENGTH * sizeof(Step));
}

void _init_field(struct Game* field[STEPS]) {

	memset(field, 0, STEPS * sizeof(struct Game*));
}

Count _trace(const struct Game* game, Step path[MAX_PATH]) {

	if (!game) return 0;
	else {
		const Count ret = _trace(get_source(game), path);
		path[ret] = last_move(game);
		return ret + 1;
	}
}

void _free_field(struct Game* field[STEPS]) {

	for (Count i = 0; i < STEPS; ++i) {

		if(field[i]) free_game_shallow(field[i]);
	}
}

void _search(struct Game* game, Step guide[GUIDE_LENGTH], struct Game* leaves[STEPS]) {

	struct Game* discovered[STEPS];
	struct Game* visited[STEPS];
	Count discoveries = 1;
	Count open = 0;
	_init_field(discovered);
	_init_field(visited);
	_init_field(leaves);
	_init_guide(guide);
	discovered[open++] = game;

	for (Count start = 0, limit = 1; discoveries > 0;) {

		for (Count d = start; d < limit; ++d) {

			struct Game* previous = discovered[d];
			assert(previous);
			const Count had = STEP_GAME(previous);
			
			for (Count w = 0; w < WAY_COUNT; ++w) {

				char way = WAYS[w];
				struct Game* next = malloc_init_game_shallow_copy(previous);

				switch (way) {

				case DOWN: do_digital_move(next, 0, 0, 1); break;
				case LEFT: do_digital_move(next, 1, 0, 0); break;
				case RIGHT: do_digital_move(next, 0, 1, 0); break;
				case DOWN_LEFT: do_digital_move(next, 1, 0, 1); break;
				case DOWN_RIGHT: do_digital_move(next, 0, 1, 1); break;
				case FALL: break;
				default: assert(False);
				}

				if (way == FALL) {

					pump(next, ease(next));
				}
				else {

					pump(next, move_rate(next));
				}

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

	for (Count p = 0; p < STEPS; ++p) {

		const struct Game* outcome = leaves[p];
		_trace(outcome, GUIDE_PATH(guide, p));
	}

	_free_field(visited);
}

void find_best(struct Game* game) {

	Step* guide = malloc(sizeof(Step) * GUIDE_LENGTH);
	struct Game* leaves[STEPS];
	_search(game, guide, leaves);
	free(guide);
	_free_field(leaves);
}