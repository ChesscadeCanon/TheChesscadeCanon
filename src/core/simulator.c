#include "simulator.h"
#include "game.h"
#include <assert.h>

void dead_tactic(struct Game* game) {

}

Count churn(struct Game* game, const Time mpf, TACTIC_FUNCTOR(tactic)) {

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

Count lurk(struct Game* game, const Time mpf, VALIDATION_FUNCTOR(validator))
{
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
