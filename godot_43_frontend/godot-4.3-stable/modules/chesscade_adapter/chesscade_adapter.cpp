#include "chesscade_adapter.h"
#include <assert.h>
#include <core/object/class_db.h>

extern "C" {

#include <rules.h>
#include <game.h>
}

String ChesscadeAdapter::get_board_state() const {

	return String(board_state(game));
}

void ChesscadeAdapter::begin_game() {

	begin(game);
}

void ChesscadeAdapter::reset() {

	clear();
	game = malloc_init_game(STANDARD_SETTINGS);
	assert(game);
}

void ChesscadeAdapter::input_toggle_pause() {

	toggle_pause(game);
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::attack_pattern() const {

	return static_cast<GodotInt>(attack(game, false, false, True));
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::forecast_captures() const
{
	return static_cast<GodotInt>(attack(game, false, True, false));
}

Vector2i ChesscadeAdapter::get_deck_size() const {

	return Vector2i(FILES, GRADES);
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::get_combo() const {

	return static_cast<GodotInt>(current_combo(game));
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::get_cursor_direction() const {

	return static_cast<GodotInt>(cursor_direction(game));
}

Vector2i ChesscadeAdapter::get_cursor() const {

	const GodotInt grade = static_cast<GodotInt>(current_cursor_grade(game));
	const GodotInt increment = static_cast<GodotInt>(current_cursor_increment(game));
	return Vector2i(increment, grade);
}

ChesscadeAdapter::GodotPiece ChesscadeAdapter::get_deck_piece(Vector2i slot) const {

	char ret[2] = { '\0', '\0' };
	ret[0] = deck(game, slot.y, slot.x);
	return GodotPiece(ret);
}

double ChesscadeAdapter::get_ease() const {

	const long double ret = static_cast<long double>(ease(game)) / 1000.0;
	return ret;
}

double ChesscadeAdapter::get_end_time() const {

	const long double ret = static_cast<long double>(ended(game)) / 1000.0;
	return ret;
}

double ChesscadeAdapter::get_time() const
{
	const long double ret = static_cast<long double>(milliseconds(game)) / 1000.0;
	return ret;
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::get_events() const {

	return current_events(game);
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::get_forecast_rank() const {

	return static_cast<GodotInt>(forecast_rank(game));
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::get_move_rate() const {

	return static_cast<GodotInt>(move_rate(game));
}

ChesscadeAdapter::GodotPiece ChesscadeAdapter::get_next_piece() const {

	char ret[2] = { '\0', '\0' };
	ret[0] = next_piece(game);
	return GodotPiece(ret);
}

ChesscadeAdapter::GodotPiece ChesscadeAdapter::get_player() const {

	char ret[2] = { '\0', '\0' };
	ret[0] = player_piece(game);
	return GodotPiece(ret);
}

Vector2i ChesscadeAdapter::get_player_square() const {

	const GodotInt f = static_cast<GodotInt>(player_piece_file(game));
	const GodotInt r = static_cast<GodotInt>(player_piece_rank(game));
	return Vector2i(f, r);
}

Vector2i ChesscadeAdapter::get_board_size() const {

	const GodotInt f = static_cast<GodotInt>(FILES);
	const GodotInt r = static_cast<GodotInt>(RANKS);
	return Vector2i(f, r);
}

ChesscadeAdapter::GodotPiece ChesscadeAdapter::get_rules() const {

	return GodotPiece(RULES);
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::get_score() const {

	return current_score(game);
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::get_scored() const {

	return last_scored(game);
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::get_spawn_rank() const {

	return spawn_rank(game);
}

ChesscadeAdapter::GodotPiece ChesscadeAdapter::get_square(const Vector2i square) const {

	char ret[2] = { '\0', '\0' };
	ret[0] = square_contents(game, square.y, square.x);
	return GodotPiece(ret);
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::get_square_bit(const Vector2i square) const {

	return square_bit(square.y, square.x);
}

void ChesscadeAdapter::input_analog_move(const Vector2 move) {

	do_analog_move(game, move.x, move.y);
}

void ChesscadeAdapter::input_digital_move(const bool left, const bool right, const bool down) {

	do_digital_move(game, left, right, down);
}

void ChesscadeAdapter::input_drop() {

	do_drop(game);
}

bool ChesscadeAdapter::is_game_over() const {

	return game_over(game);
}

bool ChesscadeAdapter::is_on_brink() const {

	return on_brink(game);
}

bool ChesscadeAdapter::is_paused() const {

	return paused(game);
}

bool ChesscadeAdapter::is_repeat() const {

	return repeated(game);
}

void ChesscadeAdapter::pump_game(double delta) {

	pump(game, static_cast<Time>(floor(delta * 1000.0)));
}

void ChesscadeAdapter::clear() {

	if (game) {

		free_game(game);
		game = NULL;
	}
}

ChesscadeAdapter::GodotInt ChesscadeAdapter::get_figure(GodotPiece piece, GodotInt layer, GodotInt index) {

	if (String(piece).length() != 1) {

		return 0;
	}

	return figure(static_cast<Piece>(String(piece).to_ascii_buffer()[0]), static_cast<enum Layer>(layer), static_cast<Index>(index));
}

void ChesscadeAdapter::_bind_methods() {

	ClassDB::bind_method(D_METHOD("get_board_state"), &get_board_state);
	ClassDB::bind_method(D_METHOD("begin_game"), &begin_game);
	ClassDB::bind_method(D_METHOD("pump_game", "delta"), &pump_game);
	ClassDB::bind_method(D_METHOD("reset"), &reset);
	ClassDB::bind_method(D_METHOD("clear"), &clear);
	ClassDB::bind_method(D_METHOD("get_combo"), &get_combo);
	ClassDB::bind_method(D_METHOD("get_deck_size"), &get_deck_size);
	ClassDB::bind_method(D_METHOD("get_cursor_direction"), &get_cursor_direction);
	ClassDB::bind_method(D_METHOD("get_cursor"), &get_cursor);
	ClassDB::bind_method(D_METHOD("get_deck_piece", "slot"), &get_deck_piece);
	ClassDB::bind_method(D_METHOD("get_ease"), &get_ease);
	ClassDB::bind_method(D_METHOD("get_end_time"), &get_end_time);
	ClassDB::bind_method(D_METHOD("get_time"), &get_time);
	ClassDB::bind_method(D_METHOD("get_events"), &get_events);
	ClassDB::bind_method(D_METHOD("input_toggle_pause"), &input_toggle_pause);
	ClassDB::bind_method(D_METHOD("get_player"), &get_player);
	ClassDB::bind_method(D_METHOD("attack_pattern"), &attack_pattern);
	ClassDB::bind_method(D_METHOD("forecast_captures"), &forecast_captures);
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
	ClassDB::bind_method(D_METHOD("is_repeat"), &is_repeat);
	ClassDB::bind_method(D_METHOD("get_figure", "piece", "layer", "index"), &get_figure);
}

ChesscadeAdapter::ChesscadeAdapter() :
game(NULL) {
	
}

ChesscadeAdapter::~ChesscadeAdapter() {

	clear();
}
