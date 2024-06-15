#include "chesscade_adapter.h"
#include <assert.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

extern "C" {

#include <rules.h>
#include <game.h>
}

godot::String godot::ChesscadeAdapter::get_board_state() const {

	return godot::String(board_state(game));
}

void godot::ChesscadeAdapter::begin_game() {

	begin(game);
}

void godot::ChesscadeAdapter::reset() {

	_clear();
	game = malloc_init_game(STANDARD_SETTINGS);
	assert(game);
}

void godot::ChesscadeAdapter::input_toggle_pause() {

	toggle_pause(game);
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::attack_pattern() const {

	return static_cast<godot::ChesscadeAdapter::GodotInt>(attack(game, false, false, True));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_combo() const {

	return static_cast<godot::ChesscadeAdapter::GodotInt>(current_combo(game));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_cursor_direction() const {

	return static_cast<godot::ChesscadeAdapter::GodotInt>(cursor_direction(game));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_cursor_grade() const {

	return current_cursor_grade(game);
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_cursor_increment() const {

	return static_cast<godot::ChesscadeAdapter::GodotInt>(current_cursor_increment(game));
}

godot::ChesscadeAdapter::GodotPiece godot::ChesscadeAdapter::get_deck_piece(godot::Vector2i slot) const {

	char ret[2] = { '\0', '\0' };
	ret[0] = deck(game, slot.y, slot.x);
	return godot::ChesscadeAdapter::GodotPiece(ret);
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_ease() const {

	return static_cast<godot::ChesscadeAdapter::GodotInt>(ease(game));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_end_time() const {

	return static_cast<godot::ChesscadeAdapter::GodotInt>(ended(game));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_events() const {

	return current_events(game);
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_forecast_rank() const {

	return static_cast<godot::ChesscadeAdapter::GodotInt>(forecast_rank(game));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_move_rate() const {

	return static_cast<godot::ChesscadeAdapter::GodotInt>(move_rate(game));
}

godot::ChesscadeAdapter::GodotPiece godot::ChesscadeAdapter::get_next_piece() const {

	char ret[2] = { '\0', '\0' };
	ret[0] = next_piece(game);
	return godot::ChesscadeAdapter::GodotPiece(ret);
}

godot::ChesscadeAdapter::GodotPiece godot::ChesscadeAdapter::get_player() const {

	char ret[2] = { '\0', '\0' };
	ret[0] = player_piece(game);
	return godot::ChesscadeAdapter::GodotPiece(ret);
}

godot::Vector2i godot::ChesscadeAdapter::get_player_square() const {

	const godot::ChesscadeAdapter::GodotInt f = static_cast<godot::ChesscadeAdapter::GodotInt>(player_piece_file(game));
	const godot::ChesscadeAdapter::GodotInt r = static_cast<godot::ChesscadeAdapter::GodotInt>(player_piece_rank(game));
	return Vector2i(f, r);
}

godot::Vector2i godot::ChesscadeAdapter::get_board_size() const {

	const godot::ChesscadeAdapter::GodotInt f = static_cast<godot::ChesscadeAdapter::GodotInt>(FILES);
	const godot::ChesscadeAdapter::GodotInt r = static_cast<godot::ChesscadeAdapter::GodotInt>(RANKS);
	return Vector2i(f, r);
}

godot::ChesscadeAdapter::GodotPiece godot::ChesscadeAdapter::get_rules() const {

	return godot::ChesscadeAdapter::GodotPiece(RULES);
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_score() const {

	return current_score(game);
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_scored() const {

	return last_scored(game);
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_spawn_rank() const {

	return spawn_rank(game);
}

godot::ChesscadeAdapter::GodotPiece godot::ChesscadeAdapter::get_square(const godot::Vector2i square) const {

	char ret[2] = { '\0', '\0' };
	ret[0] = square_contents(game, square.y, square.x);
	return godot::ChesscadeAdapter::GodotPiece(ret);
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_square_bit(const godot::Vector2i square) const {

	return square_bit(square.y, square.x);
}

void godot::ChesscadeAdapter::input_analog_move(const godot::Vector2 move) {

	do_analog_move(game, move.x, move.y);
}

void godot::ChesscadeAdapter::input_digital_move(const bool left, const bool right, const bool down) {

	do_digital_move(game, left, right, down);
}

void godot::ChesscadeAdapter::input_drop() {

	do_drop(game);
}

bool godot::ChesscadeAdapter::is_game_over() const {

	return game_over(game);
}

bool godot::ChesscadeAdapter::is_on_brink() const {

	return on_brink(game);
}

bool godot::ChesscadeAdapter::is_paused() const {

	return paused(game);
}

bool godot::ChesscadeAdapter::is_repeat() const {

	return repeated(game);
}

void godot::ChesscadeAdapter::pump_game(double delta) {

	pump(game, static_cast<Time>(floor(delta * 1000.0)));
}

void godot::ChesscadeAdapter::_clear() {

	if (game) {

		free_game(game);
		game = NULL;
	}
}

void godot::ChesscadeAdapter::_ready() {

	reset();
}

void godot::ChesscadeAdapter::_exit_tree() {

	_clear();
}

void godot::ChesscadeAdapter::_bind_methods() {

	godot::ClassDB::bind_method(godot::D_METHOD("get_board_state"), &godot::ChesscadeAdapter::get_board_state);
	godot::ClassDB::bind_method(godot::D_METHOD("reset"), &godot::ChesscadeAdapter::reset);
	godot::ClassDB::bind_method(godot::D_METHOD("get_combo"), &godot::ChesscadeAdapter::get_combo);
	godot::ClassDB::bind_method(godot::D_METHOD("get_cursor_direction"), &godot::ChesscadeAdapter::get_cursor_direction);
	godot::ClassDB::bind_method(godot::D_METHOD("get_cursor_grade"), &godot::ChesscadeAdapter::get_cursor_grade);
	godot::ClassDB::bind_method(godot::D_METHOD("get_cursor_increment"), &godot::ChesscadeAdapter::get_cursor_increment);
	godot::ClassDB::bind_method(godot::D_METHOD("get_deck_piece", "slot"), &godot::ChesscadeAdapter::get_deck_piece);
	godot::ClassDB::bind_method(godot::D_METHOD("get_ease"), &godot::ChesscadeAdapter::get_ease);
	godot::ClassDB::bind_method(godot::D_METHOD("get_end_time"), &godot::ChesscadeAdapter::get_end_time);
	godot::ClassDB::bind_method(godot::D_METHOD("get_events"), &godot::ChesscadeAdapter::get_events);
	godot::ClassDB::bind_method(godot::D_METHOD("input_toggle_pause"), &godot::ChesscadeAdapter::input_toggle_pause);
	godot::ClassDB::bind_method(godot::D_METHOD("get_player"), &godot::ChesscadeAdapter::get_player);
	godot::ClassDB::bind_method(godot::D_METHOD("attack_pattern"), &godot::ChesscadeAdapter::attack_pattern);
	godot::ClassDB::bind_method(godot::D_METHOD("get_forecast_rank"), &godot::ChesscadeAdapter::get_forecast_rank);
	godot::ClassDB::bind_method(godot::D_METHOD("get_move_rate"), &godot::ChesscadeAdapter::get_move_rate);
	godot::ClassDB::bind_method(godot::D_METHOD("get_player_square"), &godot::ChesscadeAdapter::get_player_square);
	godot::ClassDB::bind_method(godot::D_METHOD("get_board_size"), &godot::ChesscadeAdapter::get_board_size);
	godot::ClassDB::bind_method(godot::D_METHOD("get_rules"), &godot::ChesscadeAdapter::get_rules);
	godot::ClassDB::bind_method(godot::D_METHOD("get_score"), &godot::ChesscadeAdapter::get_score);
	godot::ClassDB::bind_method(godot::D_METHOD("get_scored"), &godot::ChesscadeAdapter::get_scored);
	godot::ClassDB::bind_method(godot::D_METHOD("get_spawn_rank"), &godot::ChesscadeAdapter::get_spawn_rank);
	godot::ClassDB::bind_method(godot::D_METHOD("get_square", "square"), &godot::ChesscadeAdapter::get_square);
	godot::ClassDB::bind_method(godot::D_METHOD("get_square_bit", "square"), &godot::ChesscadeAdapter::get_square_bit);
	godot::ClassDB::bind_method(godot::D_METHOD("input_analog_move", "move"), &godot::ChesscadeAdapter::input_analog_move);
	godot::ClassDB::bind_method(godot::D_METHOD("input_digital_move", "left", "right", "down"), &godot::ChesscadeAdapter::input_digital_move);
	godot::ClassDB::bind_method(godot::D_METHOD("input_drop"), &godot::ChesscadeAdapter::input_drop);
	godot::ClassDB::bind_method(godot::D_METHOD("is_game_over"), &godot::ChesscadeAdapter::is_game_over);
	godot::ClassDB::bind_method(godot::D_METHOD("is_on_brink"), &godot::ChesscadeAdapter::is_on_brink);
	godot::ClassDB::bind_method(godot::D_METHOD("is_paused"), &godot::ChesscadeAdapter::is_paused);
}

godot::ChesscadeAdapter::ChesscadeAdapter() {

	godot::UtilityFunctions::print("Hello?");
}

godot::ChesscadeAdapter::~ChesscadeAdapter() {

	_clear();
}
