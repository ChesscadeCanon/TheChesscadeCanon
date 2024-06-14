#include "chesscade_adapter.h"
#include <assert.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

extern "C" {

#include <game.h>
}

void godot::ChesscadeAdapter::_ready() {

	if (game) {

		free_game(game);
		game = NULL;
	}

	game = malloc_init_game(STANDARD_SETTINGS);
}

void godot::ChesscadeAdapter::_exit_tree() {

	if (game) {

		free_game(game);
		game = NULL;
	}
}

godot::String godot::ChesscadeAdapter::get_player() const {

	char ret[2] = { '\0', '\0' };
	ret[0] = player_piece(game);
	return godot::String(ret);
}

void godot::ChesscadeAdapter::_bind_methods() {

	godot::ClassDB::bind_method(godot::D_METHOD("get_player"), &godot::ChesscadeAdapter::get_player);
}

godot::ChesscadeAdapter::ChesscadeAdapter() {

	godot::UtilityFunctions::print("Hello?");
}

godot::ChesscadeAdapter::~ChesscadeAdapter() {

	if (game) {

		free_game(game);
	}
}
