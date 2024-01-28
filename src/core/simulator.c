#include "simulator.h"
#include "game.h"
void churn(const Count games, const Count fps) {

	for (Count g = 0; g < games; ++g) {

		struct Game* game = malloc_init_game(DEFAULT_SETTINGS);

		begin(game);
		while (!game_over(game)) {

			pump(game, fps);
		}

		print_board_state(game);
		free_game(game);
	}
}