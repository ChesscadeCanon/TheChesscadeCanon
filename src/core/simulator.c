#include "simulator.h"
#include "game.h"
void churn(const Count games) {

	for (Count g = 0; g < games; ++g) {

		struct Game* game = malloc_init_game(DEFAULT_SETTINGS);

		while (!game_over(game)) {

			pump(game, ease(game));
		}

		print_board_state(game);
		free_game(game);
	}
}