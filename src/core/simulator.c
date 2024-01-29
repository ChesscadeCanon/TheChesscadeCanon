#include "simulator.h"
#include "game.h"

Count churn(struct Game* game, const Time mpf, TACTIC_FUNCTOR(tactic)) {

	begin(game);
	Count pumps = 0;
	while (!game_over(game)) {

		++pumps;
		if (tactic) {

			tactic(game);
		}
		pump(game, mpf);
	}
	return pumps;
}