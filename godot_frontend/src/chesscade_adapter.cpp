#include "chesscade_adapter.h"
#include <assert.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

extern "C" {

#include <rules.h>
#include <game.h>
}

godot::String godot::ChesscadeAdapter::get_board_state() const {

	return String(board_state(game));
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

	return static_cast<GodotInt>(attack(game, false, false, True));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_combo() const {

	return static_cast<GodotInt>(current_combo(game));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_cursor_direction() const {

	return static_cast<GodotInt>(cursor_direction(game));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_cursor_grade() const {

	return current_cursor_grade(game);
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_cursor_increment() const {

	return static_cast<GodotInt>(current_cursor_increment(game));
}

godot::ChesscadeAdapter::GodotPiece godot::ChesscadeAdapter::get_deck_piece(godot::Vector2i slot) const {

	char ret[2] = { '\0', '\0' };
	ret[0] = deck(game, slot.y, slot.x);
	return GodotPiece(ret);
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_ease() const {

	return static_cast<GodotInt>(ease(game));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_end_time() const {

	return static_cast<GodotInt>(ended(game));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_events() const {

	return current_events(game);
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_forecast_rank() const {

	return static_cast<GodotInt>(forecast_rank(game));
}

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_move_rate() const {

	return static_cast<GodotInt>(move_rate(game));
}

godot::ChesscadeAdapter::GodotPiece godot::ChesscadeAdapter::get_next_piece() const {

	char ret[2] = { '\0', '\0' };
	ret[0] = next_piece(game);
	return GodotPiece(ret);
}

godot::ChesscadeAdapter::GodotPiece godot::ChesscadeAdapter::get_player() const {

	char ret[2] = { '\0', '\0' };
	ret[0] = player_piece(game);
	return GodotPiece(ret);
}

godot::Vector2i godot::ChesscadeAdapter::get_player_square() const {

	const GodotInt f = static_cast<GodotInt>(player_piece_file(game));
	const GodotInt r = static_cast<GodotInt>(player_piece_rank(game));
	return Vector2i(f, r);
}

godot::Vector2i godot::ChesscadeAdapter::get_board_size() const {

	const GodotInt f = static_cast<GodotInt>(FILES);
	const GodotInt r = static_cast<GodotInt>(RANKS);
	return Vector2i(f, r);
}

godot::ChesscadeAdapter::GodotPiece godot::ChesscadeAdapter::get_rules() const {

	return GodotPiece(RULES);
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
	return GodotPiece(ret);
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

godot::ChesscadeAdapter::GodotInt godot::ChesscadeAdapter::get_figure(GodotPiece piece, GodotInt layer, GodotInt index) {

	if (piece.length() != 1) {
		return 0;
	}

	return figure(static_cast<Piece>(piece[0]), static_cast<enum Layer>(layer), static_cast<Index>(index));
}

void godot::ChesscadeAdapter::_ready() {

	reset();
}

void godot::ChesscadeAdapter::_exit_tree() {

	_clear();
}

void godot::ChesscadeAdapter::_bind_methods() {

	ClassDB::bind_method(D_METHOD("get_board_state"), &get_board_state);
	ClassDB::bind_method(D_METHOD("begin_game"), &begin_game);
	ClassDB::bind_method(D_METHOD("pump_game", "delta"), &pump_game);
	ClassDB::bind_method(D_METHOD("reset"), &reset);
	ClassDB::bind_method(D_METHOD("get_combo"), &get_combo);
	ClassDB::bind_method(D_METHOD("get_cursor_direction"), &get_cursor_direction);
	ClassDB::bind_method(D_METHOD("get_cursor_grade"), &get_cursor_grade);
	ClassDB::bind_method(D_METHOD("get_cursor_increment"), &get_cursor_increment);
	ClassDB::bind_method(D_METHOD("get_deck_piece", "slot"), &get_deck_piece);
	ClassDB::bind_method(D_METHOD("get_ease"), &get_ease);
	ClassDB::bind_method(D_METHOD("get_end_time"), &get_end_time);
	ClassDB::bind_method(D_METHOD("get_events"), &get_events);
	ClassDB::bind_method(D_METHOD("input_toggle_pause"), &input_toggle_pause);
	ClassDB::bind_method(D_METHOD("get_player"), &get_player);
	ClassDB::bind_method(D_METHOD("attack_pattern"), &attack_pattern);
	ClassDB::bind_method(D_METHOD("get_forecast_rank"), &get_forecast_rank);
	ClassDB::bind_method(D_METHOD("get_move_rate"), &get_move_rate);
	ClassDB::bind_method(D_METHOD("get_player_square"), &get_player_square);
	ClassDB::bind_method(D_METHOD("get_board_size"), &get_board_size);
	ClassDB::bind_method(D_METHOD("get_rules"), &get_rules);
	ClassDB::bind_method(D_METHOD("get_score"), &get_score);
	ClassDB::bind_method(D_METHOD("get_scored"), &get_scored);
	ClassDB::bind_method(D_METHOD("get_spawn_rank"), &get_spawn_rank);
	ClassDB::bind_method(D_METHOD("get_square", "square"), &get_square);
	ClassDB::bind_method(D_METHOD("get_square_bit", "square"), &get_square_bit);
	ClassDB::bind_method(D_METHOD("input_analog_move", "move"), &input_analog_move);
	ClassDB::bind_method(D_METHOD("input_digital_move", "left", "right", "down"), &input_digital_move);
	ClassDB::bind_method(D_METHOD("input_drop"), &input_drop);
	ClassDB::bind_method(D_METHOD("is_game_over"), &is_game_over);
	ClassDB::bind_method(D_METHOD("is_on_brink"), &is_on_brink);
	ClassDB::bind_method(D_METHOD("is_paused"), &is_paused);
	ClassDB::bind_method(D_METHOD("get_figure", "piece", "layer", "index"), &get_figure);
}

godot::ChesscadeAdapter::ChesscadeAdapter() :
game(NULL) {
	
}

godot::ChesscadeAdapter::~ChesscadeAdapter() {

	_clear();
}
