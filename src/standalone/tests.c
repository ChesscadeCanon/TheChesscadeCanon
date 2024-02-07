#include "tests.h"
#include "simulator.h"
#include "game.h"
#include "main.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define SELECT(__test_number__, __select__) {if(__select__ >= 0 && __test_number__ != __select__) return;}

#define START_WATCH \
struct timespec __WATCH__;\
assert(timespec_get(&__WATCH__, TIME_UTC) == TIME_UTC);

#define STOP_WATCH_AS(__var__) \
struct timespec __NOW__;\
assert(timespec_get(&__NOW__, TIME_UTC) == TIME_UTC);\
Time __var__ = MILLISECONDS_DIFFERENCE(__WATCH__, __NOW__);

#define TACTIC_FUNCTOR(__var__) void (__var__)(struct Game*)
#define VALIDATION_FUNCTOR(__var__) Bool (__var__)(const struct Game* __var__)

#define TEST_FLIP_DECK 1

#if TEST_FLIP_DECK
#define WITH_THESE_SETTINGS (WHITE_PAWN_HIT_UP | BLACK_PAWN_SPAWN_LOW | WHITE_PAWN_LAND_HIGH | PAWNS_PROMOTE | KING_ON_REPEAT | DOUBLE_BISHOPS | CHECKMATE | DIAGONALS | FLIP_DECK)
#define THIS_MUST_BE_THE_NUMBER_OF_FALLS_WITH_NO_INPUT 30
#define THIS_MUST_BE_THE_FINAL_STATE_WITH_NO_INPUT \
"\
________\n\
________\n\
________\n\
________\n\
________\n\
________\n\
_P_____P\n\
____K___\n\
"
#define THIS_MUST_BE_THE_NUMBER_OF_FALLS_WITH_LEFT_INPUT 31
#define THIS_MUST_BE_THE_FINAL_STATE_WITH_LEFT_INPUT \
"\
Q_______\n\
P_______\n\
b_______\n\
p_______\n\
k_______\n\
p_______\n\
P_______\n\
________\n\
"
#else
#define WITH_THESE_SETTINGS (WHITE_PAWN_HIT_UP | BLACK_PAWN_SPAWN_LOW | WHITE_PAWN_LAND_HIGH | PAWNS_PROMOTE | KING_ON_REPEAT | DOUBLE_BISHOPS | CHECKMATE | DIAGONALS)
#define THIS_MUST_BE_THE_NUMBER_OF_FALLS_WITH_NO_INPUT 59
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
#endif
#define THIS_MUST_BE_THE_END_TIME_WITH_NO_INPUT(__game__) (THIS_MUST_BE_THE_NUMBER_OF_FALLS_WITH_NO_INPUT * ease(__game__))
#define THIS_MUST_BE_THE_END_TIME_WITH_LEFT_INPUT(__game__) (THIS_MUST_BE_THE_NUMBER_OF_FALLS_WITH_LEFT_INPUT * ease(__game__))

void _dead_tactic(struct Game* game) {}

void _lefty_tactic(struct Game* game) {

	do_digital_move(game, True, False, False);
}

Bool _dead_man_validator(const struct Game* game) {

	return ended(game) == THIS_MUST_BE_THE_END_TIME_WITH_NO_INPUT(game) &&
		falls(game) == THIS_MUST_BE_THE_NUMBER_OF_FALLS_WITH_NO_INPUT &&
		strncmp(THIS_MUST_BE_THE_FINAL_STATE_WITH_NO_INPUT, board_state(game), BOARD_LENGTH) == 0;
}

Bool _tenth_dentist_validator(const struct Game* game) {

	return ended(game) >= 0 ? falls(game) * ease(game) == ended(game) : falls(game) * ease(game) == time_taken(game);
}

Bool _left_hand_validator(const struct Game* game) {

	printf("Target end time: %lld\n", THIS_MUST_BE_THE_END_TIME_WITH_LEFT_INPUT(game));
	return ended(game) == THIS_MUST_BE_THE_END_TIME_WITH_LEFT_INPUT(game) &&
		falls(game) == THIS_MUST_BE_THE_NUMBER_OF_FALLS_WITH_LEFT_INPUT &&
		strncmp(THIS_MUST_BE_THE_FINAL_STATE_WITH_LEFT_INPUT, board_state(game), BOARD_LENGTH) == 0;
}

Count _playthrough(struct Game* game, const Time mpf) {

	begin(game);
	Count pumps = 0;
	while (!game_over(game)) {

		++pumps;
		pump(game, mpf);
	}
	return pumps;
}

Count _tactical_playthrough(struct Game* game, const Time mpf, TACTIC_FUNCTOR(tactic)) {

	assert(tactic);
	begin(game);
	Count pumps = 0;
	while (!game_over(game)) {

		tactic(game);
		++pumps;
		pump(game, mpf);
	}
	return pumps;
}

Count _validated_playthrough(struct Game* game, const Time mpf, VALIDATION_FUNCTOR(validator)) {

	assert(validator);
	begin(game);
	Count pumps = 0;
	while (!game_over(game)) {

		if (!validator(game)) return pumps;
		++pumps;
		pump(game, mpf);
	}
	return pumps;
}

void _single_game_test(struct Game* game, TACTIC_FUNCTOR(tactic), Time mpf, Bool verbose) {

	const Count pumps = _tactical_playthrough(game, mpf, tactic);
	assert(ended(game) >= 0);

	if (verbose) {

		printf("finished in %lld milliseconds game time, with %llu pumps\n", ended(game), pumps);
		printf("final board state:\n%s", board_state(game));
	}
}

