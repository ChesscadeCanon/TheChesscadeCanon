#include "tests.h"
#include "simulator.h"
#include "game.h"
#include "main.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define START_WATCH \
struct timespec __WATCH__;\
assert(timespec_get(&__WATCH__, TIME_UTC) == TIME_UTC);
#define STOP_WATCH_AS(__var__) \
struct timespec __NOW__;\
assert(timespec_get(&__NOW__, TIME_UTC) == TIME_UTC);\
Time __var__ = MILLISECONDS_DIFFERENCE(__WATCH__, __NOW__);
#define SELECT(__test_number__, __select__) {if(__select__ >= 0 && __test_number__ != __select__) return;}
#define WITH_THESE_SETTINGS (WHITE_PAWN_HIT_UP | BLACK_PAWN_SPAWN_LOW | WHITE_PAWN_LAND_HIGH | PAWNS_PROMOTE | KING_ON_REPEAT | DOUBLE_BISHOPS | CHECKMATE | DIAGONALS)
#define THIS_MUST_BE_THE_NUMBER_OF_FALLS_WITH_NO_INPUT 59
#define THIS_MUST_BE_THE_END_TIME_WITH_NO_INPUT(__game__) (THIS_MUST_BE_THE_NUMBER_OF_FALLS_WITH_NO_INPUT * ease(__game__))
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

Time _single_game_test(TACTIC_FUNCTOR(tactic), VALIDATION_FUNCTOR(validator), Time mpf, Bool verbose) {

	struct Game* game = malloc_init_game(WITH_THESE_SETTINGS);
	const Count pumps = churn(game, mpf, dead_tactic);
	assert(ended(game) >= 0);
	Time ret = ended(game);

	if (verbose) {

		printf("finished in %lld milliseconds game time, with %llu pumps\n", ended(game), pumps);
		printf("final board state:\n%s", board_state(game));
	}

	assert(validator(game));
	free_game(game);
	return ret;
}

Time _tick_bloat_test(TACTIC_FUNCTOR(tactic), VALIDATION_FUNCTOR(validator), Time max_mpf, Time tolerance, Bool verbose) {

	Count round = 0;
	Time ret = 0;
	Time game_time = -1;
	for (Time mpf = 1; mpf <= max_mpf; ++mpf) {

		++round;

		if (verbose) {

			printf("beginning round %llu, %lld milliseconds per frame\n", round, mpf);
		}

		const Time gt = _single_game_test(tactic, validator, mpf, verbose);

		if (verbose) {

			printf("game %llu completed in %lld milliseconds game time\n", round, gt);

			if (game_time >= 0) {

				printf("previous game took %lld milliseconds\n", game_time);
			}
		}

		ret += gt;
		game_time = gt;
	}

	return ret;
}

Bool _dead_man_validator(const struct Game* game) {

	return ended(game) == THIS_MUST_BE_THE_END_TIME_WITH_NO_INPUT(game) &&
	falls(game) == THIS_MUST_BE_THE_NUMBER_OF_FALLS_WITH_NO_INPUT &&
	strncmp(THIS_MUST_BE_THE_FINAL_STATE_WITH_NO_INPUT, board_state(game), BOARD_LENGTH) == 0;
}

Bool _tenth_dentist_validator(const struct Game* game) {

	return falls(game) == ended(game) || falls(game) == milliseconds(game);
}

void _test_0(const int select, const Bool verbose) {

	SELECT(0, select);
	printf("Executing test 0: The Dead Man's Game\n");
	printf("%lld games will be simulated with no user input.\n", THIS_MUST_BE_THE_END_TIME_WITH_NO_INPUT(NULL));
	printf("%lld ought to be the length in milliseconds of any (the) game with no user input.\n", THIS_MUST_BE_THE_END_TIME_WITH_NO_INPUT(NULL));
	printf("Each game will be played with milliseconds per frame set to its index in the series.\n");
	printf("After the game, the state of the board will be compared to the known final state of the Dead Man's Game.\n");
	printf("If all games end in the correct state, the test will pass.\n");
	printf("This test will help demonstrate that frame rate has no meaningful impact on the course of the game.\n");
	START_WATCH;
	const Time game_time = _tick_bloat_test(dead_tactic, _dead_man_validator, THIS_MUST_BE_THE_END_TIME_WITH_NO_INPUT(NULL), 0, verbose);
	STOP_WATCH_AS(difference);
	printf("Completed %lld milliseconds of game time in %lld milliseconds.\n", game_time, difference);
	printf("test 0 complete\n");
}

void _test_1(const int select, const Bool verbose) {

	SELECT(1, select);
	printf("Executing test 1: The Tenth Dentist\n");
	printf("Test 0 revealed that when MPF = 10 and ease = 1, a discrepancy begins to appear.\n");
	printf("MPF 0-9 are fine; the number of milliseconds before game over stays constant at %d.\n", THIS_MUST_BE_THE_NUMBER_OF_FALLS_WITH_NO_INPUT);
	printf("This is correct, because that many falls occur in the Dead Man's Game.\n");
	printf("But at 10, the number of milliseconds before a game over is 57.\n");
	printf("After that, the end time sporadically fluctuates around 59.\n");
	printf("From 59 mpf onward, games take exactly 7 milliseconds.\n");
	printf("This test is designed to help pintpoint exactly on what turn the discrepancy occurs.\n");
	struct Game* game = malloc_init_game(WITH_THESE_SETTINGS);
	const Count pumps = lurk(game, 10, _tenth_dentist_validator);
	printf("The game ended after fall %llu on millisecond %lld at pump %llu\n", falls(game), ended(game), pumps);
	assert(_tenth_dentist_validator(game));
	free_game(game);
}

void run_tests(const int select, const Bool verbose) {

	if (WITH_THESE_SETTINGS != STANDARD_SETTINGS) {

		printf("Warning: Test settings differ from standard settings!");
	}

	printf("Running tests.\n");
	_test_0(select, verbose);
	_test_1(select, verbose);
}