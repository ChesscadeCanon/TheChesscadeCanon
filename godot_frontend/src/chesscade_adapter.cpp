#include "chesscade_adapter.h"
#include <api.h>
#include <assert.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void godot::ChesscadeAdapter::_ready() {

	if (game) {

		delete_game(game);
		game = NULL;
	}

	game = malloc_init_default_game();
}

void godot::ChesscadeAdapter::_exit_tree() {

	if (game) {

		delete_game(game);
		game = NULL;
	}
}

godot::String godot::ChesscadeAdapter::get_player_piece() const {

	char ret[2] = { '\0', '\0' };
	ret[0] = get_player(game);
	return godot::String(ret);
}

void godot::ChesscadeAdapter::_bind_methods() {

	godot::ClassDB::bind_method(godot::D_METHOD("get_player_piece"), &godot::ChesscadeAdapter::get_player_piece);
}

godot::ChesscadeAdapter::ChesscadeAdapter() {

	godot::UtilityFunctions::print("Hello?");
}

godot::ChesscadeAdapter::~ChesscadeAdapter() {

	if (game) {

		delete_game(game);
	}
}