Time _custom_ease_not_thread_safe = 1;

Time _test_ease_functor_not_thread_safe(const struct Game* game) {

	return _custom_ease_not_thread_safe;
}

Time _ease_up_test(TACTIC_FUNCTOR(tactic), VALIDATION_FUNCTOR(validator), const Time min_ease, const Time mpf, const Bool verbose) {

	Time ret = 0;
	_custom_ease_not_thread_safe = min_ease;

	while(_custom_ease_not_thread_safe < 4096) {

		if (_custom_ease_not_thread_safe > 2048) {

			_custom_ease_not_thread_safe = 3000;
		}

		struct Game* game = malloc_init_standard_game_with_ease_functor(_test_ease_functor_not_thread_safe);

		if (_custom_ease_not_thread_safe < move_rate(game) * 36) _custom_ease_not_thread_safe = move_rate(game) * 36;

		if (verbose) {

			printf("Trying game with ease = %lld\n", _custom_ease_not_thread_safe);
		}

		_single_game_test(game, tactic, mpf, verbose);
		assert(validator(game));
		ret += ended(game);
		free_game(game);
		_custom_ease_not_thread_safe <<= 1;
	}

	return ret;
}

Time _tick_bloat_test(TACTIC_FUNCTOR(tactic), VALIDATION_FUNCTOR(validator), Time min_ease, Time max_mpf, Time tolerance, Bool verbose) {

	Count round = 0;
	Time ret = 0;
	Time game_time = -1;
	for (Time mpf = 1; mpf <= max_mpf; ++mpf) {

		++round;

		if (verbose) {

			printf("beginning round %llu, %lld milliseconds per frame\n", round, mpf);
		}

		const Time gt = _ease_up_test(tactic, validator, min_ease, mpf, verbose);

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

void _test_0(const int select, const Bool verbose) {

	SELECT(0, select);
	printf("Executing test 0: The Dead Man's Game\n");
	printf("%lld games will be simulated with no user input.\n", THIS_MUST_BE_THE_END_TIME_WITH_NO_INPUT(NULL));
	printf("%lld ought to be the length in milliseconds of any (the) game with no user input.\n", THIS_MUST_BE_THE_END_TIME_WITH_NO_INPUT(NULL));
	printf("Each round will be played with milliseconds per frame set to its index in the series.\n");
	printf("In each round, games will be played with ease (fall rate) equal to 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, and 3000.\n");
	printf("After each game, the state of the board will be compared to the known final state of the Dead Man's Game.\n");
	printf("If all games end in the correct state, the test will pass.\n");
	printf("This test will help demonstrate that frame rate has no meaningful impact on the course of the game.\n");
	START_WATCH;
	const Time game_time = _tick_bloat_test(_dead_tactic, _dead_man_validator, 1, THIS_MUST_BE_THE_END_TIME_WITH_NO_INPUT(NULL), 0, verbose);
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
	printf("This test is designed to help pinpoint exactly on what turn the discrepancy occurs.\n");
	struct Game* game = malloc_init_game(WITH_THESE_SETTINGS);
	const Count pumps = _validated_playthrough(game, 10, _tenth_dentist_validator);
	printf("The game ended after fall %llu on millisecond %lld at pump %llu\n", falls(game), ended(game), pumps);
	assert(_tenth_dentist_validator(game));
	free_game(game);
	printf("test 1 complete\n");
}

void _test_2(const int select, const Bool verbose) {

	SELECT(2, select);
	const Time mpf = 60;
	printf("Executing test 2: The Left Hand Path\n");
	printf("This test is similar to the Dead Man's Game, except the left input is held the whole time.\n");
	START_WATCH;
	const Time game_time = _tick_bloat_test(_lefty_tactic, _left_hand_validator, move_rate(NULL) * (FILES-1), THIS_MUST_BE_THE_END_TIME_WITH_LEFT_INPUT(NULL), 0, verbose);
	STOP_WATCH_AS(difference);
	printf("Completed %lld milliseconds of game time in %lld milliseconds.\n", game_time, difference);
	printf("test 2 complete\n");
}

void _test_3(const int select, const Bool verbose) {

	SELECT(3, select);
	printf("Executing test 3: Lefty Loosey\n");
	printf("Test 2 revealed that when the left arrow key is held, the game ends incorrectly if the ease is lower than the milliseconds per frame.\n");
	printf("This test tries a single Left Hand Path game with ease = 64 * 7 and MPF = 64 * 7 * 2 in order to pinpoint the problom.\n");
	_custom_ease_not_thread_safe = 448;
	struct Game* game = malloc_init_standard_game_with_ease_functor(_test_ease_functor_not_thread_safe);
	const Count pumps = _tactical_playthrough(game, 463, _lefty_tactic);
	printf("The game ended after fall %llu on millisecond %lld at pump %llu\n", falls(game), ended(game), pumps);
	print_board_state(game);
	assert(_left_hand_validator(game));
	free_game(game);
	printf("test 3 complete\n");
}

void run_tests(const int select, const Bool verbose) {

	if (WITH_THESE_SETTINGS != STANDARD_SETTINGS) {

		printf("Warning: Test settings differ from standard settings!");
	}

	printf("Running tests.\n");
	_test_0(select, verbose);
	_test_1(select, verbose);
	_test_2(select, verbose);
	_test_3(select, verbose);
}