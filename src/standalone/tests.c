#include "tests.h"
#include "simulator.h"
#include "game.h"
#include "main.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define SELECT(__test_number__, __select__) {if(__select__ >= 0 && __test_number__ != __select__) return;}
#define WITH_THESE_SETTINGS (WHITE_PAWN_HIT_UP | BLACK_PAWN_SPAWN_LOW | WHITE_PAWN_LAND_HIGH | PAWNS_PROMOTE | KING_ON_REPEAT | DOUBLE_BISHOPS | CHECKMATE | DIAGONALS)
#define THIS_MUST_BE_THE_FINAL_STATE_WITH_NO_INPUT \
"\
________\n\
________\n\
________\n\
________\n\
________\n\
_______K\n\
_______P\n\
__q_KKK_\n\
"

void _test_0(int select, Bool verbose) {

	SELECT(0, select);
	printf("Executing test 0: The Dead Man's Game\n");
	printf("135,936 games will be simulated with no user input.\n");
	printf("135,936 ought to be the length in milliseconds of any (the) game with no user input.\n");
	printf("Each game will be played with milliseconds per frame set to its index in the series.\n");
	printf("After the game, the state of the board will be compared to the known final state of the Dead Man's Game.\n");
	printf("If all games end in the correct state, the test will pass.\n");
	printf("This test will help demonstrate that frame rate has no meaningful impact on the course of the game.\n");
	struct timespec then;
	assert(timespec_get(&then, TIME_UTC) == TIME_UTC);
	Count round = 0;
	Time game_time = 0;
	for (Time mpf = 1; mpf <= 135936; ++mpf) {

		++round;
		if (verbose) {

			printf("beginning round %llu, %lld milliseconds per frame\n", round, mpf);
		}

		struct Game* game = malloc_init_game(WITH_THESE_SETTINGS);
		const Count pumps = churn(game, mpf, NULL);
		game_time += milliseconds(game);

		if (verbose) {

			printf("finished in %lld milliseconds game time, with %llu pumps\n", milliseconds(game), pumps);
			printf("final board state:\n%s", board_state(game));
		}

		assert(strncmp(THIS_MUST_BE_THE_FINAL_STATE_WITH_NO_INPUT, board_state(game), BOARD_LENGTH) == 0);
		free_game(game);
	}

	printf("test 0 complete\n");
	struct timespec now;
	assert(timespec_get(&now, TIME_UTC) == TIME_UTC);
	Time difference = MILLISECONDS_DIFFERENCE(then, now);
	printf("Completed %lld milliseconds of game time in %lld milliseconds.\n", game_time, difference);
}

void run_tests(int select, Bool verbose) {

	if (WITH_THESE_SETTINGS != STANDARD_SETTINGS) {

		printf("Warning: Test settings differ from standard settings!");
	}

	printf("Running tests.\n");
	_test_0(select, verbose);
}